
#include <cmath>
#include <cerrno>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstdint>
#include <string>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
#define FLUX_EXPORT extern "C" __declspec(dllexport)
#else
#define FLUX_EXPORT extern "C"
#endif

namespace {
using Args = std::vector<double>;
using Operation = double (*)(const Args&);
thread_local std::string result;

constexpr double MAX_TIMESTAMP = 253402300799.0;
constexpr double SECONDS_PER_DAY = 86400.0;

Args parse(const char* raw, std::size_t expected) {
    if (!raw)
        throw std::runtime_error("null argument string");

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

        while (*p == ' ' || *p == '\t') ++p;
        if (!*p)
            throw std::runtime_error("trailing comma");
    }

    if (values.size() != expected)
        throw std::runtime_error("wrong number of arguments");

    return values;
}

int integer(double value, int minValue, int maxValue) {
    if (!std::isfinite(value) ||
        std::floor(value) != value ||
        value < minValue || value > maxValue)
        throw std::runtime_error("invalid integer or out of range");

    return static_cast<int>(value);
}

double checkedTimestamp(double value) {
    if (!std::isfinite(value) ||
        std::floor(value) != value ||
        value < 0.0 || value > MAX_TIMESTAMP)
        throw std::runtime_error("timestamp outside supported range");

    return value;
}

std::tm utcTime(double timestamp) {
    std::time_t raw = static_cast<std::time_t>(
        checkedTimestamp(timestamp)
    );

    std::tm output = {};

#ifdef _WIN32
    if (gmtime_s(&output, &raw) != 0)
        throw std::runtime_error("UTC conversion failed");
#else
    if (gmtime_r(&raw, &output) == nullptr)
        throw std::runtime_error("UTC conversion failed");
#endif

    return output;
}

bool leapYear(int year) {
    return (year % 4 == 0) &&
           (year % 100 != 0 || year % 400 == 0);
}

int monthDays(int year, int month) {
    static const int days[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    int value = days[month - 1];

    if (month == 2 && leapYear(year))
        ++value;

    return value;
}

double createTimestamp(
    int year, int month, int day,
    int hour, int minute, int second
) {
    if (day > monthDays(year, month))
        throw std::runtime_error("invalid day for month");

    std::tm date = {};
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;
    date.tm_hour = hour;
    date.tm_min = minute;
    date.tm_sec = second;
    date.tm_isdst = 0;

#ifdef _WIN32
    std::time_t value = _mkgmtime(&date);
#else
    std::time_t value = timegm(&date);
#endif

    if (value == static_cast<std::time_t>(-1))
        throw std::runtime_error("timestamp conversion failed");

    double timestamp = static_cast<double>(value);
    checkedTimestamp(timestamp);

    std::tm verified = utcTime(timestamp);

    if (verified.tm_year + 1900 != year ||
        verified.tm_mon + 1 != month ||
        verified.tm_mday != day ||
        verified.tm_hour != hour ||
        verified.tm_min != minute ||
        verified.tm_sec != second)
        throw std::runtime_error("date conversion mismatch");

    return timestamp;
}

const char* calculate(
    const char* raw,
    std::size_t count,
    Operation op
) noexcept {
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

// Current Unix timestamp in UTC
double op_now(const Args&) {
    std::time_t current = std::time(nullptr);

    if (current == static_cast<std::time_t>(-1))
        throw std::runtime_error("system time unavailable");

    return checkedTimestamp(static_cast<double>(current));
}

// Convert UTC date and time into Unix timestamp
double op_timestamp(const Args& a) {
    int year = integer(a[0], 1970, 9999);
    int month = integer(a[1], 1, 12);
    int day = integer(a[2], 1, 31);
    int hour = integer(a[3], 0, 23);
    int minute = integer(a[4], 0, 59);
    int second = integer(a[5], 0, 59);

    return createTimestamp(
        year, month, day, hour, minute, second
    );
}

// Extract UTC date and time components
double op_year(const Args& a) {
    return utcTime(a[0]).tm_year + 1900;
}

double op_month(const Args& a) {
    return utcTime(a[0]).tm_mon + 1;
}

double op_day(const Args& a) {
    return utcTime(a[0]).tm_mday;
}

double op_hour(const Args& a) {
    return utcTime(a[0]).tm_hour;
}

double op_minute(const Args& a) {
    return utcTime(a[0]).tm_min;
}

double op_second(const Args& a) {
    return utcTime(a[0]).tm_sec;
}

// Sunday = 0, Monday = 1, ..., Saturday = 6
double op_weekday(const Args& a) {
    return utcTime(a[0]).tm_wday;
}

// January 1 = 1
double op_dayOfYear(const Args& a) {
    return utcTime(a[0]).tm_yday + 1;
}

double op_isLeapYear(const Args& a) {
    int year = integer(a[0], 1970, 9999);
    return leapYear(year) ? 1.0 : 0.0;
}

double op_daysInMonth(const Args& a) {
    int year = integer(a[0], 1970, 9999);
    int month = integer(a[1], 1, 12);
    return monthDays(year, month);
}

// Add a number of UTC days
double op_addDays(const Args& a) {
    double timestamp = checkedTimestamp(a[0]);
    double days = a[1];

    if (std::floor(days) != days)
        throw std::runtime_error("days must be an integer");

    double output = timestamp + days * SECONDS_PER_DAY;
    return checkedTimestamp(output);
}

// Difference in days: second timestamp minus first
double op_diffDays(const Args& a) {
    double first = checkedTimestamp(a[0]);
    double second = checkedTimestamp(a[1]);

    return (second - first) / SECONDS_PER_DAY;
}

// Add a number of seconds
double op_addSeconds(const Args& a) {
    double timestamp = checkedTimestamp(a[0]);
    double seconds = a[1];

    if (std::floor(seconds) != seconds)
        throw std::runtime_error("seconds must be an integer");

    return checkedTimestamp(timestamp + seconds);
}
}

// Exported functions

FLUX_EXPORT const char* flux_now(const char* s) {
    return calculate(s, 0, op_now);
}

FLUX_EXPORT const char* flux_timestamp(const char* s) {
    return calculate(s, 6, op_timestamp);
}

FLUX_EXPORT const char* flux_year(const char* s) {
    return calculate(s, 1, op_year);
}

FLUX_EXPORT const char* flux_month(const char* s) {
    return calculate(s, 1, op_month);
}

FLUX_EXPORT const char* flux_day(const char* s) {
    return calculate(s, 1, op_day);
}

FLUX_EXPORT const char* flux_hour(const char* s) {
    return calculate(s, 1, op_hour);
}

FLUX_EXPORT const char* flux_minute(const char* s) {
    return calculate(s, 1, op_minute);
}

FLUX_EXPORT const char* flux_second(const char* s) {
    return calculate(s, 1, op_second);
}

FLUX_EXPORT const char* flux_weekday(const char* s) {
    return calculate(s, 1, op_weekday);
}

FLUX_EXPORT const char* flux_dayOfYear(const char* s) {
    return calculate(s, 1, op_dayOfYear);
}

FLUX_EXPORT const char* flux_isLeapYear(const char* s) {
    return calculate(s, 1, op_isLeapYear);
}

FLUX_EXPORT const char* flux_daysInMonth(const char* s) {
    return calculate(s, 2, op_daysInMonth);
}

FLUX_EXPORT const char* flux_addDays(const char* s) {
    return calculate(s, 2, op_addDays);
}

FLUX_EXPORT const char* flux_diffDays(const char* s) {
    return calculate(s, 2, op_diffDays);
}

FLUX_EXPORT const char* flux_addSeconds(const char* s) {
    return calculate(s, 2, op_addSeconds);
}
