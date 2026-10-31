
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "ws2_32.lib")
#define EXPORT extern "C" __declspec(dllexport)
namespace {
struct Arg { char type; std::string data; };
thread_local std::string output;
size_t readSize(const std::string& s,size_t& p,char delim) {
    if(p>=s.size() || s[p]<'0' || s[p]>'9') throw std::runtime_error("invalid length");
    size_t n=0;
    while(p<s.size() && s[p]>='0' && s[p]<='9') {
        unsigned d=unsigned(s[p++]-'0');
        if(n>(std::numeric_limits<size_t>::max()-d)/10) throw std::runtime_error("length overflow");
        n=n*10+d;
    }
    if(p>=s.size() || s[p++]!=delim) throw std::runtime_error("invalid delimiter");
    return n;
}
std::vector<Arg> parse(const char* raw) {
    if(!raw) throw std::runtime_error("null arguments");
    std::string s(raw);
    if(s.compare(0,3,"F2|")!=0) throw std::runtime_error("ABI v2 required");
    size_t p=3, count=readSize(s,p,'|');
    if(count>16) throw std::runtime_error("too many arguments");
    std::vector<Arg> a;
    for(size_t i=0;i<count;++i) {
        if(p>=s.size()) throw std::runtime_error("truncated argument");
        char t=s[p++];
        if(t!='S' && t!='N') throw std::runtime_error("invalid argument type");
        size_t n=readSize(s,p,':');
        if(n>s.size()-p) throw std::runtime_error("truncated argument");
        a.push_back({t,s.substr(p,n)});
        p+=n;
    }
    if(p!=s.size()) throw std::runtime_error("trailing bytes");
    return a;
}
void count(const std::vector<Arg>& a,size_t n) {
    if(a.size()!=n) throw std::runtime_error("wrong number of arguments");
}
std::string str(const Arg& a) {
    if(a.type!='S') throw std::runtime_error("expected string");
    return a.data;
}
uint32_t integer(const Arg& a) {
    if(a.type!='N') throw std::runtime_error("expected number");
    char* end=nullptr;
    double x=std::strtod(a.data.c_str(),&end);
    if(end==a.data.c_str() || *end || !std::isfinite(x) || x<0 || x>4294967295.0 || std::floor(x)!=x)
        throw std::runtime_error("expected integer 0..4294967295");
    return static_cast<uint32_t>(x);
}
std::wstring wide(const std::string& s) {
    if(s.empty()) return L"";
    int n=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),nullptr,0);
    if(!n) throw std::runtime_error("invalid UTF-8");
    std::wstring w(n,L'\0');
    if(!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),&w[0],n))
        throw std::runtime_error("UTF-8 conversion failed");
    return w;
}
std::string utf8(const std::wstring& w) {
    if(w.empty()) return "";
    int n=WideCharToMultiByte(CP_UTF8,0,w.data(),static_cast<int>(w.size()),nullptr,0,nullptr,nullptr);
    if(!n) throw std::runtime_error("Unicode conversion failed");
    std::string s(n,'\0');
    WideCharToMultiByte(CP_UTF8,0,w.data(),static_cast<int>(w.size()),&s[0],n,nullptr,nullptr);
    return s;
}
void noNul(const std::string& s) {
    if(s.find('\0')!=std::string::npos) throw std::runtime_error("binary NUL unsupported by native ABI");
}
struct InternetHandle {
    HINTERNET h=nullptr;
    explicit InternetHandle(HINTERNET x=nullptr):h(x){}
    ~InternetHandle(){ if(h) WinHttpCloseHandle(h); }
    InternetHandle(const InternetHandle&)=delete;
    InternetHandle& operator=(const InternetHandle&)=delete;
};
struct RequestResult { unsigned status; std::string body; std::string contentType; };
RequestResult request(const std::string& method,const std::string& url,
                      const std::string& body,const std::string& contentType) {
    if(url.size()>8192) throw std::runtime_error("URL too long");
    std::wstring wurl=wide(url);
    URL_COMPONENTS u={};
    u.dwStructSize=sizeof(u);
    u.dwSchemeLength=static_cast<DWORD>(-1);
    u.dwHostNameLength=static_cast<DWORD>(-1);
    u.dwUrlPathLength=static_cast<DWORD>(-1);
    u.dwExtraInfoLength=static_cast<DWORD>(-1);
    if(!WinHttpCrackUrl(wurl.c_str(),0,0,&u)) throw std::runtime_error("invalid URL");
    if(u.nScheme!=INTERNET_SCHEME_HTTP && u.nScheme!=INTERNET_SCHEME_HTTPS)
        throw std::runtime_error("only http and https are supported");
    std::wstring host(u.lpszHostName,u.dwHostNameLength);
    std::wstring path;
    if(u.lpszUrlPath) path.assign(u.lpszUrlPath,u.dwUrlPathLength);
    if(u.lpszExtraInfo) path.append(u.lpszExtraInfo,u.dwExtraInfoLength);
    if(path.empty()) path=L"/";
    InternetHandle session(WinHttpOpen(L"FluxScript/1.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                      WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0));
    if(!session.h) throw std::runtime_error("WinHttpOpen failed");
    WinHttpSetTimeouts(session.h,5000,5000,10000,10000);
    InternetHandle connection(WinHttpConnect(session.h,host.c_str(),u.nPort,0));
    if(!connection.h) throw std::runtime_error("WinHttpConnect failed");
    DWORD flags=(u.nScheme==INTERNET_SCHEME_HTTPS)?WINHTTP_FLAG_SECURE:0;
    InternetHandle req(WinHttpOpenRequest(connection.h,wide(method).c_str(),path.c_str(),
                            nullptr,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,flags));
    if(!req.h) throw std::runtime_error("WinHttpOpenRequest failed");
    std::wstring headers;
    if(!contentType.empty()) headers=L"Content-Type: "+wide(contentType)+L"\r\n";
    if(body.size()>1048576) throw std::runtime_error("request body exceeds 1 MiB");
    LPVOID payload=body.empty()?WINHTTP_NO_REQUEST_DATA:const_cast<char*>(body.data());
    if(!WinHttpSendRequest(req.h,headers.empty()?WINHTTP_NO_ADDITIONAL_HEADERS:headers.c_str(),
                           headers.empty()?0:static_cast<DWORD>(-1),
                           payload,static_cast<DWORD>(body.size()),static_cast<DWORD>(body.size()),0))
        throw std::runtime_error("WinHttpSendRequest failed: "+std::to_string(GetLastError()));
    if(!WinHttpReceiveResponse(req.h,nullptr))
        throw std::runtime_error("WinHttpReceiveResponse failed: "+std::to_string(GetLastError()));
    DWORD status=0, size=sizeof(status);
    if(!WinHttpQueryHeaders(req.h,WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER,
                            WINHTTP_HEADER_NAME_BY_INDEX,&status,&size,WINHTTP_NO_HEADER_INDEX))
        throw std::runtime_error("HTTP status unavailable");
    std::string result;
    char chunk[8192];
    while(true) {
        DWORD got=0;
        if(!WinHttpReadData(req.h,chunk,sizeof(chunk),&got))
            throw std::runtime_error("WinHttpReadData failed");
        if(!got) break;
        if(result.size()+got>4*1024*1024) throw std::runtime_error("response exceeds 4 MiB");
        result.append(chunk,got);
    }
    noNul(result);
    return {status,result,""};
}
std::string urlEncode(const std::string& s) {
    static const char* hex="0123456789ABCDEF";
    std::string o;
    for(unsigned char c:s) {
        if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='-'||c=='_'||c=='.'||c=='~')
            o+=char(c);
        else { o+='%'; o+=hex[c>>4]; o+=hex[c&15]; }
    }
    return o;
}
int nibble(char c) {
    if(c>='0'&&c<='9') return c-'0';
    if(c>='a'&&c<='f') return c-'a'+10;
    if(c>='A'&&c<='F') return c-'A'+10;
    return -1;
}
std::string urlDecode(const std::string& s) {
    std::string o;
    for(size_t i=0;i<s.size();++i) {
        if(s[i]=='%') {
            if(i+2>=s.size()||nibble(s[i+1])<0||nibble(s[i+2])<0)
                throw std::runtime_error("invalid percent escape");
            o+=char((nibble(s[i+1])<<4)|nibble(s[i+2])); i+=2;
        } else o+=s[i];
    }
    noNul(o);
    return o;
}
struct Wsa {
    Wsa(){ WSADATA data; if(WSAStartup(MAKEWORD(2,2),&data)!=0) throw std::runtime_error("WSAStartup failed"); }
    ~Wsa(){ WSACleanup(); }
};
std::string dns(const std::string& host) {
    Wsa wsa;
    addrinfo hints={};
    hints.ai_family=AF_INET;
    addrinfo* result=nullptr;
    if(getaddrinfo(host.c_str(),nullptr,&hints,&result)!=0) throw std::runtime_error("DNS lookup failed");
    std::string out;
    for(addrinfo* p=result;p;p=p->ai_next) {
        char buf[INET_ADDRSTRLEN]={};
        auto* a=reinterpret_cast<sockaddr_in*>(p->ai_addr);
        if(inet_ntop(AF_INET,&a->sin_addr,buf,sizeof(buf))) {
            if(!out.empty()) out+=',';
            out+=buf;
        }
    }
    freeaddrinfo(result);
    if(out.empty()) throw std::runtime_error("no IPv4 address found");
    return out;
}
std::string hostName() {
    Wsa wsa;
    char name[256]={};
    if(gethostname(name,sizeof(name))!=0) throw std::runtime_error("gethostname failed");
    return name;
}
uint32_t ipv4Number(const std::string& ip) {
    Wsa wsa;
    in_addr addr={};
    if(inet_pton(AF_INET,ip.c_str(),&addr)!=1) throw std::runtime_error("invalid IPv4 address");
    return ntohl(addr.s_addr);
}
std::string ipv4String(uint32_t n) {
    Wsa wsa;
    in_addr addr={};
    addr.s_addr=htonl(n);
    char buf[INET_ADDRSTRLEN]={};
    if(!inet_ntop(AF_INET,&addr,buf,sizeof(buf))) throw std::runtime_error("IPv4 conversion failed");
    return buf;
}
bool tcpReachable(const std::string& host,uint32_t port,uint32_t timeout) {
    if(port<1||port>65535) throw std::runtime_error("port must be 1..65535");
    if(timeout<1||timeout>30000) throw std::runtime_error("timeout must be 1..30000 ms");
    Wsa wsa;
    addrinfo hints={}; hints.ai_family=AF_UNSPEC; hints.ai_socktype=SOCK_STREAM;
    addrinfo* result=nullptr;
    std::string service=std::to_string(port);
    if(getaddrinfo(host.c_str(),service.c_str(),&hints,&result)!=0) return false;
    bool ok=false;
    for(addrinfo* p=result;p;p=p->ai_next) {
        SOCKET sock=socket(p->ai_family,p->ai_socktype,p->ai_protocol);
        if(sock==INVALID_SOCKET) continue;
        u_long nonblocking=1;
        ioctlsocket(sock,FIONBIO,&nonblocking);
        int r=connect(sock,p->ai_addr,static_cast<int>(p->ai_addrlen));
        if(r==0) ok=true;
        else if(WSAGetLastError()==WSAEWOULDBLOCK) {
            fd_set writes,errors;
            FD_ZERO(&writes); FD_ZERO(&errors);
            FD_SET(sock,&writes); FD_SET(sock,&errors);
            timeval tv={static_cast<long>(timeout/1000),static_cast<long>((timeout%1000)*1000)};
            int selected=select(0,nullptr,&writes,&errors,&tv);
            if(selected>0 && FD_ISSET(sock,&writes)) {
                int err=0; int len=sizeof(err);
                if(getsockopt(sock,SOL_SOCKET,SO_ERROR,reinterpret_cast<char*>(&err),&len)==0 && err==0)
                    ok=true;
            }
        }
        closesocket(sock);
        if(ok) break;
    }
    freeaddrinfo(result);
    return ok;
}
template<typename F> const char* invoke(F f) {
    try {
        output=f();
        if(output.find('\0')!=std::string::npos) throw std::runtime_error("NUL in result");
    } catch(const std::exception& e) { output="E:"+std::string(e.what()); }
    catch(...) { output="E:unknown native error"; }
    return output.c_str();
}
std::string S(const std::string& v){ noNul(v); return "S:"+v; }
std::string N(uint64_t v){ return "N:"+std::to_string(v); }
}
EXPORT int __cdecl flux_abi_version(){ return 2; }
EXPORT const char* __cdecl flux_httpGet(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);return S(request("GET",str(a[0]),"","").body);});
}
EXPORT const char* __cdecl flux_httpPost(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,3);return S(request("POST",str(a[0]),str(a[1]),str(a[2])).body);});
}
EXPORT const char* __cdecl flux_httpStatus(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);return N(request("GET",str(a[0]),"","").status);});
}
EXPORT const char* __cdecl flux_httpPostStatus(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,3);return N(request("POST",str(a[0]),str(a[1]),str(a[2])).status);});
}
EXPORT const char* __cdecl flux_urlEncode(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);return S(urlEncode(str(a[0])));});
}
EXPORT const char* __cdecl flux_urlDecode(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);return S(urlDecode(str(a[0])));});
}
EXPORT const char* __cdecl flux_dnsLookup(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);return S(dns(str(a[0])));});
}
EXPORT const char* __cdecl flux_hostname(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,0);return S(hostName());});
}
EXPORT const char* __cdecl flux_ipv4ToInt(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);return N(ipv4Number(str(a[0])));});
}
EXPORT const char* __cdecl flux_intToIpv4(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);return S(ipv4String(integer(a[0])));});
}
EXPORT const char* __cdecl flux_isIpv4(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,1);try{ipv4Number(str(a[0]));return N(1);}catch(...){return N(0);}});
}
EXPORT const char* __cdecl flux_tcpReachable(const char* raw) {
    return invoke([&]{auto a=parse(raw);count(a,3);return N(tcpReachable(str(a[0]),integer(a[1]),integer(a[2]))?1:0);});
}
