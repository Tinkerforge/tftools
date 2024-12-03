/* TFTools
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

#include <type_traits>

#include "./Tools.h"

template<typename T>
struct Option {
public:
    template<typename U = std::is_trivially_copy_constructible<T>, typename std::enable_if<U::value>::type...>
    Option(T t): val(t), have_val(true) {}
               // val has to be initialized if it is a primitive type.
    Option() : val(), have_val(false) {}

    T &unwrap() {
        return expect("unwrapped Option without value!");
    }

    const T &unwrap() const {
        return expect("unwrapped Option without value!");
    }

    bool try_unwrap(T *target) {
        if (this->is_none())
            return false;

        *target = this->unwrap();
        return true;
    }

    bool try_unwrap(T *target) const {
        if (this->is_none())
            return false;

        *target = this->unwrap();
        return true;
    }

    T &unwrap_or(T &default_value) {
        if (!have_val)
            return default_value;
        return val;
    }

    const T &unwrap_or(const T &default_value) const {
        if (!have_val)
            return default_value;
        return val;
    }

    T &expect(const char *message) {
        if (!have_val)
            system_abort(message);
        return val;
    }

    const T &expect(const char *message) const {
        if (!have_val)
            system_abort(message);
        return val;
    }

    T &insert(const T &default_value) {
        val = default_value;
        have_val = true;
        return val;
    }

    bool is_some() const {
        return have_val;
    }

    bool is_none() const {
        return !have_val;
    }

    void clear() {
        have_val = false;
    }

private:
    T val;
    bool have_val;
};
