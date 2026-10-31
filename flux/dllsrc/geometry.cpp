#include <cmath>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#ifdef _WIN32
#define FLUX_EXPORT extern "C" __declspec(dllexport)
#else
#define FLUX_EXPORT extern "C"
#endif
namespace {
thread_local std::string result;
using Args = std::vector<double>;
using Operation = double (*)(const Args&);
constexpr double PI = 3.14159265358979323846264338327950288;
Args parse(const char* raw, std::size_t expected) {
    if (!raw) throw std::runtime_error("null argument string");
    Args values;
    const char* p = raw;
    while (true) {
        while (*p == ' ' || *p == '\t') ++p;
        if (!*p) break;
        errno = 0;
        char* end = nullptr;
        double value = std::strtod(p, &end);
        if (end == p || errno == ERANGE || !std::isfinite(value))
            throw std::runtime_error("invalid finite number");
        p = end;
        while (*p == ' ' || *p == '\t') ++p;
        if (*p && *p != ',') throw std::runtime_error("expected comma separator");
        values.push_back(value);
        if (!*p) break;
        ++p;
        if (!*p) throw std::runtime_error("trailing comma");
    }
    if (values.size() != expected) throw std::runtime_error("wrong number of arguments");
    return values;
}
const char* calculate(const char* raw, std::size_t count, Operation op) noexcept {
    try {
        double value = op(parse(raw, count));
        if (!std::isfinite(value)) throw std::runtime_error("non-finite result");
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "%.17g", value);
        result = buffer;
    } catch (const std::exception& e) {
        result = std::string("error: ") + e.what();
    } catch (...) {
        result = "error: unknown native failure";
    }
    return result.c_str();
}
double nonnegative(double x) {
    if (x < 0) throw std::runtime_error("negative length or radius");
    return x;
}
double sq(double x) { return x * x; }
double op_circleArea(const Args& a) { return PI * sq(nonnegative(a[0])); }
double op_circleCircumference(const Args& a) { return 2 * PI * nonnegative(a[0]); }
double op_rectangleArea(const Args& a) { return nonnegative(a[0]) * nonnegative(a[1]); }
double op_triangleArea(const Args& a) { return nonnegative(a[0]) * nonnegative(a[1]) / 2; }
double op_sphereVolume(const Args& a) { double r = nonnegative(a[0]); return 4.0 / 3.0 * PI * r * r * r; }
double op_sphereArea(const Args& a) { return 4 * PI * sq(nonnegative(a[0])); }
double op_boxVolume(const Args& a) { return nonnegative(a[0]) * nonnegative(a[1]) * nonnegative(a[2]); }
double op_distance3D(const Args& a) { return std::hypot(std::hypot(a[3]-a[0], a[4]-a[1]), a[5]-a[2]); }
double op_dot2D(const Args& a) { return a[0]*a[2] + a[1]*a[3]; }
double op_dot3D(const Args& a) { return a[0]*a[3] + a[1]*a[4] + a[2]*a[5]; }
double op_cross2D(const Args& a) { return a[0]*a[3] - a[1]*a[2]; }
double op_cross3DX(const Args& a) { return a[1]*a[5] - a[2]*a[4]; }
double op_cross3DY(const Args& a) { return a[2]*a[3] - a[0]*a[5]; }
double op_cross3DZ(const Args& a) { return a[0]*a[4] - a[1]*a[3]; }
double op_rotate2DX(const Args& a) { double t = a[2] * PI / 180.0; return a[0]*std::cos(t) - a[1]*std::sin(t); }
double op_rotate2DY(const Args& a) { double t = a[2] * PI / 180.0; return a[0]*std::sin(t) + a[1]*std::cos(t); }
}
FLUX_EXPORT const char* flux_circleArea(const char* s) { return calculate(s, 1, op_circleArea); }
FLUX_EXPORT const char* flux_circleCircumference(const char* s) { return calculate(s, 1, op_circleCircumference); }
FLUX_EXPORT const char* flux_rectangleArea(const char* s) { return calculate(s, 2, op_rectangleArea); }
FLUX_EXPORT const char* flux_triangleArea(const char* s) { return calculate(s, 2, op_triangleArea); }
FLUX_EXPORT const char* flux_sphereVolume(const char* s) { return calculate(s, 1, op_sphereVolume); }
FLUX_EXPORT const char* flux_sphereArea(const char* s) { return calculate(s, 1, op_sphereArea); }
FLUX_EXPORT const char* flux_boxVolume(const char* s) { return calculate(s, 3, op_boxVolume); }
FLUX_EXPORT const char* flux_distance3D(const char* s) { return calculate(s, 6, op_distance3D); }
FLUX_EXPORT const char* flux_dot2D(const char* s) { return calculate(s, 4, op_dot2D); }
FLUX_EXPORT const char* flux_dot3D(const char* s) { return calculate(s, 6, op_dot3D); }
FLUX_EXPORT const char* flux_cross2D(const char* s) { return calculate(s, 4, op_cross2D); }
FLUX_EXPORT const char* flux_cross3DX(const char* s) { return calculate(s, 6, op_cross3DX); }
FLUX_EXPORT const char* flux_cross3DY(const char* s) { return calculate(s, 6, op_cross3DY); }
FLUX_EXPORT const char* flux_cross3DZ(const char* s) { return calculate(s, 6, op_cross3DZ); }
FLUX_EXPORT const char* flux_rotate2DX(const char* s) { return calculate(s, 3, op_rotate2DX); }
FLUX_EXPORT const char* flux_rotate2DY(const char* s) { return calculate(s, 3, op_rotate2DY); }
