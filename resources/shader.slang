[[vk::binding(0, 0)]]
RWTexture3D<float3> outputImage;

static constexpr float RAY_POINTS = 128;
static constexpr float SUN_POINTS = 96;
static constexpr float HEIGHT_POINTS = 48;

static constexpr float safeRadius = 6345e3f;
static constexpr float earthRadius = 6360e3f;
static constexpr float atmosphereRadius = 6460e3f;

static constexpr float rayleighScaleHeight = 7994.0f;
static constexpr float mieScaleHeight = 5000.0f;

static constexpr float3 rayleighScattering = { 15e-6f, 15e-6f, 15e-6f };
static constexpr float3 mieScattering = { 70e-6f, 50e-6f, 35e-6f };
static constexpr float3 mieAbsorption = { 8e-5f, 15e-5f, 25e-5f };

static constexpr uint viewSegments = 10000u;
static constexpr uint lightSegments = 500u;

static constexpr uint viewSamples = viewSegments + 1u;
static constexpr uint lightSamples = lightSegments + 1u;
static constexpr float atmosphereRadiusSquared = atmosphereRadius * atmosphereRadius;
static constexpr float atmosphereRange = atmosphereRadius - safeRadius;

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
    float rayZenithCos = signPow(dispatchThreadID.x / RAY_POINTS * 2.0f - 1.0f, 1.0f / 0.5f);
    float sunZenithCos = signPow(dispatchThreadID.y / SUN_POINTS * 2.0f - 1.0f, 1.0f / 0.5f);
    float height = pow(dispatchThreadID.z / float(HEIGHT_POINTS), 2.0f) * atmosphereRange + safeRadius;

    float3 rayDirection = { sqrt(1.0f - rayZenithCos * rayZenithCos), rayZenithCos, 0.0f };
    float3 rayOrigin = { 0.0f, height, 0.0f };
    float3 sunDirection = { sqrt(1.0f - sunZenithCos * sunZenithCos), sunZenithCos, 0.0f };

    float A = dot(rayOrigin, rayDirection);
    float B = dot(rayOrigin, rayOrigin) - atmosphereRadiusSquared;
    float C = A * A - B;

    if (C < 0.0f) {
        outputImage[dispatchThreadID] = {};
        return;
    }

    float atmosphereFarIntersection = -A + sqrt(C);
    float segmentLength = atmosphereFarIntersection / float(viewSegments);

    float3 sumR = {};
    float3 sumM = {};

    float opticalDepthR = {};
    float opticalDepthM = {};

    for (uint i = {}; i < viewSamples; ++i) {
        float sampleDistance = segmentLength * float(i);
        float3 samplePosition = rayOrigin + rayDirection * sampleDistance;
        float sampleHeight = length(samplePosition);
        float sampleAtmosphereHeight = sampleHeight - earthRadius;

        float rayleighDensity = sampleRayleighDensity(sampleAtmosphereHeight);
        float mieDensity = sampleMieDensity(sampleAtmosphereHeight);

        float Hr = rayleighDensity * segmentLength;
        float Hm = mieDensity * segmentLength;

        opticalDepthR += Hr;
        opticalDepthM += Hm;

        float opticalDepthLR = {};
        float opticalDepthLM = {};

        float A2 = dot(samplePosition, sunDirection);
        float B2 = dot(samplePosition, samplePosition) - atmosphereRadiusSquared;
        float C2 = A2 * A2 - B2;

        if (C2 < 0.0f) {
            continue;
        }

        float atmosphereFarIntersection2 = -A2 + sqrt(C2);
        float segmentLength2 = atmosphereFarIntersection2 / float(lightSegments);

        for (uint j = {}; j < lightSamples; ++j) {
            float sampleDistance2 = segmentLength2 * float(j);
            float3 samplePosition2 = samplePosition + sunDirection * sampleDistance2;
            float sampleHeight2 = length(samplePosition2);
            float sampleAtmosphereHeight2 = sampleHeight2 - earthRadius;

            opticalDepthLR += sampleRayleighDensity(sampleAtmosphereHeight2) * segmentLength2;
            opticalDepthLM += sampleMieDensity(sampleAtmosphereHeight2) * segmentLength2;
        }

        float3 tau = rayleighScattering * (opticalDepthR + opticalDepthLR) + (mieScattering + mieAbsorption) * (opticalDepthM + opticalDepthLM);

        float3 attenuation = exp(-tau);

        sumR += attenuation * Hr;
        sumM += attenuation * Hm;
    }

    float3 inscattering = (sumR * rayleighScattering + sumM * mieScattering);

    outputImage[dispatchThreadID] = { inscattering };
}
