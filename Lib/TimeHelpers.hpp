#pragma once

#include <cstdint>
#include <chrono>
#include <type_traits>

namespace Time {

using Nanos = std::chrono::duration<std::uint64_t, std::nano>;
using NanosF = std::chrono::duration<long double, std::nano>;
using Micros = std::chrono::duration<std::uint64_t, std::micro>;
using MicrosF = std::chrono::duration<long double, std::micro>;
using Millis = std::chrono::duration<std::uint64_t, std::milli>;
using MillisF = std::chrono::duration<double, std::milli>;
using Seconds = std::chrono::duration<std::uint64_t>;
using SecondsF = std::chrono::duration<double>;
using Minutes = std::chrono::duration<std::uint64_t, std::ratio<60, 1>>;
using MinutesF = std::chrono::duration<double, std::ratio<60, 1>>;
using Hours = std::chrono::duration<std::uint64_t, std::ratio<60 * 60, 1>>;
using HoursF = std::chrono::duration<double, std::ratio<60 * 60, 1>>;
using Days = std::chrono::duration<std::uint64_t, std::ratio<60 * 60 * 24, 1>>;
using DaysF = std::chrono::duration<double, std::ratio<60 * 60 * 24, 1>>;

template<typename T>
struct IsDuration : public std::false_type {};

template<typename Rep, typename Period>
struct IsDuration<std::chrono::duration<Rep, Period>> : public std::true_type {};

template<typename T>
using IsDurationVal = typename IsDuration<T>::value;

}