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

void GenerateAtmosphereLut(
    float4_t* out,
    uint32_t dimR,
    uint32_t dimMu,
    uint32_t dimMuS,
    uint32_t viewSamples,
    uint32_t lightSamples,
    float earthRadius,
    float atmosphereRadius,
    float intensity,
    float rayleighScaleHeight,
    float mieScaleHeight,
    float mieAnisotropy,
    float3_t BetaRScattering,
    float3_t BetaMScattering,
    float3_t BetaMAbsorption
)
{
    float atmosphereRadius2 = atmosphereRadius * atmosphereRadius;
    float earthRadius2 = earthRadius * earthRadius;

    for (uint32_t ir {}; ir < dimR; ++ir) {
        printf("Slice %d of %d\n", ir, dimR);

        float vr { static_cast<float>(ir) / static_cast<float>(dimR - 1) };
        float r { earthRadius + (atmosphereRadius - earthRadius) * (vr * vr) };

        for (uint32_t imu {}; imu < dimMu; ++imu) {
            float vu { static_cast<float>(imu) / static_cast<float>(dimMu - 1) };
            float mu { signf(vu - 0.5f) * std::pow(std::abs(2.0f * vu - 1.0f), 2.0f) };

            float3_t viewDir { std::sqrt(1.0f - mu * mu), mu, 0.0f };
            viewDir.normalize();

            for (uint32_t ims {}; ims < dimMuS; ++ims) {
                float vs { static_cast<float>(ims) / static_cast<float>(dimMuS - 1) };
                float muS { signf(vs - 0.5f) * std::pow(std::abs(2.0f * vs - 1.0f), 2.0f) };

                float3_t sunDir { std::sqrt(1.0f - muS * muS), muS, 0.0f };
                sunDir.normalize();

                float3_t origin { 0.0f, r, 0.0f };

                float B { origin.dot(viewDir) };
                float C { origin.dot(origin) - atmosphereRadius2 };
                float D { B * B - C };

                if (D < 0.0f) {
                    out[ir * dimMu * dimMuS + imu * dimMuS + ims] = {};
                    continue;
                }

                float tMax { -B + std::sqrt(D) };

                float3_t sumR {};
                float3_t sumM {};

                float opticalDepthR {};
                float opticalDepthM {};

                for (uint32_t i {}; i < viewSamples; ++i) {
                    float u0 { static_cast<float>(i) / viewSamples };
                    float u1 { static_cast<float>(i + 1) / viewSamples };

                    float t0 { u0 * tMax };
                    float t1 { u1 * tMax };
                    float t { 0.5f * (t0 + t1) };
                    float ds { t1 - t0 };

                    float3_t P { origin + viewDir * t };
                    float rP { P.length() };

                    if (rP < earthRadius || rP > atmosphereRadius)
                        break;

                    float h { rP - earthRadius };

                    float dR { std::exp(-h / rayleighScaleHeight) };
                    float dM { std::exp(-h / mieScaleHeight) };

                    float Hr { dR * ds };
                    float Hm { dM * ds };

                    opticalDepthR += Hr;
                    opticalDepthM += Hm;

                    float opticalDepthLR {};
                    float opticalDepthLM {};

                    float B2 { P.dot(sunDir) };
                    float C2 { P.dot(P) - atmosphereRadius2 };
                    float D2 { B2 * B2 - C2 };

                    if (D2 < 0.0f)
                        continue;

                    float tMaxL { -B2 + std::sqrt(D2) };

                    bool occluded { false };

                    for (uint32_t j {}; j < lightSamples; ++j) {
                        float v0 { static_cast<float>(j) / lightSamples };
                        float v1 { static_cast<float>(j + 1) / lightSamples };

                        float tL0 { v0 * tMaxL };
                        float tL1 { v1 * tMaxL };
                        float tL { 0.5f * (tL0 + tL1) };
                        float dsL { tL1 - tL0 };

                        float3_t Pl { P + sunDir * tL };
                        float rL { Pl.length() };

                        if (rL < earthRadius) {
                            occluded = true;
                            break;
                        }

                        if (rL > atmosphereRadius)
                            break;

                        float hL { rL - earthRadius };

                        opticalDepthLR += std::exp(-hL / rayleighScaleHeight) * dsL;
                        opticalDepthLM += std::exp(-hL / mieScaleHeight) * dsL;
                    }

                    if (occluded)
                        continue;

                    float3_t tau { BetaRScattering * (opticalDepthR + opticalDepthLR) + (BetaMScattering + BetaMAbsorption) * (opticalDepthM + opticalDepthLM) };

                    float3_t attenuation { (float3_t {} - tau).exp() };

                    sumR += attenuation * Hr;
                    sumM += attenuation * Hm;
                }

                float3_t inscattering { (sumR * BetaRScattering + sumM * BetaMScattering) * intensity };

                out[ir * dimMu * dimMuS + imu * dimMuS + ims] = {
                    inscattering.x,
                    inscattering.y,
                    inscattering.z,
                    0.0f
                };
            }
        }
    }
}

int main()
{
    constexpr uint32_t lutR { 48_u32 };
    constexpr uint32_t lutMu { 96_u32 };
    constexpr uint32_t lutMuS { 32_u32 };
    constexpr uint32_t lutLength { lutR * lutMu * lutMuS };
    float4_t* LUT = new (std::nothrow) float4_t[lutLength];

    GenerateAtmosphereLut(
        LUT,
        lutR,
        lutMu,
        lutMuS,
        5000_u32,
        100_u32,
        6360e3_f,
        6560e3_f,
        20.0f,
        7994.0_f,
        1200.0_f,
        0.88_f,
        { 15e-6_f, 15e-6_f, 15e-6_f },
        { 70e-6_f, 50e-6_f, 35e-6_f },
        { 8e-6_f, 15e-6_f, 25e-6_f }
    );

    /*
    GenerateAtmosphereLut(
        LUT,
        lutR,
        lutMu,
        lutMuS,
        100_u32,
        100_u32,
        6360e3_f,
        6560e3_f,
        20.0f,
        7994.0_f,
        1200.0_f,
        0.88_f,
        { 5.8e-6_f, 13e-6_f, 22.4e-6_f },
        { 20e-6_f, 20e-6_f, 20e-6_f },
        { 22e-6_f, 22e-6_f, 22e-6_f }
    );
    */

    std::ofstream("lut.bin", std::ios::binary).write(reinterpret_cast<char*>(LUT), lutLength * sizeof(float4_t));

    return 0;
}
