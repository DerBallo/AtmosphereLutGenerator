#pragma once

#ifndef ve_std_stdint_h_included
    #define ve_std_stdint_h_included
    #include <stdint.h>
#endif
#ifndef ve_std_cmath_included
    #define ve_std_cmath_included
    #include <cmath>
#endif
#ifndef ve_std_bit_included
    #define ve_std_bit_included
    #include <bit>
#endif

/*
typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;
*/

constexpr int8_t operator""_i8(unsigned long long value) noexcept { return static_cast<int8_t>(value); }
constexpr uint8_t operator""_u8(unsigned long long value) noexcept { return static_cast<uint8_t>(value); }
constexpr int16_t operator""_i16(unsigned long long value) noexcept { return static_cast<int16_t>(value); }
constexpr uint16_t operator""_u16(unsigned long long value) noexcept { return static_cast<uint16_t>(value); }
constexpr int32_t operator""_i32(unsigned long long value) noexcept { return static_cast<int32_t>(value); }
constexpr uint32_t operator""_u32(unsigned long long value) noexcept { return static_cast<uint32_t>(value); }
constexpr int64_t operator""_i64(unsigned long long value) noexcept { return static_cast<int64_t>(value); }
constexpr uint64_t operator""_u64(unsigned long long value) noexcept { return static_cast<uint64_t>(value); }

/*
constexpr int128_t operator""_i128(unsigned long long value) noexcept { return static_cast<int128_t>(value); }
constexpr uint128_t operator""_u128(unsigned long long value) noexcept { return static_cast<uint128_t>(value); }
*/

constexpr float operator""_f(long double value) noexcept { return static_cast<float>(value); }
constexpr float operator""_f(unsigned long long value) noexcept { return static_cast<float>(value); }
constexpr double operator""_d(long double value) noexcept { return static_cast<double>(value); }
constexpr double operator""_d(unsigned long long value) noexcept { return static_cast<double>(value); }
constexpr long double operator""_ld(long double value) noexcept { return static_cast<long double>(value); }
constexpr long double operator""_ld(unsigned long long value) noexcept { return static_cast<long double>(value); }

inline constexpr long double deg_to_rad_ld { 0.017453292519943295769236907684886_ld };
inline constexpr double deg_to_rad_d { 0.017453292519943295769236907684886_d };
inline constexpr float deg_to_rad_f { 0.017453292519943295769236907684886_f };
inline constexpr long double rad_to_deg_ld { 57.295779513082320876798154814105_ld };
inline constexpr double rad_to_deg_d { 57.295779513082320876798154814105_d };
inline constexpr float rad_to_deg_f { 57.295779513082320876798154814105_f };
inline constexpr long double pi_ld { 3.1415926535897932384626433832795_ld };
inline constexpr double pi_d { 3.1415926535897932384626433832795_d };
inline constexpr float pi_f { 3.1415926535897932384626433832795_f };
inline constexpr long double sphere_factor_ld { 0.62035049089940001666800681204778_ld };
inline constexpr double sphere_factor_d { 0.62035049089940001666800681204778_d };
inline constexpr float sphere_factor_f { 0.62035049089940001666800681204778_f };

constexpr float fov_to_focal_distance_f(float fov) noexcept
{
    return 0.5_f / std::tan(fov * 0.5_f * deg_to_rad_f);
}

constexpr double fov_to_focal_distance_d(double fov) noexcept
{
    return 0.5_d / std::tan(fov * 0.5_d * deg_to_rad_d);
}

constexpr long double fov_to_focal_distance_ld(long double fov) noexcept
{
    return 0.5_ld / std::tan(fov * 0.5_ld * deg_to_rad_ld);
}

/*
constexpr float units_to_meters_f(int128_t value) noexcept
{
    if (value == 0) { return 0.0_f; }
    uint32_t sign;
    if (value < 0) {
        value = -value;
        sign = 0x80000000_u32;
    } else {
        sign = 0_u32;
    }
    uint64_t high = static_cast<uint64_t>(value >> 64_u32);
    int32_t exponent;
    if (high == 0_u64) {
        uint64_t low = static_cast<uint64_t>(value);
        exponent = 158_i32 - std::countl_zero(low);
    } else {
        exponent = 222_i32 - std::countl_zero(high);
    }
    int32_t shift = exponent - 118_i32;
    uint32_t mantissa;
    if (shift < 0) {
        mantissa = static_cast<uint32_t>(value << (-shift));
    } else {
        mantissa = static_cast<uint32_t>(value >> shift);
    }
    return std::bit_cast<float>(sign | static_cast<uint32_t>(exponent << 23_u32) | (mantissa & 0x7FFFFF_u32));
}

constexpr int128_t meters_to_units_f(float value) noexcept
{
    return static_cast<int128_t>(value * 4294967296.0_f);
}

template <std::floating_point T>
constexpr T units_to_meters_slow(int128_t value) noexcept
{
    static constexpr T multiplier { static_cast<T>(1.0_ld / 4294967296.0_ld) };
    return static_cast<T>(value) * multiplier;
}

template <std::floating_point T>
constexpr int128_t meters_to_units_slow(T value) noexcept
{
    static constexpr T multiplier { static_cast<T>(4294967296.0_ld) };
    return static_cast<int128_t>(value * multiplier);
}
*/
