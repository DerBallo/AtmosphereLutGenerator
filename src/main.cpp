#include <cstdio>

#include "derballo.universal.hpp"
#include "derballo.vk.buffer.hpp"
#include "derballo.vk.command_buffer.hpp"
#include "derballo.vk.command_pool.hpp"
#include "derballo.vk.descriptor_pool.hpp"
#include "derballo.vk.descriptor_set.hpp"
#include "derballo.vk.descriptor_set_layout.hpp"
#include "derballo.vk.device.hpp"
#include "derballo.vk.fence.hpp"
#include "derballo.vk.pipeline_layout.hpp"
#include "derballo.vk.pnext_chain.hpp"

#include <fstream>

ve::VulkanInstance ve::vulkanInstance {
    makeInstanceCreateInfo(
        {},
        0_u32,
        addressof(makeApplicationInfo(
            {},
            "AtmosphereLutGenerator",
            VK_MAKE_VERSION(1, 0, 0),
            "DerBalloEngine",
            VK_MAKE_VERSION(1, 0, 0),
            VK_API_VERSION_1_4
        )),
        {
#ifndef NDEBUG
            "VK_LAYER_KHRONOS_validation",
#endif
        },
        {
            "VK_EXT_debug_utils",
        }
    ),
};

ve::Gpu ve::gpu {
    1_u32,
};

ve::Device ve::device {
    makeDeviceCreateInfo(
        PnextChain {
            VkPhysicalDeviceVulkan11Features {
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
            },
            VkPhysicalDeviceVulkan12Features {
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
                {},
                {},
                {},
                VK_TRUE,
                VK_TRUE,
                {},
                {},
                {},
                VK_TRUE,
                VK_TRUE,
                VK_TRUE,
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                VK_TRUE,
                {},
                {},
                {},
                VK_TRUE,
                VK_TRUE,
                VK_TRUE,
                {},
                {},
                {},
                {},
                {},
                {},
                VK_TRUE,
                VK_TRUE,
                VK_TRUE,
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
            },
            VkPhysicalDeviceVulkan13Features {
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                VK_TRUE,
                {},
                {},
                {},
                {},
                {},
            },
            VkPhysicalDeviceVulkan14Features {
                VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES,
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
                {},
            },
        },
        0_u32,
        {
            makeDeviceQueueCreateInfo(
                {},
                0_u32,
                0_u32,
                {
                    1.0_f,
                }
            ),
        },
        {},
        {
            "VK_KHR_buffer_device_address",
            "VK_KHR_deferred_host_operations",
            "VK_KHR_shader_float_controls",
            "VK_KHR_spirv_1_4",
        }
    ),
};

ve::CommandPool ve::commandPool {
    makeCommandPoolCreateInfo(
        {},
        VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        0_u32
    ),
};

ve::CommandBuffer ve::singleUseCommandBuffer {
    makeCommandBufferAllocateInfo(
        {},
        commandPool.handle,
        VK_COMMAND_BUFFER_LEVEL_PRIMARY
    ),
};

ve::Fence ve::singleUseFence {
    ve::defaultFenceCreateInfo,
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
    constexpr uint64_t lutSize { sizeof(float4_t) * lutLength };

    ve::Buffer<ve::BufferType::DeviceSizeless> lutDeviceBuffer {
        lutSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    };

    ve::Buffer<ve::BufferType::HostSizeless> lutStagingBuffer {
        lutSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    };

    ve::DescriptorSetLayout descriptorSetLayout {
        ve::makeDescriptorSetLayoutCreateInfo(
            {},
            {},
            {
                {
                    0_u32,
                    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    1_u32,
                    VK_SHADER_STAGE_COMPUTE_BIT,
                    {},
                },
            }
        ),
    };

    ve::PipelineLayout pipelineLayout {
        ve::makePipelineLayoutCreateInfo(
            {},
            {},
            {
                descriptorSetLayout.handle,
            },
            {}
        ),
    };

    ve::DescriptorPool descriptorPool {
        ve::makeDescriptorPoolCreateInfo(
            {},
            {},
            1_u32,
            {
                {
                    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                    1_u32,
                },
            }
        ),
    };

    ve::DescriptorSet descriptorSet {
        ve::makeDescriptorSetAllocateInfo(
            {},

            descriptorPool.handle,
            {
                descriptorSetLayout.handle,
            }
        ),
    };

    VkDescriptorBufferInfo lutDeviceBufferInfo {
        lutDeviceBuffer.handle,
        0_u64,
        VK_WHOLE_SIZE,
    };

    VkWriteDescriptorSet descriptorWrites[] {
        {
            VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            {},
            descriptorSet.handle,
            0_u32,
            0_u32,
            1_u32,
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            {},
            addressof(lutDeviceBufferInfo),
            {},
        },
    };

    vkUpdateDescriptorSets(
        ve::device.handle,
        lengthof(descriptorWrites),
        descriptorWrites,
        {},
        {}
    );

    // load shader module

    // create compute pipeline

    // record commands

    // transfer data back

    /*
    float4_t* LUT = new (std::nothrow) float4_t[lutLength];

    GenerateAtmosphereLut(
        LUT,
        lutR,
        lutMu,
        lutMuS,
        100_u32,
        10_u32,
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

    std::ofstream("lut.bin", std::ios::binary).write(reinterpret_cast<char*>(LUT), lutLength * sizeof(float4_t));
    */

    return 0;
}
