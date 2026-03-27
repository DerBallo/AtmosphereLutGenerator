[[vk::binding(0, 0)]]
RWTexture3D<float3> outputImage;

#ifndef RAY_POINTS
    #define RAY_POINTS 128
#endif
#ifndef SUN_POINTS
    #define SUN_POINTS 96
#endif
#ifndef HEIGHT_POINTS
    #define HEIGHT_POINTS 48
#endif

#ifndef VIEW_SAMPLES
    #define VIEW_SAMPLES 5000
#endif
#ifndef LIGHT_SAMPLES
    #define LIGHT_SAMPLES 1000
#endif

#ifndef ATMOSPHERE_START
    #define ATMOSPHERE_START 6345e3f
#endif
#ifndef SEA_LEVEL
    #define SEA_LEVEL 6360e3f
#endif
#ifndef ATMOSPHERE_END
    #define ATMOSPHERE_END 6460e3f
#endif
static constexpr float ATMOSPHERE_END_SQUARED = ATMOSPHERE_END * ATMOSPHERE_END;
static constexpr float ATMOSPHERE_RANGE = ATMOSPHERE_END - ATMOSPHERE_START;

#ifndef RAYLEIGH_SCALE_HEIGHT
    #define RAYLEIGH_SCALE_HEIGHT 7994.0f
#endif
#ifndef MIE_SCALE_HEIGHT
    #define MIE_SCALE_HEIGHT 5000.0f
#endif
// static constexpr float mieScaleHeight = 5000.0f;
// static constexpr float mieScaleHeight = 1200.0f;

#ifndef SUN_INTENSITY
    #define SUN_INTENSITY 20.0f
#endif
#ifndef RAYLEIGH_SCATTERING
    #define RAYLEIGH_SCATTERING float3(15e-6f, 15e-6f, 15e-6f)
#endif
#ifndef MIE_SCATTERING
    #define MIE_SCATTERING float3(70e-6f, 50e-6f, 35e-6f)
#endif
#ifndef MIE_ABSORPTION
    #define MIE_ABSORPTION float3(5.8e-6f, 13e-6f, 22.4e-6f)
#endif
// static constexpr float3 rayleighScattering = { 15e-6f, 15e-6f, 15e-6f };
// static constexpr float3 rayleighScattering = { 5.8e-6f, 13e-6f, 22.4e-6f };
// static constexpr float3 mieScattering = { 70e-6f, 50e-6f, 35e-6f };
// static constexpr float3 mieScattering = { 20e-6f, 20e-6f, 20e-6f };
// static constexpr float3 mieAbsorption = { 8e-5f, 15e-5f, 25e-5f };
// static constexpr float3 mieAbsorption = { 22e-6f, 22e-6f, 22e-6f };
// static constexpr float3 mieAbsorption = { 0.0f, 0.0f, 0.0f };

float signPow(float x, float power)
{
    return sign(x) * pow(abs(x), power);
}

float sampleRayleighDensity(float sampleHeight)
{
    return min(exp(-sampleHeight / RAYLEIGH_SCALE_HEIGHT), 1000000.0f);
}

float sampleMieDensity(float sampleHeight)
{
    return min(exp(-sampleHeight / MIE_SCALE_HEIGHT), 1000000.0f);
}

[shader("compute")]
[numthreads(4, 4, 4)]
void compute(uint3 dispatchThreadID: SV_DispatchThreadID)
{
    float rayZenithCos = signPow(float(dispatchThreadID.x) / float(RAY_POINTS) * 2.0f - 1.0f, 1.0f / 0.5f);
    float sunZenithCos = signPow(float(dispatchThreadID.y) / float(SUN_POINTS) * 2.0f - 1.0f, 1.0f / 0.5f);
    float height = pow(float(dispatchThreadID.z) / float(HEIGHT_POINTS), 2.0f) * ATMOSPHERE_RANGE + ATMOSPHERE_START;

    float3 rayDirection = { sqrt(1.0f - rayZenithCos * rayZenithCos), rayZenithCos, 0.0f };
    float3 rayOrigin = { 0.0f, height, 0.0f };
    float3 sunDirection = { sqrt(1.0f - sunZenithCos * sunZenithCos), sunZenithCos, 0.0f };

    float A = dot(rayOrigin, rayDirection);
    float B = dot(rayOrigin, rayOrigin) - ATMOSPHERE_END_SQUARED;
    float C = A * A - B;

    if (C < 0.0f) {
        outputImage[dispatchThreadID] = {};
        return;
    }

    float atmosphereFarIntersection = -A + sqrt(C);
    float segmentLength = atmosphereFarIntersection / float(VIEW_SAMPLES);

    float3 sumR = {};
    float3 sumM = {};

    float opticalDepthR = {};
    float opticalDepthM = {};

    for (uint i = {}; i < VIEW_SAMPLES; ++i) {
        float sampleDistance = segmentLength * (float(i) + 0.5f);
        float3 samplePosition = rayOrigin + rayDirection * sampleDistance;
        float sampleHeight = length(samplePosition);
        float sampleAtmosphereHeight = sampleHeight - SEA_LEVEL;

        float rayleighDensity = sampleRayleighDensity(sampleAtmosphereHeight);
        float mieDensity = sampleMieDensity(sampleAtmosphereHeight);

        float Hr = rayleighDensity * segmentLength;
        float Hm = mieDensity * segmentLength;

        opticalDepthR += Hr;
        opticalDepthM += Hm;

        float opticalDepthLR = {};
        float opticalDepthLM = {};

        float A2 = dot(samplePosition, sunDirection);
        float B2 = dot(samplePosition, samplePosition) - ATMOSPHERE_END_SQUARED;
        float C2 = A2 * A2 - B2;

        if (C2 < 0.0f) {
            continue;
        }

        float atmosphereFarIntersection2 = -A2 + sqrt(C2);
        float segmentLength2 = atmosphereFarIntersection2 / float(LIGHT_SAMPLES);

        for (uint j = {}; j < LIGHT_SAMPLES; ++j) {
            float sampleDistance2 = segmentLength2 * (float(j) + 0.5f);
            float3 samplePosition2 = samplePosition + sunDirection * sampleDistance2;
            float sampleHeight2 = length(samplePosition2);
            float sampleAtmosphereHeight2 = sampleHeight2 - SEA_LEVEL;

            opticalDepthLR += sampleRayleighDensity(sampleAtmosphereHeight2) * segmentLength2;
            opticalDepthLM += sampleMieDensity(sampleAtmosphereHeight2) * segmentLength2;
        }

        float3 tau = RAYLEIGH_SCATTERING * (opticalDepthR + opticalDepthLR) + (MIE_SCATTERING + MIE_ABSORPTION) * (opticalDepthM + opticalDepthLM);

        float3 attenuation = exp(-tau);

        sumR += attenuation * Hr;
        sumM += attenuation * Hm;
    }

    float3 inscattering = (sumR * RAYLEIGH_SCATTERING + sumM * MIE_SCATTERING) * SUN_INTENSITY;

    outputImage[dispatchThreadID] = { inscattering };
}
