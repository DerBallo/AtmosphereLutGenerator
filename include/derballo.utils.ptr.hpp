#pragma once

#include "derballo.universal.hpp"

namespace ve {
    template <typename T> constexpr T* add_bytes(T* in, size_t bytes) noexcept { return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) + bytes); }

    template <typename T> constexpr T* sub_bytes(T* in, size_t bytes) noexcept { return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) - bytes); }

    template <typename T> constexpr T* add_bytes_t(void* in, size_t bytes) noexcept { return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) + bytes); }

    template <typename T> constexpr T* sub_bytes_t(void* in, size_t bytes) noexcept { return reinterpret_cast<T*>(reinterpret_cast<byte_t*>(in) - bytes); }

    constexpr size_t alignup(size_t value, size_t alignment) { return (value + alignment - 1_size) & ~(alignment - 1_size); }
}
