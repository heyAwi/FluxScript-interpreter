#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>
#ifdef _WIN32
#define FLUX_EXPORT extern "C" __declspec(dllexport)
#else
#define FLUX_EXPORT extern "C" __attribute__((visibility("default")))
#endif
namespace {
thread_local std::string result;
std::vector<double> parse(const char* raw, std::size_t expected) {
    if (!raw) throw std::runtime_error("null argument string");
    std::vector<double> values;
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
const char* format(double value) {
    if (!std::isfinite(value)) throw std::runtime_error("non-finite result");
    char buffer[64];
    std::snprintf(buffer, sizeof(buffer), "%.17g", value);
    result = buffer;
    return result.c_str();
}
template<typename F>
const char* calculate(const char* args, std::size_t count, F fn) noexcept {
    try { return format(fn(parse(args, count))); }
    catch (const std::exception& e) { result = std::string("error: ") + e.what(); return result.c_str(); }
    catch (...) { result = "error: unknown native failure"; return result.c_str(); }
}
double requireRange(double value, double lo, double hi, const char* label) {
    if (value < lo || value > hi) throw std::runtime_error(std::string(label) + " out of range");
    return value;
}
double requirePositive(double value, const char* label) {
    if (value <= 0) throw std::runtime_error(std::string(label) + " must be positive");
    return value;
}
std::mt19937_64& rng() {
    static thread_local std::mt19937_64 engine(std::random_device{}());
    return engine;
}
double unitRandom() {
    return std::generate_canonical<double, 53>(rng());
}
}
FLUX_EXPORT const char* flux_clamp(const char* s) {
    return calculate(s, 3, [](const std::vector<double>& a) {
        if (a[1] > a[2]) throw std::runtime_error("min exceeds max");
        return std::max(a[1], std::min(a[0], a[2]));
    });
}
FLUX_EXPORT const char* flux_normalize(const char* s) {
    return calculate(s, 3, [](const std::vector<double>& a) {
        if (a[1] >= a[2]) throw std::runtime_error("max must exceed min");
        return (a[0] - a[1]) / (a[2] - a[1]);
    });
}
FLUX_EXPORT const char* flux_health(const char* s) {
    return calculate(s, 2, [](const std::vector<double>& a) {
        return std::max(0.0, a[0] - std::max(0.0, a[1]));
    });
}
FLUX_EXPORT const char* flux_heal(const char* s) {
    return calculate(s, 3, [](const std::vector<double>& a) {
        if (a[2] < 0) throw std::runtime_error("max health must be nonnegative");
        return std::min(a[2], std::max(0.0, a[0]) + std::max(0.0, a[1]));
    });
}
FLUX_EXPORT const char* flux_isDead(const char* s) {
    return calculate(s, 1, [](const std::vector<double>& a) { return a[0] <= 0 ? 1.0 : 0.0; });
}
FLUX_EXPORT const char* flux_score(const char* s) {
    return calculate(s, 2, [](const std::vector<double>& a) { return a[0] + a[1]; });
}
FLUX_EXPORT const char* flux_level(const char* s) {
    return calculate(s, 2, [](const std::vector<double>& a) {
        requirePositive(a[1], "xpPerLevel");
        return std::floor(std::max(0.0, a[0]) / a[1]) + 1.0;
    });
}
FLUX_EXPORT const char* flux_xpRequired(const char* s) {
    return calculate(s, 2, [](const std::vector<double>& a) {
        requirePositive(a[0], "level");
        requirePositive(a[1], "baseXP");
        return a[0] * a[1];
    });
}
FLUX_EXPORT const char* flux_cooldown(const char* s) {
    return calculate(s, 2, [](const std::vector<double>& a) {
        if (a[1] < 0) throw std::runtime_error("duration must be nonnegative");
        return a[0] >= a[1] ? 1.0 : 0.0;
    });
}
FLUX_EXPORT const char* flux_chance(const char* s) {
    return calculate(s, 1, [](const std::vector<double>& a) {
        requireRange(a[0], 0.0, 1.0, "probability");
        return unitRandom() < a[0] ? 1.0 : 0.0;
    });
}
FLUX_EXPORT const char* flux_randomRange(const char* s) {
    return calculate(s, 2, [](const std::vector<double>& a) {
        if (a[0] > a[1]) throw std::runtime_error("min exceeds max");
        if (a[0] == a[1]) return a[0];
        return a[0] + (a[1] - a[0]) * unitRandom();
    });
}
FLUX_EXPORT const char* flux_moveTowards(const char* s) {
    return calculate(s, 3, [](const std::vector<double>& a) {
        if (a[2] < 0) throw std::runtime_error("maxStep must be nonnegative");
        const double delta = a[1] - a[0];
        if (std::abs(delta) <= a[2]) return a[1];
        return a[0] + std::copysign(a[2], delta);
    });
}
FLUX_EXPORT const char* flux_wrap(const char* s) {
    return calculate(s, 3, [](const std::vector<double>& a) {
        if (a[1] >= a[2]) throw std::runtime_error("max must exceed min");
        const double width = a[2] - a[1];
        double value = std::fmod(a[0] - a[1], width);
        if (value < 0) value += width;
        return a[1] + value;
    });
}
FLUX_EXPORT const char* flux_smoothstep(const char* s) {
    return calculate(s, 3, [](const std::vector<double>& a) {
        if (a[0] >= a[1]) throw std::runtime_error("end must exceed start");
        const double t = std::max(0.0, std::min(1.0, (a[2] - a[0]) / (a[1] - a[0])));
        return t * t * (3.0 - 2.0 * t);
    });
}
FLUX_EXPORT const char* flux_damage(const char* s) {
    return calculate(s, 2, [](const std::vector<double>& a) {
        return std::max(0.0, a[0] - a[1]);
    });
}
