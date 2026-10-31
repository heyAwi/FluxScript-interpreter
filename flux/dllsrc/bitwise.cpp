
#include <cstdint>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#ifdef _WIN32
#define FLUX_EXPORT extern "C" __declspec(dllexport)
#else
#define FLUX_EXPORT extern "C"
#endif
namespace {
thread_local std::string result;
using U32 = std::uint32_t;
std::vector<U32> parse(const char* raw, std::size_t expected) {
    if (!raw) throw std::runtime_error("null argument string");
    std::vector<U32> values;
    const char* p = raw;
    while (true) {
        while (*p == ' ' || *p == '\t') ++p;
        if (!*p) break;
        if (*p == '-') throw std::runtime_error("expected unsigned 32-bit integer");
        if (*p == '+') ++p;
        if (!std::isdigit(static_cast<unsigned char>(*p)))
            throw std::runtime_error("expected unsigned 32-bit integer");
        errno = 0;
        char* end = nullptr;
        unsigned long long v = std::strtoull(p, &end, 10);
        if (errno == ERANGE || v > UINT32_MAX)
            throw std::runtime_error("integer outside 32-bit range");
        p = end;
        while (*p == ' ' || *p == '\t') ++p;
        if (*p && *p != ',') throw std::runtime_error("expected comma separator");
        values.push_back(static_cast<U32>(v));
        if (!*p) break;
        ++p;
        if (!*p) throw std::runtime_error("trailing comma");
    }
    if (values.size() != expected) throw std::runtime_error("wrong number of arguments");
    return values;
}
using Operation = U32 (*)(const std::vector<U32>&);
const char* calculate(const char* args, std::size_t count, Operation op) noexcept {
    try {
        auto values = parse(args, count);
        U32 v = op(values);
        result = std::to_string(v);
    } catch (const std::exception& e) {
        result = std::string("error: ") + e.what();
    } catch (...) {
        result = "error: unknown native failure";
    }
    return result.c_str();
}
unsigned shift(U32 n) {
    if (n > 31) throw std::runtime_error("shift must be between 0 and 31");
    return static_cast<unsigned>(n);
}
U32 op_band(const std::vector<U32>& a) { return a[0] & a[1]; }
U32 op_bor(const std::vector<U32>& a) { return a[0] | a[1]; }
U32 op_bxor(const std::vector<U32>& a) { return a[0] ^ a[1]; }
U32 op_bnot(const std::vector<U32>& a) { return ~a[0]; }
U32 op_shl(const std::vector<U32>& a) { return static_cast<U32>(a[0] << shift(a[1])); }
U32 op_shr(const std::vector<U32>& a) { return a[0] >> shift(a[1]); }
U32 op_rol(const std::vector<U32>& a) {
    unsigned n = shift(a[1]);
    return n == 0 ? a[0] : static_cast<U32>((a[0] << n) | (a[0] >> (32 - n)));
}
U32 op_ror(const std::vector<U32>& a) {
    unsigned n = shift(a[1]);
    return n == 0 ? a[0] : static_cast<U32>((a[0] >> n) | (a[0] << (32 - n)));
}
U32 op_popcount(const std::vector<U32>& a) {
    U32 n = a[0], count = 0;
    while (n) { n &= n - 1; ++count; }
    return count;
}
U32 op_bitGet(const std::vector<U32>& a) { return (a[0] >> shift(a[1])) & 1u; }
U32 op_bitSet(const std::vector<U32>& a) { return a[0] | (U32(1) << shift(a[1])); }
U32 op_bitClear(const std::vector<U32>& a) { return a[0] & ~(U32(1) << shift(a[1])); }
}
FLUX_EXPORT const char* flux_band(const char* s) { return calculate(s, 2, op_band); }
FLUX_EXPORT const char* flux_bor(const char* s) { return calculate(s, 2, op_bor); }
FLUX_EXPORT const char* flux_bxor(const char* s) { return calculate(s, 2, op_bxor); }
FLUX_EXPORT const char* flux_bnot(const char* s) { return calculate(s, 1, op_bnot); }
FLUX_EXPORT const char* flux_shl(const char* s) { return calculate(s, 2, op_shl); }
FLUX_EXPORT const char* flux_shr(const char* s) { return calculate(s, 2, op_shr); }
FLUX_EXPORT const char* flux_rol(const char* s) { return calculate(s, 2, op_rol); }
FLUX_EXPORT const char* flux_ror(const char* s) { return calculate(s, 2, op_ror); }
FLUX_EXPORT const char* flux_popcount(const char* s) { return calculate(s, 1, op_popcount); }
FLUX_EXPORT const char* flux_bitGet(const char* s) { return calculate(s, 2, op_bitGet); }
FLUX_EXPORT const char* flux_bitSet(const char* s) { return calculate(s, 2, op_bitSet); }
FLUX_EXPORT const char* flux_bitClear(const char* s) { return calculate(s, 2, op_bitClear); }
