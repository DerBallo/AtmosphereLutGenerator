# AtmosphereLutGenerator
Generates a binary LUT for a vulkan atmosphere shader and saves it as a file.

## .atmosphere file data layout:
| Offset (bytes) | Size (bytes) | Type | Name |
|---|---|---|---|
| 0 | 4 | uint32 | magic number (0x41544D4F = 'ATMO') |
| 4 | 4 | uint32 | file format version |
| 8 | 4 | uint32 | transmittance image format |
| 12 | 4 | uint32 | transmittance bytes per pixel |
| 16 | 4 | uint32 | transmittance ray points |
| 20 | 4 | float | transmittance ray points curve |
| 24 | 4 | uint32 | transmittance height points |
| 28 | 4 | float | transmittance height points curve |
| 32 | 4 | uint32 | transmittance samples |
| 36 | 4 | uint32 | inscattering image format |
| 40 | 4 | uint32 | inscattering bytes per pixel |
| 44 | 4 | uint32 | inscattering ray points |
| 48 | 4 | float | inscattering ray points curve |
| 52 | 4 | uint32 | inscattering sun points |
| 56 | 4 | float | inscattering sun points curve |
| 60 | 4 | uint32 | inscattering height points |
| 64 | 4 | float | inscattering height points curve |
| 68 | 4 | uint32 | inscattering samples |
| 72 | 4 | float | atmosphere start |
| 76 | 4 | float | sea level |
| 80 | 4 | float | atmosphere end |
| 84 | 4 | float | rayleigh scale height |
| 88 | 4 | float | mie scale height |
| 92 | 4 | float | mie anisotropy |
| 96 | 4 | float | rayleigh scattering red |
| 100 | 4 | float | rayleigh scattering green |
| 104 | 4 | float | rayleigh scattering blu |
| 108 | 4 | float | mie scattering red |
| 112 | 4 | float | mie scattering green |
| 116 | 4 | float | mie scattering blue |
| 120 | 4 | float | mie absorption red |
| 124 | 4 | float | mie absorption green |
| 128 | 4 | float | mie absorption blue |
| 132 | padding | padding | reserved |
| 256 | transmittance bytes per pixel * transmittance ray points * transmittance height points | tightly packed pixel data | transmittance |
| transmittance bytes per pixel * transmittance ray points * transmittance height points | inscattering bytes per pixel * inscattering ray points * inscattering sun points * inscattering height points | tightly packed pixel data | inscattering |

## An example of how to load an atmosphere file:
You first need to create a vulkan host-mapped buffer and then load the entire file into it:
```cpp
std::ifstream atmosphereFile("path/to/atmosphere/file", std::ios::binary | std::ios::ate);
ptrdiff_t atmosphereLutSize = atmosphereFile.tellg();
atmosphereFile.seekg(0, std::ios::beg);
atmosphereFile.read(reinterpret_cast<char*>(mappedStagingBufferPointer), atmosphereLutSize);
```
Proceed by reading the file header:
```cpp
// check magic number (optional)
// check version (optional)

// Transmittance properties
VkFormat transmittanceImageFormat = static_cast<VkFormat>(reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[2]);
uint32_t transmittanceBytesPerPixel = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[3];
uint32_t transmittanceRayPoints = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[4];
float transmittanceRayPointsCurve = reinterpret_cast<float*>(mappedStagingBufferPointer)[5];
uint32_t transmittanceHeightPoints = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[6];
float transmittanceHeightPointsCurve = reinterpret_cast<float*>(mappedStagingBufferPointer)[7];
uint32_t transmittanceSamples = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[8];

// Inscattering properties
VkFormat inscatteringImageFormat = static_cast<VkFormat>(reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[9]);
uint32_t inscatteringBytesPerPixel = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[10];
uint32_t inscatteringRayPoints = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[11];
float inscatteringRayPointsCurve = reinterpret_cast<float*>(mappedStagingBufferPointer)[12];
uint32_t inscatteringSunPoints = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[13];
float inscatteringSunPointsCurve = reinterpret_cast<float*>(mappedStagingBufferPointer)[14];
uint32_t inscatteringHeightPoints = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[15];
float inscatteringHeightPointsCurve = reinterpret_cast<float*>(mappedStagingBufferPointer)[16];
uint32_t inscatteringSamples = reinterpret_cast<uint32_t*>(mappedStagingBufferPointer)[17];

// Atmosphere dimensions
float atmosphereStart = reinterpret_cast<float*>(mappedStagingBufferPointer)[18];
float seaLevel = reinterpret_cast<float*>(mappedStagingBufferPointer)[19];
float atmosphereEnd = reinterpret_cast<float*>(mappedStagingBufferPointer)[20];

// Scattering scale heights
float rayleighScaleHeight = reinterpret_cast<float*>(mappedStagingBufferPointer)[21];
float mieScaleHeight = reinterpret_cast<float*>(mappedStagingBufferPointer)[22];
float mieAnisotropy = reinterpret_cast<float*>(mappedStagingBufferPointer)[23];

// Scattering parameters
float rayleighScatteringRed = reinterpret_cast<float*>(mappedStagingBufferPointer)[24];
float rayleighScatteringGreen = reinterpret_cast<float*>(mappedStagingBufferPointer)[25];
float rayleighScatteringBlue = reinterpret_cast<float*>(mappedStagingBufferPointer)[26];
float mieScatteringRed = reinterpret_cast<float*>(mappedStagingBufferPointer)[27];
float mieScatteringGreen = reinterpret_cast<float*>(mappedStagingBufferPointer)[28];
float mieScatteringBlue = reinterpret_cast<float*>(mappedStagingBufferPointer)[29];
float mieAbsorptionRed = reinterpret_cast<float*>(mappedStagingBufferPointer)[30];
float mieAbsorptionGreen = reinterpret_cast<float*>(mappedStagingBufferPointer)[31];
float mieAbsorptionBlue = reinterpret_cast<float*>(mappedStagingBufferPointer)[32];
```
Now you need to create a `VkSampler` with `VK_FILTER_LINEAR` (or `VK_FILTER_CUBIC_EXT` if your gpu supports it for both 2D and 3D images), `VK_SAMPLER_MIPMAP_MODE_NEAREST` (because we don't use mipmaps) and `VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE` (to prevent weirdness when sampling outside bounds).
Also create one `VkImage` with `VK_IMAGE_TYPE_2D`, `VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT` and dimensions of `{ transmittanceRayPoints, transmittanceHeightPoints, 1_u32 }` for the transmittance LUT and another image with `VK_IMAGE_TYPE_3D`, same usage flags but dimensions `{ inscatteringRayPoints, inscatteringSunPoints, inscatteringHeightPoints }`.
Next, we copy the pixel data from the staging buffer to their respective images:
```cpp
VkBufferImageCopy inscatteringCopy  
    atmosphereHeaderSize,
    {},
    {},
    {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        0,
        1,
    },
    {},
    {
        transmittanceRayPoints,
        transmittanceHeightPoints,
        1,
    },
};

VkBufferImageCopy inscatteringCopy  
    atmosphereHeaderSize + (transmittanceBytesPerPixel * transmittanceRayPoints * transmittanceHeightPoints),
    {},
    {},
    {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        0,
        1,
    },
    {},
    {
        inscatteringRayPoints,
        inscatteringSunPoints,
        inscatteringHeightPoints,
    },
};

// begin command buffer

// transform images to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL

vkCmdCopyBufferToImage(
    commandBuffer,
    stagingBuffer,
    transmittanceImage,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1,
    &transmittanceCopy
);

vkCmdCopyBufferToImage(
    commandBuffer,
    stagingBuffer,
    inscatteringImage,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1,
    &inscatteringCopy
);

// transform images to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL

// end command buffer and submit
```
Finally bind the images in your descriptor set and use a shader like this example slang shader to sample the atmosphere:
```slang
float signPow(float x, float power)
{
    return sign(x) * pow(abs(x), power);
}

float sampleRayleighPhase(float cosTheta)
{
    return (3.0f / (16.0f * PI)) * (1.0f + cosTheta * cosTheta);
}

float sampleMiePhase(float cosTheta, float anisotropyG)
{
    float g2 = anisotropyG * anisotropyG;
    return (3.0f / (8.0f * PI)) * ((1.0f - g2) * (1.0f + cosTheta * cosTheta)) / ((2.0f + g2) * pow(1.0f + g2 - 2.0f * anisotropyG * cosTheta, 1.5));
}

float3 backgroundRadiance(float raySunCos)
{
    return float3(200.0f, 160.0f, 120.0f) * smoothstep(0.9995f, 0.9999f, raySunCos);
}

float3 precomputedAtmosphere(float3 rayOrigin, float3 rayDirection, float3 sunDirection, float raySunCos)
{
    float height = length(rayOrigin);
    float3 localUp = rayOrigin / height;

    float rayZenithCos = dot(rayDirection, localUp);

    float2 uv = {
        signPow(rayZenithCos, transmittanceRayPointsCurve) * 0.5f + 0.5f,
        signPow((height - atmosphereStart) * inverseAtmosphereRange, transmittanceHeightPointsCurve)
    };

    float3 transmittance = transmittanceLut.SampleLevel(uv, 0).rgb;

    float3 transmittedRadiance = backgroundRadiance(raySunCos) * transmittance;

    float rayleighPhase = sampleRayleighPhase(raySunCos);
    float miePhase = sampleMiePhase(raySunCos, mieAnisotropy);

    float sunZenithCos = dot(sunDirection, localUp);
    float3 uvw = {
        signPow(rayZenithCos, inscatteringRayPointsCurve) * 0.5f + 0.5f,
        signPow(sunZenithCos, inscatteringSunPointsCurve) * 0.5f + 0.5f,
        signPow((height - atmosphereStart) * inverseAtmosphereRange, inscatteringHeightPointsCurve)
    };

    float3 sampledScattering = inscatteringLut.SampleLevel(uvw, 0).rgb;

    float3 inscattering = sampledScattering * (rayleighPhase + miePhase);

    return inscattering + transmittedRadiance;
}

float3 sceneColor(float3 rayOrigin, float3 rayDirection)
{
    float earthDot = dot(rayOrigin, rayDirection);
    float3 sphereFactor = rayOrigin - earthDot * rayDirection;
    float sphereFactorDot = dot(sphereFactor, sphereFactor);
    float3 sunDirection = sceneData.sunDirection_padd.xyz;

    float raySunCos = dot(rayDirection, sunDirection);

    float atmosphereDisc = atmosphereEndSquared - sphereFactorDot;
    if (atmosphereDisc < 0.0f) {
        // completely missed planet
        return backgroundRadiance(raySunCos);
    }

    float atmosphereIntersection = sqrt(atmosphereDisc);
    float atmosphereFarIntersection = -earthDot + atmosphereIntersection;

    if (atmosphereIntersection >= 0.0f && atmosphereFarIntersection >= 0.0f) {
        float atmosphereNearIntersection = -earthDot - atmosphereIntersection;
        if (atmosphereNearIntersection < 0.0f) {
            // inside planet
            return precomputedAtmosphere(rayOrigin, rayDirection, sunDirection, raySunCos);
        } else {
            // outside planet but hit
            return precomputedAtmosphere(rayOrigin + rayDirection * atmosphereNearIntersection, rayDirection, sunDirection, raySunCos);
        }
    } else {
        // also missed
        return backgroundRadiance(raySunCos);
    }
}
```
If you want the atmosphere properties to dynamically change in the shader, you have to use a uniform buffer or shader defines to get them in there.
