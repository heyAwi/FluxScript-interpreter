#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <bcrypt.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#pragma comment(lib, "bcrypt.lib")
#define EXPORT extern "C" __declspec(dllexport)
namespace {
struct Arg { char type; std::string data; };
thread_local std::string output;
size_t readSize(const std::string& s, size_t& p, char delimiter) {
    if (p >= s.size() || s[p] < '0' || s[p] > '9') throw std::runtime_error("invalid length");
    size_t n=0;
    while(p<s.size() && s[p]>='0' && s[p]<='9') {
        unsigned d=unsigned(s[p++]-'0');
        if(n>(std::numeric_limits<size_t>::max()-d)/10) throw std::runtime_error("length overflow");
        n=n*10+d;
    }
    if(p>=s.size() || s[p++]!=delimiter) throw std::runtime_error("invalid delimiter");
    return n;
}
std::vector<Arg> parse(const char* raw) {
    if(!raw) throw std::runtime_error("null input");
    std::string s(raw);
    if(s.compare(0,3,"F2|")!=0) throw std::runtime_error("ABI v2 required");
    size_t p=3, count=readSize(s,p,'|');
    if(count>32) throw std::runtime_error("too many arguments");
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
    if(p!=s.size()) throw std::runtime_error("trailing argument bytes");
    return a;
}
void count(const std::vector<Arg>& a,size_t n) { if(a.size()!=n) throw std::runtime_error("wrong number of arguments"); }
std::string str(const Arg& a) { if(a.type!='S') throw std::runtime_error("expected string"); return a.data; }
uint64_t integer(const Arg& a) {
    if(a.type!='N') throw std::runtime_error("expected number");
    char* end=nullptr;
    double x=std::strtod(a.data.c_str(),&end);
    if(end==a.data.c_str() || *end || !std::isfinite(x) || x<0 || x>4294967295.0 || std::floor(x)!=x)
        throw std::runtime_error("expected unsigned integer 0..4294967295");
    return static_cast<uint64_t>(x);
}
std::vector<unsigned char> randomData(size_t n) {
    if(n>1048576) throw std::runtime_error("random request too large");
    std::vector<unsigned char> bytes(n);
    if(n && BCryptGenRandom(nullptr,bytes.data(),static_cast<ULONG>(n),BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0)
        throw std::runtime_error("secure random generator failed");
    return bytes;
}
std::string hex(const unsigned char* b,size_t n) {
    const char* digits="0123456789abcdef";
    std::string out; out.reserve(n*2);
    for(size_t i=0;i<n;++i){out+=digits[b[i]>>4];out+=digits[b[i]&15];}
    return out;
}
int nibble(char c) {
    if(c>='0'&&c<='9')return c-'0';
    if(c>='a'&&c<='f')return c-'a'+10;
    if(c>='A'&&c<='F')return c-'A'+10;
    throw std::runtime_error("invalid hexadecimal string");
}
std::string unhex(const std::string& s) {
    if(s.size()%2)throw std::runtime_error("hex string must have even length");
    std::string out;out.reserve(s.size()/2);
    for(size_t i=0;i<s.size();i+=2)out+=char((nibble(s[i])<<4)|nibble(s[i+1]));
    return out;
}
std::string b64(const std::string& s) {
    const char* abc="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    for(size_t i=0;i<s.size();i+=3) {
        unsigned a=static_cast<unsigned char>(s[i]);
        unsigned b=i+1<s.size()?static_cast<unsigned char>(s[i+1]):0;
        unsigned c=i+2<s.size()?static_cast<unsigned char>(s[i+2]):0;
        out+=abc[a>>2];out+=abc[((a&3)<<4)|(b>>4)];
        out+=i+1<s.size()?abc[((b&15)<<2)|(c>>6)]:'=';
        out+=i+2<s.size()?abc[c&63]:'=';
    }
    return out;
}
std::string unb64(const std::string& s) {
    if(s.size()%4)throw std::runtime_error("invalid base64 length");
    std::string out;
    const std::string abc="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    for(size_t i=0;i<s.size();i+=4) {
        bool last=i+4==s.size();
        bool pad2=s[i+2]=='=',pad3=s[i+3]=='=';
        if(!last && (pad2||pad3))throw std::runtime_error("invalid base64 padding");
        if(pad2 && !pad3)throw std::runtime_error("invalid base64 padding");
        auto val=[&](char c)->unsigned {size_t k=abc.find(c);if(k==std::string::npos)throw std::runtime_error("invalid base64 character");return unsigned(k);};
        unsigned a=val(s[i]),b=val(s[i+1]),c=pad2?0:val(s[i+2]),d=pad3?0:val(s[i+3]);
        if((pad2&&(b&15))||(pad3&&!pad2&&(c&3)))throw std::runtime_error("invalid base64 padding bits");
        out+=char((a<<2)|(b>>4));
        if(!pad2)out+=char((b<<4)|(c>>2));
        if(!pad3)out+=char((c<<6)|d);
    }
    return out;
}
struct Alg {
    BCRYPT_ALG_HANDLE h=nullptr;
    explicit Alg(LPCWSTR name, bool hmac=false) {
        if(BCryptOpenAlgorithmProvider(&h,name,nullptr,hmac?BCRYPT_ALG_HANDLE_HMAC_FLAG:0)<0)
            throw std::runtime_error("cannot open hash algorithm");
    }
    ~Alg(){if(h)BCryptCloseAlgorithmProvider(h,0);}
};
std::string digest(LPCWSTR name,const std::string& msg,const std::string* key=nullptr) {
    if(msg.size()>UINT32_MAX || (key&&key->size()>UINT32_MAX))throw std::runtime_error("input too large");
    Alg alg(name,key!=nullptr);
    DWORD bytes=0, size=0;
    if(BCryptGetProperty(alg.h,BCRYPT_HASH_LENGTH,reinterpret_cast<PUCHAR>(&size),sizeof(size),&bytes,0)<0 || size==0)
        throw std::runtime_error("cannot determine digest size");
    BCRYPT_HASH_HANDLE hash=nullptr;
    PUCHAR secret=key?reinterpret_cast<PUCHAR>(const_cast<char*>(key->data())):nullptr;
    ULONG secretSize=key?static_cast<ULONG>(key->size()):0;
    if(BCryptCreateHash(alg.h,&hash,nullptr,0,secret,secretSize,0)<0)throw std::runtime_error("cannot create hash");
    std::vector<unsigned char> out(size);
    NTSTATUS status=BCryptHashData(hash,reinterpret_cast<PUCHAR>(const_cast<char*>(msg.data())),static_cast<ULONG>(msg.size()),0);
    if(status>=0)status=BCryptFinishHash(hash,out.data(),size,0);
    BCryptDestroyHash(hash);
    if(status<0)throw std::runtime_error("hash failed");
    return hex(out.data(),out.size());
}
const char* respond(const char* raw, const std::string& name) noexcept {
    try {
        auto a=parse(raw);
        std::string r;
        if(name=="sha256"||name=="sha512"||name=="sha1"||name=="md5") {
            count(a,1);
            LPCWSTR algorithm=name=="sha256"?BCRYPT_SHA256_ALGORITHM:name=="sha512"?BCRYPT_SHA512_ALGORITHM:name=="sha1"?BCRYPT_SHA1_ALGORITHM:BCRYPT_MD5_ALGORITHM;
            r=digest(algorithm,str(a[0]));
        } else if(name=="hmacSha256"||name=="hmacSha512") {
            count(a,2);std::string key=str(a[0]),msg=str(a[1]);
            r=digest(name=="hmacSha256"?BCRYPT_SHA256_ALGORITHM:BCRYPT_SHA512_ALGORITHM,msg,&key);
        } else if(name=="randomHex") {
            count(a,1);size_t n=static_cast<size_t>(integer(a[0]));
            auto bytes=randomData(n);r=hex(bytes.data(),bytes.size());
        } else if(name=="randomInt") {
            count(a,1);uint64_t max=integer(a[0]);
            if(!max)throw std::runtime_error("maximum must be positive");
            uint64_t range=max+1, limit=(uint64_t(1)<<32)/range*range;
            uint32_t x;
            do {auto bytes=randomData(4);x=uint32_t(bytes[0])|(uint32_t(bytes[1])<<8)|(uint32_t(bytes[2])<<16)|(uint32_t(bytes[3])<<24);} while(uint64_t(x)>=limit);
            output="N:"+std::to_string(uint64_t(x)%range);return output.c_str();
        } else if(name=="hexEncode") {
            count(a,1);std::string t=str(a[0]);r=hex(reinterpret_cast<const unsigned char*>(t.data()),t.size());
        } else if(name=="hexDecode") {
            count(a,1);r=unhex(str(a[0]));
        } else if(name=="base64Encode") {
            count(a,1);r=b64(str(a[0]));
        } else if(name=="base64Decode") {
            count(a,1);r=unb64(str(a[0]));
        } else if(name=="constantTimeEqual") {
            count(a,2);std::string x=str(a[0]),y=str(a[1]);
            size_t n=std::max(x.size(),y.size());
            unsigned diff=unsigned(x.size()!=y.size());
            for(size_t i=0;i<n;++i) {
                unsigned char c=i<x.size()?static_cast<unsigned char>(x[i]):0;
                unsigned char d=i<y.size()?static_cast<unsigned char>(y[i]):0;
                diff|=c^d;
            }
            output=diff?"N:0":"N:1";return output.c_str();
        } else throw std::runtime_error("unknown function");
        // Current C-string return ABI cannot transport embedded NUL bytes.
        if(r.find('\0')!=std::string::npos)throw std::runtime_error("decoded data contains NUL byte; use encoded representation");
        output="S:"+r;
    } catch(const std::exception& e){output="E:"+std::string(e.what());}
    catch(...){output="E:unknown native error";}
    return output.c_str();
}
}
EXPORT int __cdecl flux_abi_version(){return 2;}
#define FLUX_FN(name) EXPORT const char* __cdecl flux_##name(const char* raw){return respond(raw,#name);}
FLUX_FN(sha256)
FLUX_FN(sha512)
FLUX_FN(sha1)
FLUX_FN(md5)
FLUX_FN(hmacSha256)
FLUX_FN(hmacSha512)
FLUX_FN(randomHex)
FLUX_FN(randomInt)
FLUX_FN(hexEncode)
FLUX_FN(hexDecode)
FLUX_FN(base64Encode)
FLUX_FN(base64Decode)
FLUX_FN(constantTimeEqual)
