#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <cstdint>
#include <cstring>

#define FLUX_EXPORT extern "C" __declspec(dllexport)
struct sqlite3;
struct sqlite3_stmt;
using sqlite3_int64 = long long;
static constexpr int SQLITE_OK=0, SQLITE_ROW=100, SQLITE_DONE=101;
static constexpr int SQLITE_INTEGER=1, SQLITE_FLOAT=2, SQLITE_TEXT=3, SQLITE_BLOB=4, SQLITE_NULL=5;
static constexpr int SQLITE_OPEN_READONLY=0x00000001, SQLITE_OPEN_READWRITE=0x00000002, SQLITE_OPEN_CREATE=0x00000004;

struct SQLiteApi {
    HMODULE dll=nullptr;
    int (__cdecl *open_v2)(const char*,sqlite3**,int,const char*)=nullptr;
    int (__cdecl *close)(sqlite3*)=nullptr;
    const char* (__cdecl *errmsg)(sqlite3*)=nullptr;
    int (__cdecl *exec)(sqlite3*,const char*,int (__cdecl *)(void*,int,char**,char**),void*,char**)=nullptr;
    void (__cdecl *freeMem)(void*)=nullptr;
    int (__cdecl *prepare_v2)(sqlite3*,const char*,int,sqlite3_stmt**,const char**)=nullptr;
    int (__cdecl *step)(sqlite3_stmt*)=nullptr;
    int (__cdecl *finalize)(sqlite3_stmt*)=nullptr;
    int (__cdecl *column_count)(sqlite3_stmt*)=nullptr;
    const char* (__cdecl *column_name)(sqlite3_stmt*,int)=nullptr;
    int (__cdecl *column_type)(sqlite3_stmt*,int)=nullptr;
    const unsigned char* (__cdecl *column_text)(sqlite3_stmt*,int)=nullptr;
    int (__cdecl *column_bytes)(sqlite3_stmt*,int)=nullptr;
    sqlite3_int64 (__cdecl *last_insert_rowid)(sqlite3*)=nullptr;
    int (__cdecl *changes)(sqlite3*)=nullptr;
    int (__cdecl *bind_null)(sqlite3_stmt*,int)=nullptr;
    int (__cdecl *bind_text)(sqlite3_stmt*,int,const char*,int,void (__cdecl *)(void*))=nullptr;
    int (__cdecl *bind_double)(sqlite3_stmt*,int,double)=nullptr;
    int (__cdecl *reset)(sqlite3_stmt*)=nullptr;
    int (__cdecl *clear_bindings)(sqlite3_stmt*)=nullptr;
};
static SQLiteApi api;
static std::once_flag initFlag;
static std::string initError;
static std::mutex dbMutex;
static std::unordered_map<int,sqlite3*> dbs;
static std::unordered_map<int,std::pair<int,sqlite3_stmt*>> statements;
static int nextId=1;
static thread_local std::string reply;

static bool loadApi() {
    std::call_once(initFlag, [] {
        // Only use an explicit, trusted SQLite DLL location: next to the FluxScript executable.
        wchar_t exe[MAX_PATH];
        DWORD n=GetModuleFileNameW(nullptr,exe,MAX_PATH);
        if(!n || n>=MAX_PATH) {initError="cannot locate executable";return;}
        std::wstring path(exe,n);
        auto pos=path.find_last_of(L"\\/");
        if(pos==std::wstring::npos){initError="invalid executable path";return;}
        path=path.substr(0,pos+1)+L"sqlite3.dll";
        api.dll=LoadLibraryExW(path.c_str(),nullptr,LOAD_WITH_ALTERED_SEARCH_PATH);
        if(!api.dll){initError="sqlite3.dll not found beside flux.exe";return;}
#define LOAD(field,name) api.field=reinterpret_cast<decltype(api.field)>(GetProcAddress(api.dll,"sqlite3_" name)); if(!api.field){initError="sqlite3.dll missing sqlite3_" name;return;}
        LOAD(open_v2,"open_v2") LOAD(close,"close") LOAD(errmsg,"errmsg")
        LOAD(exec,"exec") LOAD(freeMem,"free") LOAD(prepare_v2,"prepare_v2")
        LOAD(step,"step") LOAD(finalize,"finalize") LOAD(column_count,"column_count")
        LOAD(column_name,"column_name") LOAD(column_type,"column_type")
        LOAD(column_text,"column_text") LOAD(column_bytes,"column_bytes")
        LOAD(last_insert_rowid,"last_insert_rowid") LOAD(changes,"changes")
        LOAD(bind_null,"bind_null") LOAD(bind_text,"bind_text") LOAD(bind_double,"bind_double")
        LOAD(reset,"reset") LOAD(clear_bindings,"clear_bindings")
#undef LOAD
    });
    return initError.empty();
}
struct Arg { char type; std::string data; };
static bool parse(const char* raw,std::vector<Arg>& args,std::string& err){
    if(!raw){err="null argument buffer";return false;}
    std::string s(raw);
    if(s.compare(0,3,"F2|")!=0){err="requires FluxScript ABI v2";return false;}
    size_t p=3, sep=s.find('|',p);
    if(sep==std::string::npos || sep==p || sep-p>6){err="invalid argument count";return false;}
    for(size_t i=p;i<sep;++i) if(s[i]<'0'||s[i]>'9'){err="invalid argument count";return false;}
    size_t count=std::stoul(s.substr(p,sep-p));
    if(count>100){err="too many arguments";return false;}
    p=sep+1;
    for(size_t i=0;i<count;++i){
        if(p>=s.size() || (s[p]!='N'&&s[p]!='S')){err="invalid argument type";return false;}
        char kind=s[p++];
        size_t colon=s.find(':',p);
        if(colon==std::string::npos || colon==p || colon-p>9){err="invalid argument length";return false;}
        size_t len=0;
        for(size_t j=p;j<colon;++j){if(s[j]<'0'||s[j]>'9'){err="invalid argument length";return false;} len=len*10+(s[j]-'0');}
        p=colon+1;
        if(len>s.size()-p){err="truncated argument";return false;}
        args.push_back({kind,s.substr(p,len)});
        p+=len;
    }
    if(p!=s.size()){err="trailing argument data";return false;}
    return true;
}
static const char* result(char type,const std::string& value){reply=std::string(1,type)+":"+value;return reply.c_str();}
static const char* error(const std::string& msg){return result('E',msg);}
static const char* number(double v){std::ostringstream o;o<<std::setprecision(17)<<v;return result('N',o.str());}
static const char* str(const std::string& v){if(v.find('\0')!=std::string::npos)return error("result contains NUL; use hex encoding for binary data");return result('S',v);}
static bool argcExact(const std::vector<Arg>& a,size_t n){return a.size()==n;}
static bool getNum(const Arg& a,double& out){
    if(a.type!='N')return false;
    char* end=nullptr;out=std::strtod(a.data.c_str(),&end);
    return end!=a.data.c_str() && *end=='\0' && std::isfinite(out);
}
static bool getId(const Arg& a,int& id){double n;if(!getNum(a,n)||n<1||n>2147483647.0||std::floor(n)!=n)return false;id=(int)n;return true;}
static bool getText(const Arg& a,std::string& out){if(a.type!='S')return false;out=a.data;return true;}
static sqlite3* findDb(int id){auto it=dbs.find(id);return it==dbs.end()?nullptr:it->second;}
static const char* dbError(sqlite3* db){const char* m=api.errmsg(db);return error(m?m:"SQLite error");}
static int allocateId(){if(nextId==std::numeric_limits<int>::max())return 0;return nextId++;}
static std::string escapeJson(const std::string& s){
    static const char* hex="0123456789abcdef";
    std::string out="\"";
    for(unsigned char c:s){
        switch(c){case '"':out+="\\\"";break;case '\\':out+="\\\\";break;case '\n':out+="\\n";break;case '\r':out+="\\r";break;case '\t':out+="\\t";break;
        default:if(c<32){out+="\\u00";out+=hex[c>>4];out+=hex[c&15];}else out+=(char)c;}
    }
    return out+'"';
}
static std::string columnValue(sqlite3_stmt* st,int col){
    int type=api.column_type(st,col);
    if(type==SQLITE_NULL)return "null";
    if(type==SQLITE_BLOB){
        const unsigned char* data=api.column_text(st,col);
        int len=api.column_bytes(st,col);
        if(!data || len<0)return "null";
        // Blob values are represented as text by SQLite's conversion; prefer hex SQL for binary.
        std::string raw(reinterpret_cast<const char*>(data),(size_t)len);
        return escapeJson(raw);
    }
    const unsigned char* data=api.column_text(st,col);
    int len=api.column_bytes(st,col);
    if(!data || len<0)return "null";
    std::string val(reinterpret_cast<const char*>(data),(size_t)len);
    if(type==SQLITE_INTEGER||type==SQLITE_FLOAT)return val;
    return escapeJson(val);
}
static const char* dispatch(const std::string& fn,const std::vector<Arg>& a){
    if(!loadApi())return error(initError);
    std::lock_guard<std::mutex> guard(dbMutex);
    if(fn=="open"||fn=="openReadOnly"){
        std::string path;if(!argcExact(a,1)||!getText(a[0],path)||path.empty())return error("expected database path string");
        sqlite3* db=nullptr;
        int flags=fn=="open"?(SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE):SQLITE_OPEN_READONLY;
        int rc=api.open_v2(path.c_str(),&db,flags,nullptr);
        if(rc!=SQLITE_OK){std::string e=db?(api.errmsg(db)?api.errmsg(db):"open failed"):"open failed";if(db)api.close(db);return error(e);}
        int id=allocateId();if(!id){api.close(db);return error("handle limit reached");}
        dbs[id]=db;return number(id);
    }
    if(fn=="close"){
        int id;if(!argcExact(a,1)||!getId(a[0],id))return error("expected database handle");
        sqlite3* db=findDb(id);if(!db)return error("invalid database handle");
        for(const auto& it:statements)if(it.second.first==id)return error("finalize statements before closing database");
        int rc=api.close(db);if(rc!=SQLITE_OK)return dbError(db);
        dbs.erase(id);return number(1);
    }
    if(fn=="prepare"){
        int id;std::string sql;
        if(!argcExact(a,2)||!getId(a[0],id)||!getText(a[1],sql))return error("expected database handle and SQL string");
        sqlite3* db=findDb(id);if(!db)return error("invalid database handle");
        sqlite3_stmt* st=nullptr;const char* tail=nullptr;
        int rc=api.prepare_v2(db,sql.c_str(),-1,&st,&tail);
        if(rc!=SQLITE_OK){if(st)api.finalize(st);return dbError(db);}
        if(!st)return error("empty SQL statement");
        if(tail){while(*tail==' '||*tail=='\n'||*tail=='\t'||*tail=='\r'||*tail==';')++tail;if(*tail){api.finalize(st);return error("prepare accepts one SQL statement");}}
        int sid=allocateId();if(!sid){api.finalize(st);return error("handle limit reached");}
        statements[sid]={id,st};return number(sid);
    }
    if(fn=="finalize"||fn=="reset"||fn=="clearBindings"||fn=="step"||fn=="columnCount"||fn=="rowJson"){
        int sid;if(!argcExact(a,1)||!getId(a[0],sid))return error("expected statement handle");
        auto it=statements.find(sid);if(it==statements.end())return error("invalid statement handle");
        sqlite3_stmt* st=it->second.second;sqlite3* db=findDb(it->second.first);
        if(fn=="finalize"){int rc=api.finalize(st);statements.erase(it);if(rc!=SQLITE_OK)return error("finalize failed");return number(1);}
        if(fn=="reset"){int rc=api.reset(st);return rc==SQLITE_OK?number(1):dbError(db);}
        if(fn=="clearBindings"){int rc=api.clear_bindings(st);return rc==SQLITE_OK?number(1):dbError(db);}
        if(fn=="step"){int rc=api.step(st);if(rc==SQLITE_ROW)return number(1);if(rc==SQLITE_DONE)return number(0);return dbError(db);}
        if(fn=="columnCount")return number(api.column_count(st));
        int cols=api.column_count(st);std::string json="{";
        for(int i=0;i<cols;++i){if(i)json+=",";const char* name=api.column_name(st,i);json+=escapeJson(name?name:"");json+=':';json+=columnValue(st,i);}
        return str(json+"}");
    }
    if(fn=="bindText"||fn=="bindNumber"||fn=="bindNull"){
        int sid,index;
        if(a.size()!=(fn=="bindNull"?2u:3u)||!getId(a[0],sid)||!getId(a[1],index))return error("expected statement handle, 1-based parameter index, and optional value");
        auto it=statements.find(sid);if(it==statements.end())return error("invalid statement handle");
        sqlite3_stmt* st=it->second.second;int rc;
        if(fn=="bindNull")rc=api.bind_null(st,index);
        else if(fn=="bindText"){
            std::string value;if(!getText(a[2],value))return error("expected string value");
            if(value.size()>INT32_MAX)return error("string too long");
            // SQLITE_TRANSIENT = (sqlite3_destructor_type)-1
            auto transient=reinterpret_cast<void (__cdecl *)(void*)>(static_cast<intptr_t>(-1));
            rc=api.bind_text(st,index,value.data(),(int)value.size(),transient);
        }else{
            double value;if(!getNum(a[2],value))return error("expected numeric value");
            rc=api.bind_double(st,index,value);
        }
        return rc==SQLITE_OK?number(1):dbError(findDb(it->second.first));
    }
    if(fn=="exec"||fn=="scalar"||fn=="queryJson"){
        int id;std::string sql;
        if(!argcExact(a,2)||!getId(a[0],id)||!getText(a[1],sql))return error("expected database handle and SQL string");
        sqlite3* db=findDb(id);if(!db)return error("invalid database handle");
        if(fn=="exec"){
            char* err=nullptr;int rc=api.exec(db,sql.c_str(),nullptr,nullptr,&err);
            if(rc!=SQLITE_OK){std::string message=err?err:"SQL execution failed";if(err)api.freeMem(err);return error(message);}
            return number(api.changes(db));
        }
        sqlite3_stmt* st=nullptr;const char* tail=nullptr;
        int rc=api.prepare_v2(db,sql.c_str(),-1,&st,&tail);
        if(rc!=SQLITE_OK){if(st)api.finalize(st);return dbError(db);}
        if(!st)return error("empty SQL statement");
        if(tail){while(*tail==' '||*tail=='\n'||*tail=='\r'||*tail=='\t'||*tail==';')++tail;if(*tail){api.finalize(st);return error("only one SQL statement allowed");}}
        rc=api.step(st);
        if(fn=="scalar"){
            if(rc==SQLITE_DONE){api.finalize(st);return str("");}
            if(rc!=SQLITE_ROW){std::string msg=api.errmsg(db);api.finalize(st);return error(msg);}
            if(api.column_count(st)<1){api.finalize(st);return str("");}
            int type=api.column_type(st,0);
            if(type==SQLITE_NULL){api.finalize(st);return str("");}
            const unsigned char* ptr=api.column_text(st,0);int len=api.column_bytes(st,0);
            std::string value=ptr&&len>=0?std::string((const char*)ptr,(size_t)len):"";
            api.finalize(st);
            if(type==SQLITE_INTEGER||type==SQLITE_FLOAT){char* end=nullptr;double v=std::strtod(value.c_str(),&end);if(end!=value.c_str()&&*end=='\0'&&std::isfinite(v))return number(v);}
            return str(value);
        }
        std::string json="[";int rows=0;
        while(rc==SQLITE_ROW){
            if(rows>=1000){api.finalize(st);return error("queryJson limit: 1000 rows; use prepared statements for larger results");}
            if(rows++)json+=",";
            json+="{";int cols=api.column_count(st);
            for(int i=0;i<cols;++i){if(i)json+=",";const char* name=api.column_name(st,i);json+=escapeJson(name?name:"");json+=':';json+=columnValue(st,i);}
            json+='}';rc=api.step(st);
        }
        if(rc!=SQLITE_DONE){std::string msg=api.errmsg(db);api.finalize(st);return error(msg);}
        api.finalize(st);return str(json+"]");
    }
    if(fn=="changes"||fn=="lastInsertId"){
        int id;if(!argcExact(a,1)||!getId(a[0],id))return error("expected database handle");
        sqlite3* db=findDb(id);if(!db)return error("invalid database handle");
        return number(fn=="changes"?(double)api.changes(db):(double)api.last_insert_rowid(db));
    }
    return error("unknown database function: "+fn);
}
static const char* invoke(const char* fn,const char* raw){
    try{std::vector<Arg> args;std::string err;if(!parse(raw,args,err))return error(err);return dispatch(fn,args);}
    catch(const std::exception& e){return error(e.what());}
    catch(...){return error("unexpected native exception");}
}
FLUX_EXPORT int __cdecl flux_abi_version(){return 2;}
#define EXPORT(name) FLUX_EXPORT const char* __cdecl flux_##name(const char* args){return invoke(#name,args);}
EXPORT(open)
EXPORT(openReadOnly)
EXPORT(close)
EXPORT(exec)
EXPORT(scalar)
EXPORT(queryJson)
EXPORT(prepare)
EXPORT(bindText)
EXPORT(bindNumber)
EXPORT(bindNull)
EXPORT(step)
EXPORT(rowJson)
EXPORT(columnCount)
EXPORT(reset)
EXPORT(clearBindings)
EXPORT(finalize)
EXPORT(changes)
EXPORT(lastInsertId)
#undef EXPORT
