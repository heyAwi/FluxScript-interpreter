
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
        if (*p && *p != ',')
            throw std::runtime_error("expected comma separator");
        values.push_back(value);
        if (!*p) break;
        ++p;
        if (!*p) throw std::runtime_error("trailing comma");
    }
    if (values.size() != expected)
        throw std::runtime_error("wrong number of arguments");
    return values;
}

const char* calculate(const char* raw, std::size_t count, Operation op) noexcept {
    try {
        Args a = parse(raw, count);
        double value = op(a);
        if (!std::isfinite(value))
            throw std::runtime_error("non-finite result");
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

double nonzero(double n) {
    if (n == 0.0) throw std::runtime_error("division by zero");
    return n;
}

double nonnegative(double n) {
    if (n < 0.0) throw std::runtime_error("negative size or radius");
    return n;
}

double sq(double n) { return n * n; }
double cross(double ax, double ay, double bx, double by) {
    return ax * by - ay * bx;
}
double orientation(double ax, double ay, double bx, double by, double cx, double cy) {
    return cross(bx - ax, by - ay, cx - ax, cy - ay);
}
bool onSegment(double ax, double ay, double bx, double by, double px, double py) {
    return px >= std::min(ax, bx) && px <= std::max(ax, bx)
        && py >= std::min(ay, by) && py <= std::max(ay, by);
}
bool segmentsIntersect(
    double ax, double ay, double bx, double by,
    double cx, double cy, double dx, double dy
) {
    double o1 = orientation(ax, ay, bx, by, cx, cy);
    double o2 = orientation(ax, ay, bx, by, dx, dy);
    double o3 = orientation(cx, cy, dx, dy, ax, ay);
    double o4 = orientation(cx, cy, dx, dy, bx, by);
    if (((o1 > 0 && o2 < 0) || (o1 < 0 && o2 > 0)) &&
        ((o3 > 0 && o4 < 0) || (o3 < 0 && o4 > 0)))
        return true;
    if (o1 == 0 && onSegment(ax, ay, bx, by, cx, cy)) return true;
    if (o2 == 0 && onSegment(ax, ay, bx, by, dx, dy)) return true;
    if (o3 == 0 && onSegment(cx, cy, dx, dy, ax, ay)) return true;
    if (o4 == 0 && onSegment(cx, cy, dx, dy, bx, by)) return true;
    return false;
}

// Original physics functions

double op_velocity(const Args& a) { return a[0] / nonzero(a[1]); }
double op_acceleration(const Args& a) { return (a[1] - a[0]) / nonzero(a[2]); }
double op_force(const Args& a) { return a[0] * a[1]; }
double op_gravity(const Args& a) { return a[0] * a[1]; }
double op_kineticEnergy(const Args& a) { return 0.5 * a[0] * sq(a[1]); }
double op_potentialEnergy(const Args& a) { return a[0] * a[1] * a[2]; }
double op_momentum(const Args& a) { return a[0] * a[1]; }
double op_distance(const Args& a) { return a[0] * a[1] + 0.5 * a[2] * sq(a[1]); }
double op_friction(const Args& a) { return std::abs(a[0]) * a[1]; }
double op_bounce(const Args& a) { return -a[0] * a[1]; }
double op_lerp(const Args& a) { return a[0] + (a[1] - a[0]) * a[2]; }
double op_distance2D(const Args& a) {
    return std::hypot(a[2] - a[0], a[3] - a[1]);
}
double op_angle2D(const Args& a) {
    constexpr double pi = 3.14159265358979323846;
    return std::atan2(a[3] - a[1], a[2] - a[0]) * 180.0 / pi;
}
double op_moveX(const Args& a) { return a[0] + a[1] * a[2]; }
double op_moveY(const Args& a) { return a[0] + a[1] * a[2]; }

// Collision functions

double op_rectRect(const Args& a) {
    nonnegative(a[2]); nonnegative(a[3]);
    nonnegative(a[6]); nonnegative(a[7]);
    return a[0] <= a[4] + a[6] && a[0] + a[2] >= a[4]
        && a[1] <= a[5] + a[7] && a[1] + a[3] >= a[5];
}

double op_circleCircle(const Args& a) {
    nonnegative(a[2]); nonnegative(a[5]);
    return std::hypot(a[3] - a[0], a[4] - a[1]) <= a[2] + a[5];
}

double op_rectCircle(const Args& a) {
    nonnegative(a[2]); nonnegative(a[3]); nonnegative(a[6]);
    double px = std::clamp(a[4], a[0], a[0] + a[2]);
    double py = std::clamp(a[5], a[1], a[1] + a[3]);
    return std::hypot(a[4] - px, a[5] - py) <= a[6];
}

double op_pointRect(const Args& a) {
    nonnegative(a[4]); nonnegative(a[5]);
    return a[0] >= a[2] && a[0] <= a[2] + a[4]
        && a[1] >= a[3] && a[1] <= a[3] + a[5];
}

double op_pointCircle(const Args& a) {
    nonnegative(a[4]);
    return std::hypot(a[0] - a[2], a[1] - a[3]) <= a[4];
}

double op_lineLine(const Args& a) {
    return segmentsIntersect(
        a[0], a[1], a[2], a[3],
        a[4], a[5], a[6], a[7]
    );
}
double op_collisionDistance(const Args& a) {
    return std::hypot(a[2] - a[0], a[3] - a[1]);
}
double op_overlapX(const Args& a) {
    nonnegative(a[1]); nonnegative(a[3]);
    return std::max(0.0, std::min(a[0] + a[1], a[2] + a[3])
        - std::max(a[0], a[2]));
}
double op_overlapY(const Args& a) {
    nonnegative(a[1]); nonnegative(a[3]);
    return std::max(0.0, std::min(a[0] + a[1], a[2] + a[3])
        - std::max(a[0], a[2]));
}
double op_raycast2D(const Args& a) {
    nonnegative(a[6]);
    double ox = a[0] - a[4];
    double oy = a[1] - a[5];
    double dx = a[2];
    double dy = a[3];
    double aa = dx * dx + dy * dy;
    if (aa == 0.0)
        throw std::runtime_error("zero ray direction");
    double bb = 2.0 * (ox * dx + oy * dy);
    double cc = ox * ox + oy * oy - a[6] * a[6];
    if (cc <= 0.0) return 0.0;
    double disc = bb * bb - 4.0 * aa * cc;
    if (disc < 0.0) return -1.0;
    double root = std::sqrt(disc);
    double t = (-bb - root) / (2.0 * aa);
    return t >= 0.0 ? t : -1.0;
}
}

FLUX_EXPORT const char* flux_velocity(const char* s) { return calculate(s, 2, op_velocity); }
FLUX_EXPORT const char* flux_acceleration(const char* s) { return calculate(s, 3, op_acceleration); }
FLUX_EXPORT const char* flux_force(const char* s) { return calculate(s, 2, op_force); }
FLUX_EXPORT const char* flux_gravity(const char* s) { return calculate(s, 2, op_gravity); }
FLUX_EXPORT const char* flux_kineticEnergy(const char* s) { return calculate(s, 2, op_kineticEnergy); }
FLUX_EXPORT const char* flux_potentialEnergy(const char* s) { return calculate(s, 3, op_potentialEnergy); }
FLUX_EXPORT const char* flux_momentum(const char* s) { return calculate(s, 2, op_momentum); }
FLUX_EXPORT const char* flux_distance(const char* s) { return calculate(s, 3, op_distance); }
FLUX_EXPORT const char* flux_friction(const char* s) { return calculate(s, 2, op_friction); }
FLUX_EXPORT const char* flux_bounce(const char* s) { return calculate(s, 2, op_bounce); }
FLUX_EXPORT const char* flux_lerp(const char* s) { return calculate(s, 3, op_lerp); }
FLUX_EXPORT const char* flux_distance2D(const char* s) { return calculate(s, 4, op_distance2D); }
FLUX_EXPORT const char* flux_angle2D(const char* s) { return calculate(s, 4, op_angle2D); }
FLUX_EXPORT const char* flux_moveX(const char* s) { return calculate(s, 3, op_moveX); }
FLUX_EXPORT const char* flux_moveY(const char* s) { return calculate(s, 3, op_moveY); }
FLUX_EXPORT const char* flux_rectRect(const char* s) { return calculate(s, 8, op_rectRect); }
FLUX_EXPORT const char* flux_circleCircle(const char* s) { return calculate(s, 6, op_circleCircle); }
FLUX_EXPORT const char* flux_rectCircle(const char* s) { return calculate(s, 7, op_rectCircle); }
FLUX_EXPORT const char* flux_pointRect(const char* s) { return calculate(s, 6, op_pointRect); }
FLUX_EXPORT const char* flux_pointCircle(const char* s) { return calculate(s, 5, op_pointCircle); }
FLUX_EXPORT const char* flux_lineLine(const char* s) { return calculate(s, 8, op_lineLine); }
FLUX_EXPORT const char* flux_collisionDistance(const char* s) { return calculate(s, 4, op_collisionDistance); }
FLUX_EXPORT const char* flux_overlapX(const char* s) { return calculate(s, 4, op_overlapX); }
FLUX_EXPORT const char* flux_overlapY(const char* s) { return calculate(s, 4, op_overlapY); }
FLUX_EXPORT const char* flux_raycast2D(const char* s) { return calculate(s, 7, op_raycast2D); }
