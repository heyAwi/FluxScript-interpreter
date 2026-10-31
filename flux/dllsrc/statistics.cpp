
#include <algorithm>
#include <cmath>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#define FLUX_EXPORT extern "C" __declspec(dllexport)
#else
#define FLUX_EXPORT extern "C"
#endif

namespace {
using Args = std::vector<double>;
using Operation = double (*)(const Args&);
thread_local std::string result;

Args parse(const char* raw) {
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

        values.push_back(value);
        p = end;

        while (*p == ' ' || *p == '\t') ++p;
        if (!*p) break;
        if (*p != ',')
            throw std::runtime_error("expected comma separator");

        ++p;
        while (*p == ' ' || *p == '\t') ++p;
        if (!*p)
            throw std::runtime_error("trailing comma");
    }

    return values;
}

void require(const Args& a, std::size_t minCount) {
    if (a.size() < minCount)
        throw std::runtime_error("not enough arguments");
}

void exact(const Args& a, std::size_t count) {
    if (a.size() != count)
        throw std::runtime_error("wrong number of arguments");
}

unsigned integer(double value, unsigned maxValue = 170) {
    if (!std::isfinite(value) || value < 0 ||
        value > maxValue || std::floor(value) != value)
        throw std::runtime_error("invalid nonnegative integer");
    return static_cast<unsigned>(value);
}

const char* calculate(const char* raw, Operation op) noexcept {
    try {
        Args a = parse(raw);
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

double op_sum(const Args& a) {
    require(a, 1);
    return std::accumulate(a.begin(), a.end(), 0.0);
}

double op_product(const Args& a) {
    require(a, 1);
    return std::accumulate(a.begin(), a.end(), 1.0,
        [](double x, double y) { return x * y; });
}

double op_mean(const Args& a) {
    require(a, 1);
    return op_sum(a) / static_cast<double>(a.size());
}

double op_median(const Args& a) {
    require(a, 1);
    Args sorted = a;
    std::sort(sorted.begin(), sorted.end());

    std::size_t n = sorted.size();
    if (n % 2 != 0)
        return sorted[n / 2];

    return sorted[n / 2 - 1] / 2.0 + sorted[n / 2] / 2.0;
}

double op_mode(const Args& a) {
    require(a, 1);
    Args sorted = a;
    std::sort(sorted.begin(), sorted.end());

    double best = sorted[0];
    std::size_t bestCount = 0;

    for (std::size_t i = 0; i < sorted.size();) {
        std::size_t j = i + 1;
        while (j < sorted.size() && sorted[j] == sorted[i])
            ++j;

        std::size_t count = j - i;
        if (count > bestCount) {
            bestCount = count;
            best = sorted[i];
        }
        i = j;
    }

    return best;
}

double op_variance(const Args& a) {
    require(a, 1);
    double mean = op_mean(a);
    double total = 0.0;

    for (double value : a) {
        double diff = value - mean;
        total += diff * diff;
    }

    return total / static_cast<double>(a.size());
}

double op_stddev(const Args& a) {
    return std::sqrt(op_variance(a));
}

double op_min(const Args& a) {
    require(a, 1);
    return *std::min_element(a.begin(), a.end());
}

double op_max(const Args& a) {
    require(a, 1);
    return *std::max_element(a.begin(), a.end());
}

double op_range(const Args& a) {
    return op_max(a) - op_min(a);
}

double op_percentile(const Args& a) {
    require(a, 2);

    double percent = a.back();
    if (percent < 0.0 || percent > 100.0)
        throw std::runtime_error("percentile must be between 0 and 100");

    Args sorted(a.begin(), a.end() - 1);
    std::sort(sorted.begin(), sorted.end());

    double index = (percent / 100.0) * (sorted.size() - 1);
    std::size_t lower = static_cast<std::size_t>(std::floor(index));
    std::size_t upper = static_cast<std::size_t>(std::ceil(index));

    if (lower == upper)
        return sorted[lower];

    double fraction = index - lower;
    return sorted[lower] +
        (sorted[upper] - sorted[lower]) * fraction;
}

double op_factorial(const Args& a) {
    exact(a, 1);
    unsigned n = integer(a[0]);
    double value = 1.0;

    for (unsigned i = 2; i <= n; ++i)
        value *= i;

    return value;
}

double op_combination(const Args& a) {
    exact(a, 2);
    unsigned n = integer(a[0]);
    unsigned r = integer(a[1]);

    if (r > n)
        throw std::runtime_error("r cannot exceed n");

    r = std::min(r, n - r);
    double value = 1.0;

    for (unsigned i = 1; i <= r; ++i)
        value = value * (n - r + i) / i;

    return std::round(value);
}

double op_permutation(const Args& a) {
    exact(a, 2);
    unsigned n = integer(a[0]);
    unsigned r = integer(a[1]);

    if (r > n)
        throw std::runtime_error("r cannot exceed n");

    double value = 1.0;
    for (unsigned i = 0; i < r; ++i)
        value *= n - i;

    return value;
}

double op_geometricMean(const Args& a) {
    require(a, 1);
    double total = 0.0;

    for (double value : a) {
        if (value < 0.0)
            throw std::runtime_error("geometric mean requires nonnegative values");
        if (value == 0.0)
            return 0.0;
        total += std::log(value);
    }

    return std::exp(total / static_cast<double>(a.size()));
}
}

FLUX_EXPORT const char* flux_sum(const char* s) {
    return calculate(s, op_sum);
}
FLUX_EXPORT const char* flux_product(const char* s) {
    return calculate(s, op_product);
}
FLUX_EXPORT const char* flux_mean(const char* s) {
    return calculate(s, op_mean);
}
FLUX_EXPORT const char* flux_median(const char* s) {
    return calculate(s, op_median);
}
FLUX_EXPORT const char* flux_mode(const char* s) {
    return calculate(s, op_mode);
}
FLUX_EXPORT const char* flux_variance(const char* s) {
    return calculate(s, op_variance);
}
FLUX_EXPORT const char* flux_stddev(const char* s) {
    return calculate(s, op_stddev);
}
FLUX_EXPORT const char* flux_min(const char* s) {
    return calculate(s, op_min);
}
FLUX_EXPORT const char* flux_max(const char* s) {
    return calculate(s, op_max);
}
FLUX_EXPORT const char* flux_range(const char* s) {
    return calculate(s, op_range);
}
FLUX_EXPORT const char* flux_percentile(const char* s) {
    return calculate(s, op_percentile);
}
FLUX_EXPORT const char* flux_factorial(const char* s) {
    return calculate(s, op_factorial);
}
FLUX_EXPORT const char* flux_combination(const char* s) {
    return calculate(s, op_combination);
}
FLUX_EXPORT const char* flux_permutation(const char* s) {
    return calculate(s, op_permutation);
}
FLUX_EXPORT const char* flux_geometricMean(const char* s) {
    return calculate(s, op_geometricMean);
}
