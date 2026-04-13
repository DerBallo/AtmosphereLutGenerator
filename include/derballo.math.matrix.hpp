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

#include "derballo.math.vector.hpp"

struct alignas(16) float3x3_t {
    union alignas(16) {
        struct {
            float m00, m01, m02;
        };
        float3_t r0;
    };
    union alignas(16) {
        struct {
            float m10, m11, m12;
        };
        float3_t r1;
    };
    union alignas(16) {
        struct {
            float m20, m21, m22;
        };
        float3_t r2;
    };

    constexpr float3_t operator*(const float3_t& other) const noexcept
    {
        return {
            m00 * other.x + m01 * other.y + m02 * other.z,
            m10 * other.x + m11 * other.y + m12 * other.z,
            m20 * other.x + m21 * other.y + m22 * other.z
        };
    }

    static constexpr float3x3_t from_pitch_yaw_rad(float2_t pitchYaw) noexcept
    {
        float sp { std::sin(pitchYaw.x) };
        float cp { std::cos(pitchYaw.x) };
        float sy { std::sin(pitchYaw.y) };
        float cy { std::cos(pitchYaw.y) };

        return {
            cy,
            sy * sp,
            sy * cp,
            0.0_f,
            cp,
            -sp,
            -sy,
            cy * sp,
            cy * cp,
        };
    }

    static constexpr float3x3_t from_pitch_yaw_deg(float2_t pitchYaw) noexcept
    {
        float pitchRad { pitchYaw.x * deg_to_rad_f };
        float yawRad { pitchYaw.y * deg_to_rad_f };

        float sp { std::sin(pitchRad) };
        float cp { std::cos(pitchRad) };
        float sy { std::sin(yawRad) };
        float cy { std::cos(yawRad) };

        return {
            cy,
            sy * sp,
            sy * cp,
            0.0_f,
            cp,
            -sp,
            -sy,
            cy * sp,
            cy * cp,
        };
    }

    constexpr float3x3_t() noexcept
        : m00(1.0_f), m01(0.0_f), m02(0.0_f), m10(0.0_f), m11(1.0_f), m12(0.0_f), m20(0.0_f), m21(0.0_f), m22(1.0_f)
    {
    }

    constexpr float3x3_t(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) noexcept
        : m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12), m20(m20), m21(m21), m22(m22)
    {
    }

    constexpr float3x3_t(float3_t row0, float3_t row1, float3_t row2) noexcept
        : r0(row0), r1(row1), r2(row2)
    {
    }

    constexpr float3x3_t(const float3x3_t& other) noexcept
        : m00(other.m00), m01(other.m01), m02(other.m02), m10(other.m10), m11(other.m11), m12(other.m12), m20(other.m20), m21(other.m21), m22(other.m22)
    {
    }

    constexpr void operator=(const float3x3_t& other) noexcept
    {
        m00 = other.m00;
        m01 = other.m01;
        m02 = other.m02;
        m10 = other.m10;
        m11 = other.m11;
        m12 = other.m12;
        m20 = other.m20;
        m21 = other.m21;
        m22 = other.m22;
    }
};

struct alignas(16) float3x4_t {
    struct alignas(16) {
        float m00, m01, m02, m03;
    };
    struct alignas(16) {
        float m10, m11, m12, m13;
    };
    struct alignas(16) {
        float m20, m21, m22, m23;
    };

    constexpr float3_t operator*(const float4_t& other) const noexcept
    {
        return {
            m00 * other.x + m01 * other.y + m02 * other.z + m03 * other.w,
            m10 * other.x + m11 * other.y + m12 * other.z + m13 * other.w,
            m20 * other.x + m21 * other.y + m22 * other.z + m23 * other.w,
        };
    }

    constexpr float3_t operator*(const float3_t& other) const noexcept
    {
        return {
            m00 * other.x + m01 * other.y + m02 * other.z,
            m10 * other.x + m11 * other.y + m12 * other.z,
            m20 * other.x + m21 * other.y + m22 * other.z
        };
    }

    static constexpr float3x4_t from_pitch_yaw_rad_pos(float2_t pitchYaw, float3_t position) noexcept
    {
        float sp { std::sin(pitchYaw.x) };
        float cp { std::cos(pitchYaw.x) };
        float sy { std::sin(pitchYaw.y) };
        float cy { std::cos(pitchYaw.y) };

        return {
            cy,
            sy * sp,
            sy * cp,
            position.x,
            0.0_f,
            cp,
            -sp,
            position.y,
            -sy,
            cy * sp,
            cy * cp,
            position.z,
        };
    }

    static constexpr float3x4_t from_pitch_yaw_deg_pos(float2_t pitchYaw, float3_t position) noexcept
    {
        float pitchRad { pitchYaw.x * deg_to_rad_f };
        float yawRad { pitchYaw.y * deg_to_rad_f };

        float sp { std::sin(pitchRad) };
        float cp { std::cos(pitchRad) };
        float sy { std::sin(yawRad) };
        float cy { std::cos(yawRad) };

        return {
            cy,
            sy * sp,
            sy * cp,
            position.x,
            0.0_f,
            cp,
            -sp,
            position.y,
            -sy,
            cy * sp,
            cy * cp,
            position.z,
        };
    }

    constexpr void set_rotation_rad(float2_t pitchYaw) noexcept
    {
        float sp { std::sin(pitchYaw.x) };
        float cp { std::cos(pitchYaw.x) };
        float sy { std::sin(pitchYaw.y) };
        float cy { std::cos(pitchYaw.y) };

        m00 = cy;
        m01 = sy * sp;
        m02 = sy * cp;
        m10 = 0.0_f;
        m11 = cp;
        m12 = -sp;
        m20 = -sy;
        m21 = cy * sp;
        m22 = cy * cp;
    }

    constexpr void set_rotation_deg(float2_t pitchYaw) noexcept
    {
        float pitchRad { pitchYaw.x * deg_to_rad_f };
        float yawRad { pitchYaw.y * deg_to_rad_f };

        float sp { std::sin(pitchRad) };
        float cp { std::cos(pitchRad) };
        float sy { std::sin(yawRad) };
        float cy { std::cos(yawRad) };

        m00 = cy;
        m01 = sy * sp;
        m02 = sy * cp;
        m10 = 0.0_f;
        m11 = cp;
        m12 = -sp;
        m20 = -sy;
        m21 = cy * sp;
        m22 = cy * cp;
    }

    constexpr float3x4_t() noexcept
        : m00(1.0_f), m01(0.0_f), m02(0.0_f), m03(0.0_f), m10(1.0_f), m11(0.0_f), m12(0.0_f), m13(1.0_f), m20(1.0_f), m21(0.0_f), m22(0.0_f), m23(0.0_f)
    {
    }

    constexpr float3x4_t(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23) noexcept
        : m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13), m20(m20), m21(m21), m22(m22), m23(m23)
    {
    }

    constexpr float3x4_t(const float3x4_t& other) noexcept
        : m00(other.m00), m01(other.m01), m02(other.m02), m03(other.m03), m10(other.m10), m11(other.m11), m12(other.m12), m13(other.m13), m20(other.m20), m21(other.m21), m22(other.m22), m23(other.m23)
    {
    }

    constexpr void operator=(const float3x4_t& other) noexcept
    {
        m00 = other.m00;
        m01 = other.m01;
        m02 = other.m02;
        m03 = other.m03;
        m10 = other.m10;
        m11 = other.m11;
        m12 = other.m12;
        m13 = other.m13;
        m20 = other.m20;
        m21 = other.m21;
        m22 = other.m22;
        m23 = other.m23;
    }
};
