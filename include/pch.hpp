/*
 * MIT License
 *
 * Copyright (c) 2026 DerBallo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <bit>
#include <cmath>
#include <concepts>
#include <deque>
#include <format>
#include <fstream>
#include <memory>
#include <print>
#include <stdint.h>
#include <string.h>
#include <tuple>
#include <vector>

#include <vulkan/vulkan_core.h>

constexpr int8_t operator""_i8(unsigned long long value) noexcept { return static_cast<int8_t>(value); }
constexpr uint8_t operator""_u8(unsigned long long value) noexcept { return static_cast<uint8_t>(value); }
constexpr int16_t operator""_i16(unsigned long long value) noexcept { return static_cast<int16_t>(value); }
constexpr uint16_t operator""_u16(unsigned long long value) noexcept { return static_cast<uint16_t>(value); }
constexpr int32_t operator""_i32(unsigned long long value) noexcept { return static_cast<int32_t>(value); }
constexpr uint32_t operator""_u32(unsigned long long value) noexcept { return static_cast<uint32_t>(value); }
constexpr int64_t operator""_i64(unsigned long long value) noexcept { return static_cast<int64_t>(value); }
constexpr uint64_t operator""_u64(unsigned long long value) noexcept { return static_cast<uint64_t>(value); }

constexpr float operator""_f(long double value) noexcept { return static_cast<float>(value); }
constexpr float operator""_f(unsigned long long value) noexcept { return static_cast<float>(value); }
constexpr double operator""_d(long double value) noexcept { return static_cast<double>(value); }
constexpr double operator""_d(unsigned long long value) noexcept { return static_cast<double>(value); }
constexpr long double operator""_ld(long double value) noexcept { return static_cast<long double>(value); }
constexpr long double operator""_ld(unsigned long long value) noexcept { return static_cast<long double>(value); }

using byte_t = std::byte;

constexpr size_t operator""_size(unsigned long long value) noexcept { return static_cast<size_t>(value); }
constexpr ptrdiff_t operator""_ptrdiff(unsigned long long value) noexcept { return static_cast<ptrdiff_t>(value); }
constexpr byte_t operator""_b(unsigned long long value) noexcept { return static_cast<byte_t>(value); }

using std::construct_at;
using std::destroy_at;

template <typename T, size_t N> constexpr size_t lengthof(const T (&)[N]) noexcept
{
    return N;
};

template <typename T>
constexpr std::remove_reference_t<T>* addressof(T&& _) noexcept
{
    return std::addressof(_);
}

#define ANSI_WARN "\033[1;33m"
#define ANSI_ERRR "\033[1;31m"
#define ANSI_SCCS "\033[1;32m"
#define ANSI_MESG "\033[1;35m"
#define ANSI_RSET "\033[0m"
