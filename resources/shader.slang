static constexpr float INF = 1.0f / 0.0f;
static constexpr float PI = 3.14159265358979323846f;

struct Payload {
    float3 color;
    float hitT;
};

struct SceneUniformData {
    float4 cameraRotation_r0;
    float4 cameraRotation_r1;
    float4 cameraRotation_r2;
    float4 cameraPosition_padd;
    uint2 windowDimensions;
    float focalDistance;
    uint frameCounter;
};

[[vk::binding(0, 0)]]
RWTexture2D<float4> resultImage;

[[vk::binding(0, 1)]]
cbuffer sceneUniformBuffer
{
    SceneUniformData sceneData;
}

[[vk::binding(1, 1)]]
RaytracingAccelerationStructure accelerationStructure;

///*
// earth preset
static constexpr float Intensity = 20.0f;
static constexpr float safeRadius = 6359e3f;
static constexpr float earthRadius = 6360e3f;
static constexpr float atmosphereRadius = 6460e3f;
// static constexpr float rayleighScaleHeight = 7994.0f;
// static constexpr float mieScaleHeight = 1200.0f;
// static constexpr float mieAnisotropy = 0.76f;
static constexpr float rayleighScaleHeight = 7994.0f;
static constexpr float mieScaleHeight = 5000.0f;
// static constexpr float rayleighScaleHeight = 26000.0f;
// static constexpr float mieScaleHeight = 22000.0f;
static constexpr float mieAnisotropy = 0.88f;
// static constexpr float3 BetaRScattering = { 5.8e-6f, 13.0e-6f, 22.4e-6f };
// static constexpr float3 BetaMScattering = { 20e-6f, 20e-6f, 20e-6f };
static constexpr float3 BetaRScattering = { 15e-6f, 15e-6f, 15e-6f };
static constexpr float3 BetaMScattering = { 70e-6f, 50e-6f, 35e-6f };
// static constexpr float3 BetaMAbsorption = { 0.0f, 0.0f, 0.0f };
static constexpr float3 BetaMAbsorption = { 8e-5f, 15e-5f, 25e-5f };
//*/

static constexpr uint segmentCount = 10;
static constexpr uint segmentCountL = 4;

static constexpr float safeRadiusSquared = safeRadius * safeRadius;
static constexpr float earthRadiusSquared = earthRadius * earthRadius;
static constexpr float atmosphereRadiusSquared = atmosphereRadius * atmosphereRadius;
static constexpr float mieAnisotropySquared = mieAnisotropy * mieAnisotropy;
static constexpr uint sampleCount = segmentCount + 1u;
static constexpr uint sampleCountL = segmentCountL + 1u;

float sampleDensityR(float sampleHeight)
{
    return min(exp(-sampleHeight / rayleighScaleHeight), 1000.0f);
}

float sampleDensityM(float sampleHeight)
{
    return min(exp(-sampleHeight / mieScaleHeight), 1000.0f);
}

float3 atmosphericScattering(float3 radiance, float hitT, float3 rayOrigin, float3 rayDirection)
{
    float Height = length(rayOrigin) + earthRadius;

    float3 sunVector = { 0.0f, 1.0f, 0.0f };
    float sunDot = clamp(dot(rayDirection, sunVector), 0.0f, 1.0f);

    float sphereDot = dot(rayOrigin, rayDirection);

    float3 sphereFactor = rayOrigin - sphereDot * rayDirection;
    float sphereFactorDot = dot(sphereFactor, sphereFactor);

    float3 rayStartPosition;
    float rayStart;
    float rayEnd;

    float atmosphereDisc = atmosphereRadiusSquared - sphereFactorDot;
    if (atmosphereDisc < 0.0f) {
        return radiance;
    }

    float atmosphereIntersection = sqrt(atmosphereDisc);
    float atmosphereFarIntersection = -sphereDot + atmosphereIntersection;

    if (atmosphereIntersection >= 0.0f && atmosphereFarIntersection >= 0.0f) {
        float atmosphereNearIntersection = -sphereDot - atmosphereIntersection;
        if (atmosphereNearIntersection < 0.0f) {
            rayStart = 0.0f;
            rayStartPosition = rayOrigin;
        } else {
            if (atmosphereNearIntersection >= hitT) {
                return radiance;
            }

            rayStart = atmosphereNearIntersection;
            rayStartPosition = rayOrigin + rayDirection * atmosphereNearIntersection;
        }
    } else {
        return radiance;
    }

    float groundDisc = safeRadiusSquared - sphereFactorDot;
    float groundIntersection = groundDisc >= 0.0f ? sqrt(groundDisc) : 0.0f;
    float groundFarIntersection = -sphereDot + groundIntersection;
    float groundNearIntersection = -sphereDot - groundIntersection;

    bool hasGroundIntersection;
    if (groundDisc >= 0.0f && groundFarIntersection >= 0.0f && groundNearIntersection >= 0.0f) {
        if (groundNearIntersection < hitT) {
            hasGroundIntersection = true;
            rayEnd = groundNearIntersection - rayStart;
        } else {
            hasGroundIntersection = false;
            rayEnd = hitT - rayStart;
        }
    } else {
        hasGroundIntersection = false;
        if (atmosphereFarIntersection < hitT) {
            rayEnd = atmosphereFarIntersection - rayStart;
        } else {
            rayEnd = hitT - rayStart;
        }
    }

    float mu = dot(rayDirection, sunVector);
    float muSquared = mu * mu;
    float phaseR = (3.0f / (16.0f * PI)) * (1.0f + muSquared);
    float phaseM = (3.0f / (8.0f * PI)) * ((1.0f - mieAnisotropySquared) * (1.0f + muSquared) / ((2.0f + mieAnisotropySquared) * pow(1.0f + mieAnisotropySquared - 2.0f * mieAnisotropy * mu, 1.5f)));

    float3 SumRayleigh = {};
    float3 SumMie = {};

    float segmentLength = rayEnd / float(segmentCount);

    float opticalDepthR = 0.0f;
    float opticalDepthM = 0.0f;

    for (uint i = 0; i < sampleCount; ++i) {
        float sampleDistance = segmentLength * float(i);
        float3 samplePoint = rayStartPosition + rayDirection * sampleDistance;
        float sampleHeight = length(samplePoint) - earthRadius;

        float Hr_sample = sampleDensityR(sampleHeight) * segmentLength;
        float Hm_sample = sampleDensityM(sampleHeight) * segmentLength;

        opticalDepthR += Hr_sample;
        opticalDepthM += Hm_sample;

        float opticalDepthLR = {};
        float opticalDepthLM = {};

        float sampleSphereDot = dot(samplePoint, sunVector);
        float3 sampleSphereFactor = samplePoint - sampleSphereDot * sunVector;
        float sampleSphereFactorDot = dot(sampleSphereFactor, sampleSphereFactor);

        float sampleGroundDisc = safeRadiusSquared - sampleSphereFactorDot;
        float sampleGroundIntersection = sampleGroundDisc >= 0.0f ? sqrt(sampleGroundDisc) : 0.0f;
        float sampleGroundFarIntersection = -sampleSphereDot + sampleGroundIntersection;
        float sampleGroundNearIntersection = -sampleSphereDot - sampleGroundIntersection;

        if (!(sampleGroundDisc >= 0.0f && sampleGroundFarIntersection >= 0.0f && sampleGroundNearIntersection >= 0.0f)) {
            float sampleAtmosphereDisc = atmosphereRadiusSquared - sampleSphereFactorDot;
            if (sampleAtmosphereDisc >= 0.0f) {
                float sampleAtmosphereIntersection = sqrt(sampleAtmosphereDisc);
                float sampleAtmosphereFarIntersection = -sampleSphereDot + sampleAtmosphereIntersection;
                float segmentLengthL = sampleAtmosphereFarIntersection / float(segmentCountL);

                for (uint j = 0; j < sampleCountL; ++j) {
                    float3 Pl = samplePoint + sunVector * (segmentLengthL * float(j));
                    float sampleHeightL = length(Pl) - earthRadius;

                    opticalDepthLR += sampleDensityR(sampleHeightL) * segmentLengthL;
                    opticalDepthLM += sampleDensityM(sampleHeightL) * segmentLengthL;
                }

                float3 tauR = (BetaRScattering) * (opticalDepthR + opticalDepthLR);
                float3 tauM = (BetaMScattering + BetaMAbsorption) * (opticalDepthM + opticalDepthLM);
                float3 tau = tauR + tauM;

                float3 attenuation = exp(-tau);

                SumRayleigh += Hr_sample * attenuation;
                SumMie += Hm_sample * attenuation;
            }
        }
    }

    float3 inscattering = (SumRayleigh * phaseR * BetaRScattering + SumMie * phaseM * BetaMScattering) * Intensity;

    if (hasGroundIntersection) {
        opticalDepthR = INF;
        opticalDepthM = INF;
    }

    float3 totalTau = BetaRScattering * opticalDepthR + (BetaMScattering + BetaMAbsorption) * opticalDepthM;

    float3 transmittance = exp(-totalTau);

    return inscattering + radiance * transmittance;
}

/*
float ApproxOpticalDepth(float r, float mu, float H)
{
    float rho = exp(-(r - earthRadius) / H);

    float disc = atmosphereRadiusSquared - r * r * (1.0f - mu * mu);
    if (disc <= 0.0f) {
        return 0.0f;
    }

    float sMax = -r * mu + sqrt(disc);
    float x = r / H;
    float h = sqrt(2.0f / x);
    float oneMinusMu = 1.0f - mu;

    float num = 1.0f + 0.399f * oneMinusMu + 0.046f * oneMinusMu * oneMinusMu;
    float den = mu + h + 0.301f * oneMinusMu + 0.017f * oneMinusMu * oneMinusMu;

    return min(H * rho * (num / den), 10000.0f);
}
*/

static constexpr float3x3 firstMat = {
    0.842479062253094, 0.0423282422610123, 0.0423756549057051,
    0.0784335999999992, 0.878468636469772, 0.0784336,
    0.0792237451477643, 0.0791661274605434, 0.879142973793104
};
static constexpr float minEv = -12.47393f;
static constexpr float maxEv = 4.026069f;
static constexpr float3x3 secondMat = {
    1.19687900512017, -0.0528968517574562, -0.0529716355144438,
    -0.0980208811401368, 1.15190312990417, -0.0980434501171241,
    -0.0990297440797205, -0.0989611768448433, 1.15107367264116
};

float3 adjustSaturation(float3 color, float saturation)
{
    float luma = dot(color, float3(0.2126, 0.7152, 0.0722)); // Rec.709
    return lerp(luma.xxx, color, saturation);
}

float3 agxTonemapping(float3 radiance)
{
    radiance = mul(firstMat, radiance);

    radiance = clamp(log2(radiance), minEv, maxEv);
    radiance = (radiance - minEv) / (maxEv - minEv);

    radiance = fma(radiance, fma(radiance, fma(radiance, fma(radiance, fma(radiance, fma(radiance, 15.5f, -40.14f), 31.96f), -6.868f), 0.4298f), 0.1191f), -0.00232f);

    radiance = mul(secondMat, radiance);

    return radiance;
}

// Interleaved gradient noise (Jimenez 2014)
static constexpr float2 dotComponenet = { 0.06711056f, 0.00583715f };
float interleavedGradientNoise(uint2 pixelId)
{
    return frac(52.9829189f * frac(dot(float2(pixelId), dotComponenet))) - 0.5f;
}

[shader("raygeneration")]
void raygeneration()
{
    uint2 pixelId = DispatchRaysIndex().xy;

    float3 rayDirection = normalize(mul(
        float3x3(
            sceneData.cameraRotation_r0.xyz,
            sceneData.cameraRotation_r1.xyz,
            sceneData.cameraRotation_r2.xyz,
        ),
        float3(
            (float(pixelId.x) - float(sceneData.windowDimensions.x >> 1)) / float(sceneData.windowDimensions.x),
            -(float(pixelId.y) - float(sceneData.windowDimensions.y >> 1)) / float(sceneData.windowDimensions.x),
            sceneData.focalDistance,
        ),
    ));

    float3 rayOrigin = sceneData.cameraPosition_padd.xyz;

    Payload payload;

    RayDesc ray = RayDesc(rayOrigin, 0.0f, rayDirection, INF);

    TraceRay(accelerationStructure, RAY_FLAG_FORCE_OPAQUE, 0xFF, 0, 0, 0, ray, payload);

    float3 color = payload.color;

    color = atmosphericScattering(color, payload.hitT, rayOrigin + float3(0.0f, earthRadius + 100.0f, 0.0f), rayDirection);

    color = agxTonemapping(color);

    color += interleavedGradientNoise(pixelId) * 0.0039215686274509803921568627450980f;

    resultImage[pixelId] = float4(color, 1.0f);
}

[shader("miss")]
void miss(inout Payload payload)
{
    float3 sunVector = float3(0.0f, 1.0f, 0.0f);
    float sunDot = clamp(dot(WorldRayDirection(), sunVector), 0.0f, 1.0f);
    payload.color = float3(50.0f, 40.0f, 30.0f) * smoothstep(0.9995f, 0.9999f, sunDot);
    payload.hitT = INF;
}

struct AABB {
    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;
};

[[vk::binding(0, 2)]]
StructuredBuffer<AABB> GeometryBuffers[];

[shader("intersection")]
void intersection()
{
    AABB geometry = GeometryBuffers[InstanceIndex()][PrimitiveIndex()];

    float3 rayOrigin = ObjectRayOrigin();
    float3 rayDir = ObjectRayDirection();

    float3 inverseDirection = 1.0 / rayDir;

    float3 t0 = (float3(geometry.minX, geometry.minY, geometry.minZ) - rayOrigin) * inverseDirection;
    float3 t1 = (float3(geometry.maxX, geometry.maxY, geometry.maxZ) - rayOrigin) * inverseDirection;

    float3 tmin3 = min(t0, t1);
    float3 tmax3 = max(t0, t1);

    float tEnter = max(max(tmin3.x, tmin3.y), tmin3.z);
    float tExit = min(min(tmax3.x, tmax3.y), tmax3.z);

    if (tExit >= tEnter && tExit >= 0.0) {
        ReportHit<int>(max(tEnter, 0.0f), 0u, 0);
    }
}

uint hash32(uint x)
{
    x += (x << 10u);
    x ^= (x >> 6u);
    x += (x << 3u);
    x ^= (x >> 11u);
    x += (x << 15u);
    return x;
}

float3 hashColor(uint id)
{
    const float invMax = 1.0 / 4294967295.0;

    uint h = hash32(id);
    float r = float(hash32(h ^ 0xf8e21da4u)) * invMax;
    float g = float(hash32(h ^ 0xf5297a4du)) * invMax;
    float b = float(hash32(h ^ 0xfb53c4e1u)) * invMax;

    return float3(r, g, b);
}

[shader("closesthit")]
void closesthit(inout Payload payload)
{
    uint primID = PrimitiveIndex();
    uint instID = InstanceID();

    payload.color = hashColor(primID ^ instID);
    payload.hitT = RayTCurrent();
}

