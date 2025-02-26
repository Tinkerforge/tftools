/* TFTools
 * Copyright (C) 2020-2021 Erik Fleckstein <erik@tinkerforge.com>
 * Copyright (C) 2024 Matthias Bolte <matthias@tinkerforge.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#pragma once

#include <stdint.h>
#include <type_traits>

#include "./StrongTypedef.h"

#define SINGLE_ARG(...) __VA_ARGS__

STRONG_INTEGER_TYPEDEF(int64_t, micros_t,
    // Necessary to be able to use ',' in the class implementation
    SINGLE_ARG(
    template <typename T> constexpr inline typename std::enable_if<std::is_fundamental<T>::value, T>::type as() const { return (T) t;}
    template <typename T> constexpr inline T to() const { return T{t / static_cast<micros_t>(T{T{1ll}.t}).t};}
    )
)

STRONG_INTEGER_TYPEDEF(int64_t, millis_t,
    SINGLE_ARG(
    constexpr inline operator micros_t () const { return micros_t{t * 1000}; }
    template <typename T> constexpr inline typename std::enable_if<std::is_fundamental<T>::value, T>::type as() const { return (T) t;}
    template <typename T> constexpr inline T to() const { return T{t / static_cast<millis_t>(T{T{1ll}.t}).t};}
    )
)

STRONG_INTEGER_TYPEDEF(int64_t, seconds_t,
    SINGLE_ARG(
    constexpr inline operator micros_t () const { return micros_t{t * 1000 * 1000}; }
    constexpr inline operator millis_t () const { return millis_t{t * 1000}; }
    template <typename T> constexpr inline typename std::enable_if<std::is_fundamental<T>::value, T>::type as() const { return (T) t;}
    template <typename T> constexpr inline T to() const { return T{t / static_cast<seconds_t>(T{T{1ll}.t}).t};}
    )
)

STRONG_INTEGER_TYPEDEF(int64_t, minutes_t,
    SINGLE_ARG(
    constexpr inline operator micros_t  () const { return micros_t{ t * 60 * 1000 * 1000}; }
    constexpr inline operator millis_t  () const { return millis_t{ t * 60 * 1000}; }
    constexpr inline operator seconds_t () const { return seconds_t{t * 60}; }
    template <typename T> constexpr inline typename std::enable_if<std::is_fundamental<T>::value, T>::type as() const { return (T) t;}
    template <typename T> constexpr inline T to() const { return T{t / static_cast<minutes_t>(T{T{1ll}.t}).t};}
    )
)

STRONG_INTEGER_TYPEDEF(int64_t, hours_t,
    SINGLE_ARG(
    constexpr inline operator micros_t  () const { return micros_t{ t * 60 * 60 * 1000 * 1000}; }
    constexpr inline operator millis_t  () const { return millis_t{ t * 60 * 60 * 1000}; }
    constexpr inline operator seconds_t () const { return seconds_t{t * 60 * 60}; }
    constexpr inline operator minutes_t () const { return minutes_t{t * 60}; }
    template <typename T> constexpr inline typename std::enable_if<std::is_fundamental<T>::value, T>::type as() const { return (T) t;}
    template <typename T> constexpr inline T to() const { return T{t / static_cast<hours_t>(T{T{1ll}.t}).t};}
    )
)

// These do not clash with the C++14 standard literals for durations:
// https://en.cppreference.com/w/cpp/chrono/duration
// because those don't start with a _
// "ud-suffix must begin with the underscore _:
// the suffixes that do not begin with the underscore
// are reserved for the literal operators provided by the standard library."
// Param type has to be unsigned long long, because:
// "if the overload set includes a literal operator with the parameter type unsigned long long,
// the user-defined literal expression is treated as a function call operator ""X(n ULL),
// where n is the literal without ud-suffix;"
constexpr micros_t operator""_us   (unsigned long long int i) { return micros_t{(int64_t)i}; }
constexpr millis_t operator""_ms   (unsigned long long int i) { return millis_t{(int64_t)i}; }
constexpr seconds_t operator""_s   (unsigned long long int i) { return seconds_t{(int64_t)i}; }
// _min would be nicer but confuses vscode
// because Arduino.h defines _min to not
// collide with std::min in case someone uses "using namespace std;" m(
constexpr minutes_t operator""_m   (unsigned long long int i) { return minutes_t{(int64_t)i}; }
constexpr hours_t operator""_h     (unsigned long long int i) { return hours_t{(int64_t)i}; }

micros_t now_us();
bool deadline_elapsed(micros_t deadline_us);
micros_t calculate_deadline(micros_t delay_us);
