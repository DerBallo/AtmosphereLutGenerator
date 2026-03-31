[[vk::binding(0, 0)]]
RWTexture3D<float3> outputImage;

#ifndef RAY_POINTS
    #define RAY_POINTS 128
#endif
static constexpr uint rayPoints = RAY_POINTS;
#ifndef SUN_POINTS
    #define SUN_POINTS 96
#endif
static constexpr uint sunPoints = SUN_POINTS;
#ifndef HEIGHT_POINTS
    #define HEIGHT_POINTS 48
#endif
static constexpr uint heightPoints = HEIGHT_POINTS;

#ifndef VIEW_SAMPLES
    #define VIEW_SAMPLES 1000
#endif
static constexpr uint viewSamples = VIEW_SAMPLES;
#ifndef LIGHT_SAMPLES
    #define LIGHT_SAMPLES 100
#endif
static constexpr uint lightSamples = LIGHT_SAMPLES;

#ifndef ATMOSPHERE_START
    #define ATMOSPHERE_START 6345e3f
#endif
static constexpr float atmosphereStart = ATMOSPHERE_START;
#ifndef SEA_LEVEL
    #define SEA_LEVEL 6360e3f
#endif
static constexpr float seaLevel = SEA_LEVEL;
#ifndef ATMOSPHERE_END
    #define ATMOSPHERE_END 6460e3f
#endif
static constexpr float atmosphereEnd = ATMOSPHERE_END;
static constexpr float atmosphereEndSquared = ATMOSPHERE_END * ATMOSPHERE_END;
static constexpr float atmosphereRange = ATMOSPHERE_END - ATMOSPHERE_START;

#ifndef RAYLEIGH_SCALE_HEIGHT
    #define RAYLEIGH_SCALE_HEIGHT 7994.0f
#endif
static constexpr float rayleighScaleHeight = RAYLEIGH_SCALE_HEIGHT;
#ifndef MIE_SCALE_HEIGHT
    #define MIE_SCALE_HEIGHT 5000.0f
#endif
static constexpr float mieScaleHeight = MIE_SCALE_HEIGHT;
// static constexpr float mieScaleHeight = 1200.0f;

#ifndef SUN_INTENSITY
    #define SUN_INTENSITY 20.0f
#endif
static constexpr float sunIntensity = SUN_INTENSITY;
#ifndef RAYLEIGH_SCATTERING_RED
    #define RAYLEIGH_SCATTERING_RED 15e-6f
#endif
#ifndef RAYLEIGH_SCATTERING_GREEN
    #define RAYLEIGH_SCATTERING_GREEN 15e-6f
#endif
#ifndef RAYLEIGH_SCATTERING_BLUE
    #define RAYLEIGH_SCATTERING_BLUE 15e-6f
#endif
static constexpr float3 rayleighScattering = {
    RAYLEIGH_SCATTERING_RED,
    RAYLEIGH_SCATTERING_GREEN,
    RAYLEIGH_SCATTERING_BLUE
};
#ifndef MIE_SCATTERING_RED
    #define MIE_SCATTERING_RED 70e-6f
#endif
#ifndef MIE_SCATTERING_GREEN
    #define MIE_SCATTERING_GREEN 50e-6f
#endif
#ifndef MIE_SCATTERING_BLUE
    #define MIE_SCATTERING_BLUE 35e-6f
#endif
static constexpr float3 mieScattering = {
    MIE_SCATTERING_RED,
    MIE_SCATTERING_GREEN,
    MIE_SCATTERING_BLUE
};
#ifndef MIE_ABSORPTION_RED
    #define MIE_ABSORPTION_RED 5.8e-6f
#endif
#ifndef MIE_ABSORPTION_GREEN
    #define MIE_ABSORPTION_GREEN 13e-6f
#endif
#ifndef MIE_ABSORPTION_BLUE
    #define MIE_ABSORPTION_BLUE 22.4e-6f
#endif
static constexpr float3 mieAbsorption = {
    MIE_ABSORPTION_RED,
    MIE_ABSORPTION_GREEN,
    MIE_ABSORPTION_BLUE
};
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
    return min(exp(-sampleHeight / rayleighScaleHeight), 1000000.0f);
}

float sampleMieDensity(float sampleHeight)
{
    return min(exp(-sampleHeight / mieScaleHeight), 1000000.0f);
}

[shader("compute")]
[numthreads(4, 4, 4)]
void compute(uint3 dispatchThreadID: SV_DispatchThreadID)
{
    float rayZenithCos = signPow(float(dispatchThreadID.x) / float(rayPoints) * 2.0f - 1.0f, 1.0f / 0.5f);
    float sunZenithCos = signPow(float(dispatchThreadID.y) / float(sunPoints) * 2.0f - 1.0f, 1.0f / 0.5f);
    float height = pow(float(dispatchThreadID.z) / float(heightPoints), 2.0f) * atmosphereRange + atmosphereStart;

    float3 rayDirection = { 0.0f, rayZenithCos, sqrt(1.0f - rayZenithCos * rayZenithCos) };
    float3 rayOrigin = { 0.0f, height, 0.0f };
    float3 sunDirection = { sqrt(1.0f - sunZenithCos * sunZenithCos), sunZenithCos, 0.0f };

    float A = dot(rayOrigin, rayDirection);
    float B = dot(rayOrigin, rayOrigin) - atmosphereEndSquared;
    float C = A * A - B;

    if (C < 0.0f) {
        outputImage[dispatchThreadID] = {};
        return;
    }

    float atmosphereFarIntersection = -A + sqrt(C);
    float segmentLength = atmosphereFarIntersection / float(viewSamples);

    float3 sumR = {};
    float3 sumM = {};

    float opticalDepthR = {};
    float opticalDepthM = {};

    for (uint i = {}; i < viewSamples; ++i) {
        float sampleDistance = segmentLength * (float(i) + 0.5f);
        float3 samplePosition = rayOrigin + rayDirection * sampleDistance;
        float sampleHeight = length(samplePosition);
        float sampleAtmosphereHeight = sampleHeight - seaLevel;

        float rayleighDensity = sampleRayleighDensity(sampleAtmosphereHeight);
        float mieDensity = sampleMieDensity(sampleAtmosphereHeight);

        float Hr = rayleighDensity * segmentLength;
        float Hm = mieDensity * segmentLength;

        opticalDepthR += Hr;
        opticalDepthM += Hm;

        float opticalDepthLR = {};
        float opticalDepthLM = {};

        float A2 = dot(samplePosition, sunDirection);
        float B2 = dot(samplePosition, samplePosition) - atmosphereEndSquared;
        float C2 = A2 * A2 - B2;

        if (C2 < 0.0f) {
            continue;
        }

        float atmosphereFarIntersection2 = -A2 + sqrt(C2);
        float segmentLength2 = atmosphereFarIntersection2 / float(lightSamples);

        for (uint j = {}; j < lightSamples; ++j) {
            float sampleDistance2 = segmentLength2 * (float(j) + 0.5f);
            float3 samplePosition2 = samplePosition + sunDirection * sampleDistance2;
            float sampleHeight2 = length(samplePosition2);
            float sampleAtmosphereHeight2 = sampleHeight2 - seaLevel;

            opticalDepthLR += sampleRayleighDensity(sampleAtmosphereHeight2) * segmentLength2;
            opticalDepthLM += sampleMieDensity(sampleAtmosphereHeight2) * segmentLength2;
        }

        float3 tau = rayleighScattering * (opticalDepthR + opticalDepthLR) + (mieScattering + mieAbsorption) * (opticalDepthM + opticalDepthLM);

        float3 attenuation = exp(-tau);

        sumR += attenuation * Hr;
        sumM += attenuation * Hm;
    }

    float3 inscattering = (sumR * rayleighScattering + sumM * mieScattering) * sunIntensity;

    outputImage[dispatchThreadID] = { inscattering };
}
