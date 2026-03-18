#include <cstdio>

#ifndef ve_std_stdint_h_included
    #define ve_std_stdint_h_included
    #include <stdint.h>
#endif
#ifndef ve_std_cmath_included
    #define ve_std_cmath_included
    #include <cmath>
#endif

#include <filesystem>
#include <fstream>

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

template <typename T, size_t N> constexpr size_t lengthof(const T (&)[N]) noexcept
{
    return N;
};

inline constexpr float pi_f { 3.1415926535897932384626433832795_f };

struct float3_t {
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
        return { x * invLen, y * invLen, z * invLen };
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

inline float clamp(float x, float a, float b)
{
    return std::max(a, std::min(b, x));
}

inline float signf(float x)
{
    return x >= 0.0_f ? 1.0_f : -1.0_f;
}

float densityR(float negativeHeight, float rayleighScaleHeight)
{
    return std::min(std::exp(negativeHeight / rayleighScaleHeight), 100.0_f);
}

float densityM(float negativeHeight, float mieScaleHeight)
{
    return std::min(std::exp(negativeHeight / mieScaleHeight), 100.0_f);
}

float phaseRayleigh(float mu)
{
    return (3.0f / (16.0_f * pi_f)) * (1.0_f + mu * mu);
}

float phaseMie(float mu, float g)
{
    float g2 = g * g;
    return (3.0_f / (8.0_f * pi_f)) * ((1.0_f - g2) * (1.0_f + mu * mu)) / ((2.0_f + g2) * std::pow(1.0_f + g2 - 2.0_f * g * mu, 1.5_f));
}

void GenerateAtmosphereLut(
    float3_t* out,
    uint32_t dimR,
    uint32_t dimMu,
    uint32_t dimMuS,
    uint32_t viewSamples,
    uint32_t lightSamples,
    uint32_t multiScatteringSteps,
    float earthRadius,
    float atmosphereRadius,
    float rayleighScaleHeight,
    float mieScaleHeight,
    float3_t BetaR,
    float3_t BetaMScattering,
    float3_t BetaMExtinction,
    float mieG
)
{
    float Rg { earthRadius };
    float Rt { atmosphereRadius };

    for (uint32_t ir {}; ir < dimR; ++ir) {
        float vr { static_cast<float>(ir) / static_cast<float>(dimR - 1_u32) };
        float r { Rg + (Rt - Rg) * (vr * vr) };

        for (uint32_t imu {}; imu < dimMu; ++imu) {
            float vu { static_cast<float>(imu) / static_cast<float>(dimMu - 1_u32) };
            float mu { signf(vu - 0.5_f) * std::pow(std::abs(2.0_f * vu - 1.0_f), 2.0_f) };

            float3_t viewDir { std::sqrt(1.0_f - mu * mu), mu, 0.0_f };
            viewDir.normalize();

            for (uint32_t ims {}; ims < dimMuS; ++ims) {
                float vs { (float)ims / (float)(dimMuS - 1) };
                float muS { signf(vs - 0.5_f) * std::pow(std::abs(2.0_f * vs - 1.0_f), 2.0_f) };

                float3_t sunDir { std::sqrt(1.0_f - muS * muS), muS, 0.0_f };
                sunDir.normalize();

                float3_t origin { 0.0_f, r, 0.0_f };

                float3_t sum {};

                float opticalDepthR {};
                float opticalDepthM {};

                float tMax { 2.0_f * Rt };

                for (uint32_t i {}; i < viewSamples; ++i) {
                    float u0 { static_cast<float>(i) / viewSamples };
                    float u1 { static_cast<float>(i + 1_u32) / viewSamples };

                    u0 = u0 * u0;
                    u1 = u1 * u1;

                    float t0 { u0 * tMax };
                    float t1 { u1 * tMax };
                    float t { 0.5_f * (t0 + t1) };
                    float ds { t1 - t0 };

                    float3_t P { origin + viewDir * t };
                    float rP { P.length() };

                    if (rP > Rt) {
                        break;
                    }

                    float nHP { -(rP - earthRadius) };

                    float dR { densityR(nHP, rayleighScaleHeight) };
                    float dM { densityM(nHP, mieScaleHeight) };

                    float Hr { dR * ds };
                    float Hm { dM * ds };

                    opticalDepthR += Hr;
                    opticalDepthM += Hm;

                    float opticalDepthLR {};
                    float opticalDepthLM {};

                    for (uint32_t j {}; j < lightSamples; ++j) {
                        float v0 { static_cast<float>(j) / lightSamples };
                        float v1 { static_cast<float>(j + 1_u32) / lightSamples };

                        v0 = v0 * v0;
                        v1 = v1 * v1;

                        float tL0 { v0 * tMax };
                        float tL1 { v1 * tMax };
                        float tL { 0.5_f * (tL0 + tL1) };
                        float dsL { tL1 - tL0 };

                        float3_t Pl { P + sunDir * tL };
                        float rL { Pl.length() };

                        if (rL > Rt) {
                            break;
                        }

                        float nHL { -(rP - earthRadius) };

                        opticalDepthLR += densityR(nHL, rayleighScaleHeight) * dsL;
                        opticalDepthLM += densityM(nHL, mieScaleHeight) * dsL;
                    }

                    float3_t tau { BetaR * (opticalDepthR + opticalDepthLR) + BetaMExtinction * (opticalDepthM + opticalDepthLM) };

                    float3_t attenuation { float3_t { -tau.x, -tau.y, -tau.z }.exp() };

                    float muPhase { viewDir.dot(sunDir) };

                    float phaseR { phaseRayleigh(muPhase) };
                    float phaseM { phaseMie(muPhase, mieG) };

                    float3_t scattering { BetaR * phaseR + BetaMScattering * phaseM };

                    sum += scattering * attenuation * ds;
                }

                float3_t multi { sum };
                for (uint32_t k {}; k < multiScatteringSteps; ++k) {
                    multi *= 0.5_f;
                    sum += multi;
                }

                out[ir * dimMu * dimMuS + imu * dimMuS + ims] = sum;
            }
        }
    }
}

int main()
{
    constexpr uint32_t lutR { 3_u32 };
    constexpr uint32_t lutMu { 3_u32 };
    constexpr uint32_t lutMuS { 3_u32 };
    float3_t LUT[lutR * lutMu * lutMuS];

    GenerateAtmosphereLut(
        LUT,
        lutR,
        lutMu,
        lutMuS,
        100_u32,
        100_u32,
        4_u32,
        6360e3_f,
        6560e3_f,
        7994.0_f,
        1200.0_f,
        { 5.8e-6_f, 13e-6_f, 22.4e-6_f },
        { 20e-6_f, 20e-6_f, 20e-6_f },
        { 22e-6_f, 22e-6_f, 22e-6_f },
        0.76f
    );

    std::ofstream("lut.bin", std::ios::binary).write(reinterpret_cast<char*>(LUT), lengthof(LUT) * sizeof(float3_t));

    return 0;
}
