[[vk::binding(0, 0)]]
RWTexture2D<float4> transmittanceImage;

[[vk::binding(1, 0)]]
Sampler2D transmittanceSampler;

[[vk::binding(2, 0)]]
RWTexture3D<float4> inscatteringImage;

// ======== Transmittance ========
// TRANSMITTANCE_RAY_POINTS
#ifndef TRANSMITTANCE_RAY_POINTS
    #warning "TRANSMITTANCE_RAY_POINTS should be defined"
    #define TRANSMITTANCE_RAY_POINTS 0
#endif
static constexpr uint transmittanceRayPoints = uint(TRANSMITTANCE_RAY_POINTS);

// TRANSMITTANCE_RAY_POINTS_CURVE
#ifndef TRANSMITTANCE_RAY_POINTS_CURVE
    #warning "TRANSMITTANCE_RAY_POINTS_CURVE should be defined"
    #define TRANSMITTANCE_RAY_POINTS_CURVE 0
#endif
static constexpr float transmittanceRayPointsCurve = float(TRANSMITTANCE_RAY_POINTS_CURVE);

// TRANSMITTANCE_HEIGHT_POINTS
#ifndef TRANSMITTANCE_HEIGHT_POINTS
    #warning "TRANSMITTANCE_HEIGHT_POINTS should be defined"
    #define TRANSMITTANCE_HEIGHT_POINTS 0
#endif
static constexpr uint transmittanceHeightPoints = uint(TRANSMITTANCE_HEIGHT_POINTS);

// TRANSMITTANCE_HEIGHT_POINTS_CURVE
#ifndef TRANSMITTANCE_HEIGHT_POINTS_CURVE
    #warning "TRANSMITTANCE_HEIGHT_POINTS_CURVE should be defined"
    #define TRANSMITTANCE_HEIGHT_POINTS_CURVE 0
#endif
static constexpr float transmittanceHeightPointsCurve = float(TRANSMITTANCE_HEIGHT_POINTS_CURVE);

// TRANSMITTANCE_SAMPLES
#ifndef TRANSMITTANCE_SAMPLES
    #warning "TRANSMITTANCE_SAMPLES should be defined"
    #define TRANSMITTANCE_SAMPLES 0
#endif
static constexpr uint transmittanceSamples = uint(TRANSMITTANCE_SAMPLES);

// ======== Inscattering ========
// INSCATTERING_RAY_POINTS
#ifndef INSCATTERING_RAY_POINTS
    #warning "INSCATTERING_RAY_POINTS should be defined"
    #define INSCATTERING_RAY_POINTS 0
#endif
static constexpr uint inscatteringRayPoints = uint(INSCATTERING_RAY_POINTS);

// INSCATTERING_RAY_POINTS_CURVE
#ifndef INSCATTERING_RAY_POINTS_CURVE
    #warning "INSCATTERING_RAY_POINTS_CURVE should be defined"
    #define INSCATTERING_RAY_POINTS_CURVE 0
#endif
static constexpr float inscatteringRayPointsCurve = float(INSCATTERING_RAY_POINTS_CURVE);

// INSCATTERING_SUN_POINTS
#ifndef INSCATTERING_SUN_POINTS
    #warning "INSCATTERING_SUN_POINTS should be defined"
    #define INSCATTERING_SUN_POINTS 0
#endif
static constexpr uint inscatteringSunPoints = uint(INSCATTERING_SUN_POINTS);

// INSCATTERING_SUN_POINTS_CURVE
#ifndef INSCATTERING_SUN_POINTS_CURVE
    #warning "INSCATTERING_SUN_POINTS_CURVE should be defined"
    #define INSCATTERING_SUN_POINTS_CURVE 0
#endif
static constexpr float inscatteringSunPointsCurve = float(INSCATTERING_SUN_POINTS_CURVE);

// INSCATTERING_HEIGHT_POINTS
#ifndef INSCATTERING_HEIGHT_POINTS
    #warning "INSCATTERING_HEIGHT_POINTS should be defined"
    #define INSCATTERING_HEIGHT_POINTS 0
#endif
static constexpr uint inscatteringHeightPoints = uint(INSCATTERING_HEIGHT_POINTS);

// INSCATTERING_HEIGHT_POINTS_CURVE
#ifndef INSCATTERING_HEIGHT_POINTS_CURVE
    #warning "INSCATTERING_HEIGHT_POINTS_CURVE should be defined"
    #define INSCATTERING_HEIGHT_POINTS_CURVE 0
#endif
static constexpr float inscatteringHeightPointsCurve = float(INSCATTERING_HEIGHT_POINTS_CURVE);

// INSCATTERING_SAMPLES
#ifndef INSCATTERING_SAMPLES
    #warning "INSCATTERING_SAMPLES should be defined"
    #define INSCATTERING_SAMPLES 0
#endif
static constexpr uint inscatteringSamples = uint(INSCATTERING_SAMPLES);

// ======== Atmosphere dimensions ========
// ATMOSPHERE_START
#ifndef ATMOSPHERE_START
    #warning "ATMOSPHERE_START should be defined"
    #define ATMOSPHERE_START 0
#endif
static constexpr float atmosphereStart = float(ATMOSPHERE_START);

// SEA_LEVEL
#ifndef SEA_LEVEL
    #warning "SEA_LEVEL should be defined"
    #define SEA_LEVEL 0
#endif
static constexpr float seaLevel = float(SEA_LEVEL);

// ATMOSPHERE_END
#ifndef ATMOSPHERE_END
    #warning "ATMOSPHERE_END should be defined"
    #define ATMOSPHERE_END 0
#endif
static constexpr float atmosphereEnd = float(ATMOSPHERE_END);

static constexpr float atmosphereEndSquared = atmosphereEnd * atmosphereEnd;
static constexpr float atmosphereRange = atmosphereEnd - atmosphereStart;
static constexpr float inverseAtmosphereRange = 1.0f / atmosphereRange;

// ======== Scattering properties ========
// RAYLEIGH_SCALE_HEIGHT
#ifndef RAYLEIGH_SCALE_HEIGHT
    #warning "RAYLEIGH_SCALE_HEIGHT should be defined"
    #define RAYLEIGH_SCALE_HEIGHT 0
#endif
static constexpr float rayleighScaleHeight = float(RAYLEIGH_SCALE_HEIGHT);

// MIE_SCALE_HEIGHT
#ifndef MIE_SCALE_HEIGHT
    #warning "MIE_SCALE_HEIGHT should be defined"
    #define MIE_SCALE_HEIGHT 0
#endif
static constexpr float mieScaleHeight = float(MIE_SCALE_HEIGHT);

// ======== Scattering values ========
// RAYLEIGH_SCATTERING
#ifndef RAYLEIGH_SCATTERING_RED
    #warning "RAYLEIGH_SCATTERING_RED should be defined"
    #define RAYLEIGH_SCATTERING_RED 0
#endif
#ifndef RAYLEIGH_SCATTERING_GREEN
    #warning "RAYLEIGH_SCATTERING_GREEN should be defined"
    #define RAYLEIGH_SCATTERING_GREEN 0
#endif
#ifndef RAYLEIGH_SCATTERING_BLUE
    #warning "RAYLEIGH_SCATTERING_BLUE should be defined"
    #define RAYLEIGH_SCATTERING_BLUE 0
#endif
static constexpr float3 rayleighScattering = float3(
    RAYLEIGH_SCATTERING_RED,
    RAYLEIGH_SCATTERING_GREEN,
    RAYLEIGH_SCATTERING_BLUE
);

// MIE_SCATTERING
#ifndef MIE_SCATTERING_RED
    #warning "MIE_SCATTERING_RED should be defined"
    #define MIE_SCATTERING_RED 0
#endif
#ifndef MIE_SCATTERING_GREEN
    #warning "MIE_SCATTERING_GREEN should be defined"
    #define MIE_SCATTERING_GREEN 0
#endif
#ifndef MIE_SCATTERING_BLUE
    #warning "MIE_SCATTERING_BLUE should be defined"
    #define MIE_SCATTERING_BLUE 0
#endif
static constexpr float3 mieScattering = float3(
    MIE_SCATTERING_RED,
    MIE_SCATTERING_GREEN,
    MIE_SCATTERING_BLUE
);

// MIE_ABSORPTION
#ifndef MIE_ABSORPTION_RED
    #warning "MIE_ABSORPTION_RED should be defined"
    #define MIE_ABSORPTION_RED 0
#endif
#ifndef MIE_ABSORPTION_GREEN
    #warning "MIE_ABSORPTION_GREEN should be defined"
    #define MIE_ABSORPTION_GREEN 0
#endif
#ifndef MIE_ABSORPTION_BLUE
    #warning "MIE_ABSORPTION_BLUE should be defined"
    #define MIE_ABSORPTION_BLUE 0
#endif
static constexpr float3 mieAbsorption = float3(
    MIE_ABSORPTION_RED,
    MIE_ABSORPTION_GREEN,
    MIE_ABSORPTION_BLUE
);

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
[numthreads(4, 4, 1)]
void transmittance(uint3 dispatchThreadID: SV_DispatchThreadID)
{
    float rayZenithCos = signPow(float(dispatchThreadID.x) / float(transmittanceRayPoints) * 2.0f - 1.0f, 1.0f / transmittanceRayPointsCurve);
    float height = signPow(float(dispatchThreadID.y) / float(transmittanceHeightPoints), 1.0f / transmittanceHeightPointsCurve) * atmosphereRange + atmosphereStart;

    float3 rayDirection = { 0.0f, rayZenithCos, sqrt(1.0f - rayZenithCos * rayZenithCos) };
    float3 rayOrigin = { 0.0f, height, 0.0f };

    float opticalDepthLR = {};
    float opticalDepthLM = {};

    float A = dot(rayOrigin, rayDirection);
    float B = dot(rayOrigin, rayOrigin) - atmosphereEndSquared;
    float C = A * A - B;

    if (C < 0.0f) {
        transmittanceImage[dispatchThreadID.xy] = {};
        return;
    }

    float atmosphereFarIntersection = -A + sqrt(C);
    float segmentLength = atmosphereFarIntersection / float(transmittanceSamples);

    for (uint j = {}; j < transmittanceSamples; ++j) {
        float sampleDistance = segmentLength * (float(j) + 0.5f);
        float3 samplePosition = rayOrigin + rayDirection * sampleDistance;
        float sampleHeight = length(samplePosition);
        float sampleAtmosphereHeight = sampleHeight - seaLevel;

        opticalDepthLR += sampleRayleighDensity(sampleAtmosphereHeight) * segmentLength;
        opticalDepthLM += sampleMieDensity(sampleAtmosphereHeight) * segmentLength;
    }

    float3 tau = rayleighScattering * opticalDepthLR + (mieScattering + mieAbsorption) * opticalDepthLM;

    float3 attenuation = exp(-tau);

    transmittanceImage[dispatchThreadID.xy] = float4(attenuation, 1.0f);
}

[shader("compute")]
[numthreads(4, 4, 4)]
void inscattering(uint3 dispatchThreadID: SV_DispatchThreadID)
{
    float rayZenithCos = signPow(float(dispatchThreadID.x) / float(inscatteringRayPoints) * 2.0f - 1.0f, 1.0f / inscatteringRayPointsCurve);
    float sunZenithCos = signPow(float(dispatchThreadID.y) / float(inscatteringSunPoints) * 2.0f - 1.0f, 1.0f / inscatteringSunPointsCurve);
    float height = pow(float(dispatchThreadID.z) / float(inscatteringHeightPoints), 1.0f / inscatteringHeightPointsCurve) * atmosphereRange + atmosphereStart;

    float3 rayDirection = { sqrt(1.0f - rayZenithCos * rayZenithCos), rayZenithCos, 0.0f };
    float3 rayOrigin = { 0.0f, height, 0.0f };
    float3 sunDirection = { sqrt(1.0f - sunZenithCos * sunZenithCos), sunZenithCos, 0.0f };

    float A = dot(rayOrigin, rayDirection);
    float B = dot(rayOrigin, rayOrigin) - atmosphereEndSquared;
    float C = A * A - B;

    if (C < 0.0f) {
        inscatteringImage[dispatchThreadID] = {};
        return;
    }

    float atmosphereFarIntersection = -A + sqrt(C);
    float segmentLength = atmosphereFarIntersection / float(inscatteringSamples);

    float3 sumR = {};
    float3 sumM = {};

    float opticalDepthR = {};
    float opticalDepthM = {};

    for (uint i = {}; i < inscatteringSamples; ++i) {
        float sampleDistance = segmentLength * (float(i) + 0.5f);
        float3 samplePosition = rayOrigin + rayDirection * sampleDistance;
        float sampleHeight = length(samplePosition);
        float sampleAtmosphereHeight = sampleHeight - seaLevel;

        float Hr = sampleRayleighDensity(sampleAtmosphereHeight) * segmentLength;
        float Hm = sampleMieDensity(sampleAtmosphereHeight) * segmentLength;

        opticalDepthR += Hr;
        opticalDepthM += Hm;

        float3 tau_view = rayleighScattering * opticalDepthR + (mieScattering + mieAbsorption) * opticalDepthM;

        float3 attenuation_view = exp(-tau_view);

        float sampleSunZenithCos = dot(sunDirection, normalize(samplePosition));

        float2 uv = {
            signPow(sampleSunZenithCos, transmittanceRayPointsCurve) * 0.5f + 0.5f,
            signPow((sampleHeight - atmosphereStart) * inverseAtmosphereRange, transmittanceHeightPointsCurve)
        };

        float3 transmittance = transmittanceSampler.SampleLevel(uv, 0).xyz;

        float3 attenuation = attenuation_view * transmittance;

        sumR += attenuation * Hr;
        sumM += attenuation * Hm;
    }

    float3 inscattering = (sumR * rayleighScattering + sumM * mieScattering) * 8.0f;

    inscatteringImage[dispatchThreadID] = float4(inscattering, 1.0f);
}
