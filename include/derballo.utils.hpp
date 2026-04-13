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

#include "derballo.format.vkresult.hpp"

#define hidden_ve_stringify(x) #x
#define ve_stringify(x) hidden_ve_stringify(x)

#define ve_delete_copy_constructors(type) \
    type(type& other) = delete; \
    type& operator=(type& other) = delete; \
    type(const type& other) = delete; \
    type& operator=(const type& other) = delete;

#define ve_delete_move_constructors(type) \
    type(type&& other) = delete; \
    type& operator=(type&& other) = delete;

#ifdef NDEBUG
    #define ve_fail(message) \
        { \
            std::abort(); \
        }
#else
    #define ve_fail(message) \
        { \
            std::println(ANSI_ERRR message "\nIn line " ve_stringify(__LINE__) " in file " __FILE__ ANSI_RSET); \
            std::abort(); \
        }
#endif

#ifdef NDEBUG
    #define ve_assert(expression) \
        { \
            bool result_ = static_cast<bool>(expression); \
            if (result_ != true) { \
                std::abort(); \
            } \
        }
#else
    #define ve_assert(expression) \
        { \
            bool result_ = static_cast<bool>(expression); \
            if (result_ != true) { \
                std::println(ANSI_ERRR "Assert failed in line " ve_stringify(__LINE__) " in file " __FILE__ ":\n" #expression ANSI_RSET); \
                std::abort(); \
            } \
        }
#endif

#ifdef NDEBUG
    #define ve_debugassert(expression) \
        { \
        }
#else
    #define ve_debugassert(expression) \
        { \
            bool result_ = static_cast<bool>(expression); \
            if (result_ != true) { \
                std::println(ANSI_ERRR "Debug assert failed in line " ve_stringify(__LINE__) " in file " __FILE__ ":\n" #expression ANSI_RSET); \
            } \
        }
#endif

#define ve_assertcatch(expression, caseTrue, caseFalse) \
    { \
        bool result_ = static_cast<bool>(expression); \
        if (result_ == true) { \
            caseTrue \
        } else { \
            caseFalse \
        } \
    }

#ifdef NDEBUG
    #define ve_vkcheck(expression) \
        { \
            VkResult check_result_ = expression; \
            if (check_result_ != VK_SUCCESS) { \
                std::abort(); \
            } \
        }
#else
    #define ve_vkcheck(expression) \
        { \
            VkResult check_result_ = expression; \
            if (check_result_ != VK_SUCCESS) { \
                std::println(ANSI_ERRR "Unexpected VkResult \"{}\"\nIn line " ve_stringify(__LINE__) " in file " __FILE__ ANSI_RSET, check_result_); \
                std::abort(); \
            } \
        }
#endif

#ifdef NDEBUG
    #define ve_vkcheck_switch(expression, ...) \
        { \
            VkResult switch_result_ = expression; \
            switch (switch_result_) { \
                __VA_ARGS__ \
                default: { \
                    std::abort(); \
                } break; \
            } \
        }
#else
    #define ve_vkcheck_switch(expression, ...) \
        { \
            VkResult switch_result_ = expression; \
            switch (switch_result_) { \
                __VA_ARGS__ \
                default: { \
                    std::println(ANSI_ERRR "Unexpected VkResult \"{}\"\nIn line " ve_stringify(__LINE__) " in file " __FILE__ ANSI_RSET, switch_result_); \
                    std::abort(); \
                } break; \
            } \
        }
#endif

#define ve_check_case(value, expression) \
    case value: expression break

namespace ve {
    template <typename T> constexpr T* add_bytes(T* in, size_t bytes) noexcept
    {
        return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) + bytes);
    }

    template <typename T> constexpr T* sub_bytes(T* in, size_t bytes) noexcept
    {
        return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) - bytes);
    }

    template <typename T> constexpr T* add_bytes_t(void* in, size_t bytes) noexcept
    {
        return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) + bytes);
    }

    template <typename T> constexpr T* sub_bytes_t(void* in, size_t bytes) noexcept
    {
        return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) - bytes);
    }

    constexpr size_t align_up(size_t value, size_t alignment) noexcept
    {
        return (value + alignment - 1_size) & ~(alignment - 1_size);
    }

    constexpr uint64_t set_bits_u64(uint32_t count) noexcept
    {
        return (count == 0_u32) ? (0_u64) : ((1_u64 << count) - 1_u64);
    }

    constexpr uint32_t set_bits_u32(uint32_t count) noexcept
    {
        return (count == 0_u32) ? (0_u32) : ((1_u32 << count) - 1_u32);
    }

    constexpr bool has_flag(uint32_t in, uint32_t flag) noexcept
    {
        return static_cast<bool>(in & flag);
    }

    template <uint32_t N> constexpr inline bool is_all_true(bool (&check)[N]) noexcept
    {
        bool result { true };
        for (uint32_t i {}; (i < N) & result; i++) {
            result &= check[i];
        }
        return result;
    }
}
