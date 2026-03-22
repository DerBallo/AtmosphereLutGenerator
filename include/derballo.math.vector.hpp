#pragma once

#include "derballo.math.scalar.hpp"

struct alignas(8) float2_t {
    float x, y;

    constexpr float2_t operator+(float2_t other) const noexcept
    {
        return {
            x + other.x,
            y + other.y,
        };
    }

    constexpr float2_t operator+(float other) const noexcept
    {
        return {
            x + other,
            y + other,
        };
    }

    constexpr float2_t operator-(float2_t other) const noexcept
    {
        return {
            x - other.x,
            y - other.y,
        };
    }

    constexpr float2_t operator-(float other) const noexcept
    {
        return {
            x - other,
            y - other,
        };
    }

    constexpr float2_t operator*(float2_t other) const noexcept
    {
        return {
            x * other.x,
            y * other.y,
        };
    }

    constexpr float2_t operator*(float other) const noexcept
    {
        return {
            x * other,
            y * other,
        };
    }

    constexpr float2_t operator/(float2_t other) const noexcept
    {
        return {
            x / other.x,
            y / other.y,
        };
    }

    constexpr float2_t operator/(float other) const noexcept
    {
        return {
            x / other,
            y / other,
        };
    }

    constexpr void operator+=(float2_t other) noexcept
    {
        x += other.x;
        y += other.y;
    }

    constexpr void operator+=(float other) noexcept
    {
        x += other;
        y += other;
    }

    constexpr void operator-=(float2_t other) noexcept
    {
        x -= other.x;
        y -= other.y;
    }

    constexpr void operator-=(float other) noexcept
    {
        x -= other;
        y -= other;
    }

    constexpr void operator*=(float2_t other) noexcept
    {
        x *= other.x;
        y *= other.y;
    }

    constexpr void operator*=(float other) noexcept
    {
        x *= other;
        y *= other;
    }

    constexpr void operator/=(float2_t other) noexcept
    {
        x /= other.x;
        y /= other.y;
    }

    constexpr void operator/=(float other) noexcept
    {
        x /= other;
        y /= other;
    }

    constexpr float length_squared() const noexcept
    {
        return x * x + y * y;
    }

    constexpr float length() const noexcept
    {
        return std::sqrt(length_squared());
    }

    constexpr float2_t normalized() const noexcept
    {
        float invLen { 1.0_f / length() };
        return { x * invLen, y * invLen };
    }

    constexpr void normalize() noexcept
    {
        float invLen { 1.0_f / length() };
        x *= invLen;
        y *= invLen;
    }

    constexpr float dot(const float2_t& other) const noexcept
    {
        return x * other.x + y * other.y;
    }

    constexpr float2_t abs() const noexcept
    {
        return { std::abs(x), std::abs(y) };
    }

    constexpr float2_t lerp(const float2_t& other, float t) const noexcept
    {
        return (*this) * (1.0_f - t) + other * t;
    }

    constexpr float2_t exp() const noexcept
    {
        return { std::exp(x), std::exp(y) };
    }

    constexpr float2_t() noexcept
        : x(0.0_f), y(0.0_f)
    {
    }

    constexpr float2_t(float x, float y) noexcept
        : x(x), y(y)
    {
    }

    constexpr float2_t(const float2_t& other) noexcept
        : x(other.x), y(other.y)
    {
    }

    constexpr void operator=(const float2_t& other) noexcept
    {
        x = other.x;
        y = other.y;
    }
};

struct alignas(16) float3_t {
    float x, y, z;

    constexpr float3_t operator+(float3_t other) const noexcept
    {
        return {
            x + other.x,
            y + other.y,
            z + other.z,
        };
    }

    constexpr float3_t operator+(float other) const noexcept
    {
        return {
            x + other,
            y + other,
            z + other,
        };
    }

    constexpr float3_t operator-(float3_t other) const noexcept
    {
        return {
            x - other.x,
            y - other.y,
            z - other.z,
        };
    }

    constexpr float3_t operator-(float other) const noexcept
    {
        return {
            x - other,
            y - other,
            z - other,
        };
    }

    constexpr float3_t operator*(float3_t other) const noexcept
    {
        return {
            x * other.x,
            y * other.y,
            z * other.z,
        };
    }

    constexpr float3_t operator*(float other) const noexcept
    {
        return {
            x * other,
            y * other,
            z * other,
        };
    }

    constexpr float3_t operator/(float3_t other) const noexcept
    {
        return {
            x / other.x,
            y / other.y,
            z / other.z,
        };
    }

    constexpr float3_t operator/(float other) const noexcept
    {
        return {
            x / other,
            y / other,
            z / other,
        };
    }

    constexpr void operator+=(float3_t other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    constexpr void operator+=(float other) noexcept
    {
        x += other;
        y += other;
        z += other;
    }

    constexpr void operator-=(float3_t other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    constexpr void operator-=(float other) noexcept
    {
        x -= other;
        y -= other;
        z -= other;
    }

    constexpr void operator*=(float3_t other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    constexpr void operator*=(float other) noexcept
    {
        x *= other;
        y *= other;
        z *= other;
    }

    constexpr void operator/=(float3_t other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
    }

    constexpr void operator/=(float other) noexcept
    {
        x /= other;
        y /= other;
        z /= other;
    }

    constexpr float length_squared() const noexcept
    {
        return x * x + y * y + z * z;
    }

    constexpr float length() const noexcept
    {
        return std::sqrt(length_squared());
    }

    constexpr float3_t normalized() const noexcept
    {
        float invLen { 1.0_f / length() };
        return {
            x * invLen,
            y * invLen,
            z * invLen,
        };
    }

    constexpr void normalize() noexcept
    {
        float invLen { 1.0_f / length() };
        x *= invLen;
        y *= invLen;
        z *= invLen;
    }

    constexpr float dot(const float3_t& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr float3_t abs() const noexcept
    {
        return { std::abs(x), std::abs(y), std::abs(z) };
    }

    constexpr float3_t lerp(const float3_t& other, float t) const noexcept
    {
        return (*this) * (1.0_f - t) + other * t;
    }

    constexpr float3_t exp() const noexcept
    {
        return { std::exp(x), std::exp(y), std::exp(z) };
    }

    constexpr float3_t() noexcept
        : x(0.0_f), y(0.0_f), z(0.0_f)
    {
    }

    constexpr float3_t(float x, float y, float z) noexcept
        : x(x), y(y), z(z)
    {
    }

    constexpr float3_t(const float3_t& other) noexcept
        : x(other.x), y(other.y), z(other.z)
    {
    }

    constexpr void operator=(const float3_t& other) noexcept
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
};

struct alignas(16) float4_t {
    float x, y, z, w;

    constexpr float4_t operator+(float4_t other) const noexcept
    {
        return {
            x + other.x,
            y + other.y,
            z + other.z,
            w + other.w,
        };
    }

    constexpr float4_t operator+(float other) const noexcept
    {
        return {
            x + other,
            y + other,
            z + other,
            w + other,
        };
    }

    constexpr float4_t operator-(float4_t other) const noexcept
    {
        return {
            x - other.x,
            y - other.y,
            z - other.z,
            w - other.w,
        };
    }

    constexpr float4_t operator-(float other) const noexcept
    {
        return {
            x - other,
            y - other,
            z - other,
            w - other,
        };
    }

    constexpr float4_t operator*(float4_t other) const noexcept
    {
        return {
            x * other.x,
            y * other.y,
            z * other.z,
            w * other.w,
        };
    }

    constexpr float4_t operator*(float other) const noexcept
    {
        return {
            x * other,
            y * other,
            z * other,
            w * other,
        };
    }

    constexpr float4_t operator/(float4_t other) const noexcept
    {
        return {
            x / other.x,
            y / other.y,
            z / other.z,
            w / other.w,
        };
    }

    constexpr float4_t operator/(float other) const noexcept
    {
        return {
            x / other,
            y / other,
            z / other,
            w / other,
        };
    }

    constexpr void operator+=(float4_t other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
    }

    constexpr void operator+=(float other) noexcept
    {
        x += other;
        y += other;
        z += other;
        w += other;
    }

    constexpr void operator-=(float4_t other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
    }

    constexpr void operator-=(float other) noexcept
    {
        x -= other;
        y -= other;
        z -= other;
        w -= other;
    }

    constexpr void operator*=(float4_t other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
    }

    constexpr void operator*=(float other) noexcept
    {
        x *= other;
        y *= other;
        z *= other;
        w *= other;
    }

    constexpr void operator/=(float4_t other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
    }

    constexpr void operator/=(float other) noexcept
    {
        x /= other;
        y /= other;
        z /= other;
        w /= other;
    }

    constexpr float length_squared() const noexcept
    {
        return x * x + y * y + z * z + w * w;
    }

    constexpr float length() const noexcept
    {
        return std::sqrt(length_squared());
    }

    constexpr float4_t normalized() const noexcept
    {
        float invLen { 1.0_f / length() };
        return { x * invLen, y * invLen, z * invLen, w * invLen };
    }

    constexpr void normalize() noexcept
    {
        float invLen { 1.0_f / length() };
        x *= invLen;
        y *= invLen;
        z *= invLen;
        w *= invLen;
    }

    constexpr float dot(const float4_t& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    constexpr float4_t abs() const noexcept
    {
        return { std::abs(x), std::abs(y), std::abs(z), std::abs(w) };
    }

    constexpr float4_t lerp(const float4_t& other, float t) const noexcept
    {
        return (*this) * (1.0_f - t) + other * t;
    }

    constexpr float4_t exp() const noexcept
    {
        return { std::exp(x), std::exp(y), std::exp(z), std::exp(w) };
    }

    constexpr float4_t() noexcept
        : x(0.0_f), y(0.0_f), z(0.0_f), w(0.0_f)
    {
    }

    constexpr float4_t(float x, float y, float z, float w) noexcept
        : x(x), y(y), z(z), w(w)
    {
    }

    constexpr float4_t(const float4_t& other) noexcept
        : x(other.x), y(other.y), z(other.z), w(other.w)
    {
    }

    constexpr void operator=(const float4_t& other) noexcept
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
};

struct alignas(16) double3_t {
    double x, y, z;

    constexpr double3_t operator+(double3_t other) const noexcept
    {
        return {
            x + other.x,
            y + other.y,
            z + other.z,
        };
    }

    constexpr double3_t operator+(double other) const noexcept
    {
        return {
            x + other,
            y + other,
            z + other,
        };
    }

    constexpr double3_t operator-(double3_t other) const noexcept
    {
        return {
            x - other.x,
            y - other.y,
            z - other.z,
        };
    }

    constexpr double3_t operator-(double other) const noexcept
    {
        return {
            x - other,
            y - other,
            z - other,
        };
    }

    constexpr double3_t operator*(double3_t other) const noexcept
    {
        return {
            x * other.x,
            y * other.y,
            z * other.z,
        };
    }

    constexpr double3_t operator*(double other) const noexcept
    {
        return {
            x * other,
            y * other,
            z * other,
        };
    }

    constexpr double3_t operator/(double3_t other) const noexcept
    {
        return {
            x / other.x,
            y / other.y,
            z / other.z,
        };
    }

    constexpr double3_t operator/(double other) const noexcept
    {
        return {
            x / other,
            y / other,
            z / other,
        };
    }

    constexpr void operator+=(double3_t other) noexcept
    {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    constexpr void operator+=(double other) noexcept
    {
        x += other;
        y += other;
        z += other;
    }

    constexpr void operator-=(double3_t other) noexcept
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    constexpr void operator-=(double other) noexcept
    {
        x -= other;
        y -= other;
        z -= other;
    }

    constexpr void operator*=(double3_t other) noexcept
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    constexpr void operator*=(double other) noexcept
    {
        x *= other;
        y *= other;
        z *= other;
    }

    constexpr void operator/=(double3_t other) noexcept
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
    }

    constexpr void operator/=(double other) noexcept
    {
        x /= other;
        y /= other;
        z /= other;
    }

    constexpr double length_squared() const noexcept
    {
        return x * x + y * y + z * z;
    }

    constexpr double length() const noexcept
    {
        return std::sqrt(length_squared());
    }

    constexpr double3_t normalized() const noexcept
    {
        double invLen { 1.0_f / length() };
        return { x * invLen, y * invLen, z * invLen };
    }

    constexpr void normalize() noexcept
    {
        double invLen { 1.0_f / length() };
        x *= invLen;
        y *= invLen;
        z *= invLen;
    }

    constexpr double dot(const double3_t& other) const noexcept
    {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr double3_t abs() const noexcept
    {
        return { std::abs(x), std::abs(y), std::abs(z) };
    }

    constexpr double3_t lerp(const double3_t& other, double t) const noexcept
    {
        return (*this) * (1.0_d - t) + other * t;
    }

    constexpr double3_t() noexcept
        : x(0.0_d), y(0.0_d), z(0.0_d)
    {
    }

    constexpr double3_t(double x, double y, double z) noexcept
        : x(x), y(y), z(z)
    {
    }

    constexpr double3_t(const double3_t& other) noexcept
        : x(other.x), y(other.y), z(other.z)
    {
    }

    constexpr void operator=(const double3_t& other) noexcept
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
};

/*
struct alignas(16) int128_3_t {
    int128_t x, y, z;

    constexpr void operator+=(float3_t other) noexcept
    {
        x += meters_to_units_f(other.x);
        y += meters_to_units_f(other.y);
        z += meters_to_units_f(other.z);
    }

    constexpr void operator-=(float3_t other) noexcept
    {
        x -= meters_to_units_f(other.x);
        y -= meters_to_units_f(other.y);
        z -= meters_to_units_f(other.z);
    }

    constexpr float3_t to_meters() noexcept
    {
        return {
            units_to_meters_f(x),
            units_to_meters_f(y),
            units_to_meters_f(z),
        };
    }

    constexpr float3_t relative_position(int128_3_t& other) noexcept
    {
        return {
            units_to_meters_f(x - other.x),
            units_to_meters_f(y - other.y),
            units_to_meters_f(z - other.z),
        };
    }

    constexpr int128_t approx_length()
    {
        int128_t xabs = std::abs(x);
        int128_t yabs = std::abs(y);
        int128_t zabs = std::abs(z);

        int128_t max = (xabs > yabs && xabs > zabs) ? xabs : ((yabs > zabs) ? yabs : zabs);
        int128_t sum = xabs + yabs + zabs;

        return (max + sum) >> 1;
    }

    constexpr int128_3_t()
        : x(0_i128), y(0_i128), z(0_i128)
    {
    }

    constexpr int128_3_t(int128_t x, int128_t y, int128_t z)
        : x(x), y(y), z(z)
    {
    }

    constexpr int128_3_t(const int128_3_t& other)
        : x(other.x), y(other.y), z(other.z)
    {
    }
};

struct uint128_3_t {
    uint128_t x;
    uint128_t y;
    uint128_t z;

    constexpr uint128_3_t()
        : x(0_u128), y(0_u128), z(0_u128)
    {
    }

    constexpr uint128_3_t(int128_t x, int128_t y, int128_t z)
        : x(x), y(y), z(z)
    {
    }

    constexpr uint128_3_t(const uint128_3_t& other)
        : x(other.x), y(other.y), z(other.z)
    {
    }
};
*/
