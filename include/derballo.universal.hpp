#pragma once

#ifndef ve_std_memory_included
    #define ve_std_memory_included
    #include <memory>
#endif
#ifndef ve_std_stddef_h_included
    #define ve_std_stddef_h_included
    #include <stddef.h>
#endif

typedef std::byte byte_t;

constexpr size_t operator""_size(unsigned long long value) noexcept { return static_cast<size_t>(value); }
constexpr ptrdiff_t operator""_ptrdiff(unsigned long long value) noexcept { return static_cast<ptrdiff_t>(value); }
constexpr byte_t operator""_b(unsigned long long value) noexcept { return static_cast<byte_t>(value); }

#include "derballo.math.matrix.hpp"
#include "derballo.math.scalar.hpp"
#include "derballo.math.vector.hpp"

using std::construct_at;
using std::destroy_at;

template <typename T, typename... Args> void reconstruct_at(T* obj, Args&&... args)
{
    if constexpr (!std::is_trivially_destructible_v<T>) {
        destroy_at(obj);
    }
    construct_at(obj, std::forward<Args>(args)...);
}

template <typename T, size_t N> constexpr size_t lengthof(const T (&)[N]) noexcept
{
    return N;
};

template <typename T>
constexpr std::remove_reference_t<T>* addressof(T&& _) noexcept
{
    return std::addressof(_);
}
