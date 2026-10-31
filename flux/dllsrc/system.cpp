#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <Lmcons.h>
#include <cmath>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>

#define FLUX_EXPORT extern "C" __declspec(dllexport)

namespace {
using Args = std::vector<double>;
using Operation = double (*)(const Args&);
thread_local std::string result;

Args parse(const char* raw, size_t expected) {
    if (!raw) throw std::runtime_error("null arguments");
    Args values;
    const char* p = raw;
    while (true) {
        while (*p == ' ' || *p == '\t') ++p;
        if (!*p) break;
        errno = 0;
        char* end = nullptr;
        double value = std::strtod(p, &end);
        if (end == p || errno == ERANGE || !std::isfinite(value))
            throw std::runtime_error("invalid numeric argument");
        values.push_back(value);
        p = end;
        while (*p == ' ' || *p == '\t') ++p;
        if (!*p) break;
        if (*p != ',') throw std::runtime_error("expected comma");
        ++p;
        while (*p == ' ' || *p == '\t') ++p;
        if (!*p) throw std::runtime_error("trailing comma");
    }
    if (values.size() != expected) throw std::runtime_error("wrong number of arguments");
    return values;
}

const char* numberResult(const char* raw, size_t count, Operation op) noexcept {
    try {
        const double value = op(parse(raw, count));
        if (!std::isfinite(value)) throw std::runtime_error("non-finite result");
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "%.17g", value);
        result = std::string("N:") + buffer;
    } catch (const std::exception& e) {
        result = std::string("E:") + e.what();
    } catch (...) {
        result = "E:unknown native failure";
    }
    return result.c_str();
}

using TextOperation = std::string (*)(const Args&);
const char* textResult(const char* raw, size_t count, TextOperation op) noexcept {
    try {
        result = std::string("S:") + op(parse(raw, count));
    } catch (const std::exception& e) {
        result = std::string("E:") + e.what();
    } catch (...) {
        result = "E:unknown native failure";
    }
    return result.c_str();
}

MEMORYSTATUSEX memoryInfo() {
    MEMORYSTATUSEX info = {};
    info.dwLength = sizeof(info);
    if (!GlobalMemoryStatusEx(&info)) throw std::runtime_error("memory information unavailable");
    return info;
}
SYSTEM_INFO systemInfo() {
    SYSTEM_INFO info = {};
    GetSystemInfo(&info);
    return info;
}
double mib(ULONGLONG bytes) {
    return static_cast<double>(bytes) / 1048576.0;
}
struct DiskInfo { ULONGLONG total; ULONGLONG free; };
DiskInfo diskInfo() {
    ULARGE_INTEGER available = {}, total = {}, freeTotal = {};
    if (!GetDiskFreeSpaceExA(nullptr, &available, &total, &freeTotal))
        throw std::runtime_error("disk information unavailable");
    return {total.QuadPart, freeTotal.QuadPart};
}
double fileTimeSeconds(const FILETIME& value) {
    ULARGE_INTEGER time = {};
    time.LowPart = value.dwLowDateTime;
    time.HighPart = value.dwHighDateTime;
    return static_cast<double>(time.QuadPart) / 10000000.0;
}
std::string wideToUtf8(const std::wstring& wide) {
    if (wide.empty()) return "";
    if (wide.size() > static_cast<size_t>(std::numeric_limits<int>::max()))
        throw std::runtime_error("text too long");
    int len = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide.data(),
        static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
    if (!len) throw std::runtime_error("UTF-8 conversion failed");
    std::string output(static_cast<size_t>(len), '\0');
    if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wide.data(),
        static_cast<int>(wide.size()), &output[0], len, nullptr, nullptr))
        throw std::runtime_error("UTF-8 conversion failed");
    return output;
}

// Numeric operations
double op_cpuCount(const Args&) { return systemInfo().dwNumberOfProcessors; }
double op_memoryTotalMB(const Args&) { return mib(memoryInfo().ullTotalPhys); }
double op_memoryFreeMB(const Args&) { return mib(memoryInfo().ullAvailPhys); }
double op_memoryLoad(const Args&) { return memoryInfo().dwMemoryLoad; }
double op_uptimeSeconds(const Args&) { return static_cast<double>(GetTickCount64()) / 1000.0; }
double op_processId(const Args&) { return GetCurrentProcessId(); }
double op_threadId(const Args&) { return GetCurrentThreadId(); }
double op_pageSize(const Args&) { return systemInfo().dwPageSize; }
double op_pointerBits(const Args&) { return sizeof(void*) * 8; }
double op_screenWidth(const Args&) { return GetSystemMetrics(SM_CXSCREEN); }
double op_screenHeight(const Args&) { return GetSystemMetrics(SM_CYSCREEN); }
double op_diskTotalMB(const Args&) { return mib(diskInfo().total); }
double op_diskFreeMB(const Args&) { return mib(diskInfo().free); }
double op_processCpuSeconds(const Args&) {
    FILETIME created = {}, exited = {}, kernel = {}, user = {};
    if (!GetProcessTimes(GetCurrentProcess(), &created, &exited, &kernel, &user))
        throw std::runtime_error("process CPU time unavailable");
    return fileTimeSeconds(kernel) + fileTimeSeconds(user);
}
double op_sleepMs(const Args& args) {
    const double ms = args[0];
    if (std::floor(ms) != ms || ms < 0 || ms > 60000)
        throw std::runtime_error("sleepMs expects integer milliseconds from 0 to 60000");
    Sleep(static_cast<DWORD>(ms));
    return 0;
}

// String operations (UTF-8)
std::string op_hostname(const Args&) {
    DWORD capacity = MAX_COMPUTERNAME_LENGTH + 1;
    wchar_t name[MAX_COMPUTERNAME_LENGTH + 1] = {};
    if (!GetComputerNameW(name, &capacity))
        throw std::runtime_error("computer name unavailable");
    return wideToUtf8(std::wstring(name, capacity));
}
std::string op_username(const Args&) {
    DWORD capacity = UNLEN + 1;
    wchar_t name[UNLEN + 1] = {};
    if (!GetUserNameW(name, &capacity))
        throw std::runtime_error("user name unavailable");
    return wideToUtf8(std::wstring(name));
}
std::string op_osName(const Args&) { return "Windows"; }
std::string op_architecture(const Args&) {
    SYSTEM_INFO info = {};
    GetNativeSystemInfo(&info);
    switch (info.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64: return "x64";
        case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
        case PROCESSOR_ARCHITECTURE_ARM: return "ARM";
        case PROCESSOR_ARCHITECTURE_IA64: return "IA64";
        default: return "unknown";
    }
}
std::string op_windowsDirectory(const Args&) {
    wchar_t path[MAX_PATH + 1] = {};
    UINT length = GetWindowsDirectoryW(path, MAX_PATH + 1);
    if (!length || length > MAX_PATH)
        throw std::runtime_error("Windows directory unavailable");
    return wideToUtf8(std::wstring(path, length));
}
}

#define FLUX_NUMBER(name, count) \
FLUX_EXPORT const char* flux_##name(const char* s) { return numberResult(s, count, op_##name); }
#define FLUX_TEXT(name, count) \
FLUX_EXPORT const char* flux_##name(const char* s) { return textResult(s, count, op_##name); }

FLUX_NUMBER(cpuCount, 0)
FLUX_NUMBER(memoryTotalMB, 0)
FLUX_NUMBER(memoryFreeMB, 0)
FLUX_NUMBER(memoryLoad, 0)
FLUX_NUMBER(uptimeSeconds, 0)
FLUX_NUMBER(processId, 0)
FLUX_NUMBER(threadId, 0)
FLUX_NUMBER(pageSize, 0)
FLUX_NUMBER(pointerBits, 0)
FLUX_NUMBER(screenWidth, 0)
FLUX_NUMBER(screenHeight, 0)
FLUX_NUMBER(diskTotalMB, 0)
FLUX_NUMBER(diskFreeMB, 0)
FLUX_NUMBER(processCpuSeconds, 0)
FLUX_NUMBER(sleepMs, 1)
FLUX_TEXT(hostname, 0)
FLUX_TEXT(username, 0)
FLUX_TEXT(osName, 0)
FLUX_TEXT(architecture, 0)
FLUX_TEXT(windowsDirectory, 0)
