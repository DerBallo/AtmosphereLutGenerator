#include <cstdio>

#include "derballo.universal.hpp"
#include "derballo.vk.buffer.hpp"
#include "derballo.vk.command_buffer.hpp"
#include "derballo.vk.command_pool.hpp"
#include "derballo.vk.compute_pipeline.hpp"
#include "derballo.vk.descriptor_pool.hpp"
#include "derballo.vk.descriptor_set.hpp"
#include "derballo.vk.descriptor_set_layout.hpp"
#include "derballo.vk.device.hpp"
#include "derballo.vk.fence.hpp"
#include "derballo.vk.image.hpp"
#include "derballo.vk.image_view.hpp"
#include "derballo.vk.pipeline_layout.hpp"
#include "derballo.vk.pnext_chain.hpp"
#include "derballo.vk.sampler.hpp"
#include "derballo.vk.shader_module.hpp"

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

namespace ve {
    struct AtmosphereLutGenerationProperties {
        VkFormat imageFormat;
        uint32_t bytesPerPixel;
        uint32_t rayPoints;
        uint32_t sunPoints;
        uint32_t heightPoints;
        uint32_t viewSamples;
        uint32_t lightSamples;
        float atmosphereStart;
        float seaLevel;
        float atmosphereEnd;
        float rayleighScaleHeight;
        float mieScaleHeight;
        float sunIntensity;
        float rayleighScatteringRed;
        float rayleighScatteringGreen;
        float rayleighScatteringBlue;
        float mieScatteringRed;
        float mieScatteringGreen;
        float mieScatteringBlue;
        float mieAbsorptionRed;
        float mieAbsorptionGreen;
        float mieAbsorptionBlue;
    };

    inline constexpr uint64_t atmosphereHeaderSize { 256_u64 };

    void generateLut(AtmosphereLutGenerationProperties properties, const char* fileName)
    {
        ve_assert(((properties.rayPoints & 3) == 0) && ((properties.sunPoints & 3) == 0) && ((properties.heightPoints & 3) == 0));

        uint64_t transmittanceDataSize { properties.bytesPerPixel * properties.rayPoints * properties.heightPoints };
        uint64_t inscatteringDataSize { properties.bytesPerPixel * properties.rayPoints * properties.sunPoints * properties.heightPoints };
        uint64_t stagingBufferSize { atmosphereHeaderSize + transmittanceDataSize + inscatteringDataSize };

        ve::Image transmittanceImage {
            ve::makeImageCreateInfo(
                {},
                {},
                VK_IMAGE_TYPE_2D,
                properties.imageFormat,
                {
                    properties.rayPoints,
                    properties.heightPoints,
                    1_u32,
                },
                1,
                1,
                VK_SAMPLE_COUNT_1_BIT,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT
                    | VK_IMAGE_USAGE_STORAGE_BIT
                    | VK_IMAGE_USAGE_SAMPLED_BIT,
                VK_SHARING_MODE_EXCLUSIVE,
                {},
                VK_IMAGE_LAYOUT_UNDEFINED
            ),
        };

        ve::Image inscatteringImage {
            ve::makeImageCreateInfo(
                {},
                {},
                VK_IMAGE_TYPE_3D,
                properties.imageFormat,
                {
                    properties.rayPoints,
                    properties.sunPoints,
                    properties.heightPoints,
                },
                1,
                1,
                VK_SAMPLE_COUNT_1_BIT,
                VK_IMAGE_TILING_OPTIMAL,
                VK_IMAGE_USAGE_TRANSFER_SRC_BIT
                    | VK_IMAGE_USAGE_STORAGE_BIT,
                VK_SHARING_MODE_EXCLUSIVE,
                {},
                VK_IMAGE_LAYOUT_UNDEFINED
            ),
        };

        constexpr VkImageSubresourceRange subresourceRange {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0u,
            1u,
            0u,
            1u,
        };

        ve::ImageView transmittanceImageView {
            ve::makeImageViewCreateInfo(
                {},
                {},
                transmittanceImage.handle,
                VK_IMAGE_VIEW_TYPE_2D,
                properties.imageFormat,
                {
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_G,
                    VK_COMPONENT_SWIZZLE_B,
                    VK_COMPONENT_SWIZZLE_A,
                },
                subresourceRange
            ),
        };

        ve::Sampler transmittanceSampler {
            makeSamplerCreateInfo(
                {},
                {},
                VK_FILTER_LINEAR,
                VK_FILTER_LINEAR,
                VK_SAMPLER_MIPMAP_MODE_NEAREST,
                VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                {},
                {},
                1.0f,
                {},
                {},
                {},
                {},
                {},
                {}
            ),
        };

        ve::ImageView inscatteringImageView {
            ve::makeImageViewCreateInfo(
                {},
                {},
                inscatteringImage.handle,
                VK_IMAGE_VIEW_TYPE_3D,
                properties.imageFormat,
                {
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_G,
                    VK_COMPONENT_SWIZZLE_B,
                    VK_COMPONENT_SWIZZLE_A,
                },
                subresourceRange
            ),
        };

        ve::Buffer<ve::BufferType::HostSizeless> stagingBuffer {
            stagingBufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        };

        ve::DescriptorSetLayout transmittanceDescriptorSetLayout {
            ve::makeDescriptorSetLayoutCreateInfo(
                {},
                {},
                {
                    {
                        0_u32,
                        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                        1_u32,
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        {},
                    },
                }
            ),
        };

        ve::DescriptorSetLayout inscatteringDescriptorSetLayout {
            ve::makeDescriptorSetLayoutCreateInfo(
                {},
                {},
                {
                    {
                        1_u32,
                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                        1_u32,
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        {},
                    },
                    {
                        2_u32,
                        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                        1_u32,
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        {},
                    },
                }
            ),
        };

        ve::PipelineLayout transmittancePipelineLayout {
            ve::makePipelineLayoutCreateInfo(
                {},
                {},
                {
                    transmittanceDescriptorSetLayout.handle,
                },
                {}
            ),
        };

        ve::PipelineLayout inscatteringPipelineLayout {
            ve::makePipelineLayoutCreateInfo(
                {},
                {},
                {
                    inscatteringDescriptorSetLayout.handle,
                },
                {}
            ),
        };

        ve::DescriptorPool descriptorPool {
            ve::makeDescriptorPoolCreateInfo(
                {},
                {},
                3_u32,
                {
                    {
                        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                        2_u32,
                    },
                    {
                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                        1_u32,
                    },
                }
            ),
        };

        ve::DescriptorSet transmittanceDescriptorSet {
            ve::makeDescriptorSetAllocateInfo(
                {},
                descriptorPool.handle,
                {
                    transmittanceDescriptorSetLayout.handle,
                }
            ),
        };

        ve::DescriptorSet inscatteringDescriptorSet {
            ve::makeDescriptorSetAllocateInfo(
                {},
                descriptorPool.handle,
                {
                    inscatteringDescriptorSetLayout.handle,
                }
            ),
        };

        VkDescriptorImageInfo transmittanceImageInfo {
            {},
            transmittanceImageView.handle,
            VK_IMAGE_LAYOUT_GENERAL,
        };

        VkDescriptorImageInfo transmittanceSamplerInfo {
            transmittanceSampler.handle,
            transmittanceImageView.handle,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        };

        VkDescriptorImageInfo inscatteringImageInfo {
            {},
            inscatteringImageView.handle,
            VK_IMAGE_LAYOUT_GENERAL,
        };

        VkWriteDescriptorSet descriptorWrites[] {
            {
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                {},
                transmittanceDescriptorSet.handle,
                0_u32,
                0_u32,
                1_u32,
                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                addressof(transmittanceImageInfo),
                {},
                {},
            },
            {
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                {},
                inscatteringDescriptorSet.handle,
                1_u32,
                0_u32,
                1_u32,
                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                addressof(transmittanceSamplerInfo),
                {},
                {},
            },
            {
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                {},
                inscatteringDescriptorSet.handle,
                2_u32,
                0_u32,
                1_u32,
                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                addressof(inscatteringImageInfo),
                {},
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

        std::string shaderCompileCommand {
            "slangc "
            "-target spirv "
            "-profile spirv_1_5 "
            "-O3 "
            "-fvk-use-entrypoint-name "
            "-entry transmittance "
            "-entry inscattering "
        };

#define add_define_uint(name, value) \
    shaderCompileCommand += std::format(std::locale::classic(), "-D" name "={}u ", value);

        add_define_uint("RAY_POINTS", properties.rayPoints);
        add_define_uint("SUN_POINTS", properties.sunPoints);
        add_define_uint("HEIGHT_POINTS", properties.heightPoints);

        add_define_uint("VIEW_SAMPLES", properties.viewSamples);
        add_define_uint("LIGHT_SAMPLES", properties.lightSamples);

#undef add_define_uint

#define add_define_float(name, value) \
    shaderCompileCommand += std::format(std::locale::classic(), "-D" name "={:.32f}f ", value);

        add_define_float("ATMOSPHERE_START", properties.atmosphereStart);
        add_define_float("SEA_LEVEL", properties.seaLevel);
        add_define_float("ATMOSPHERE_END", properties.atmosphereEnd);

        add_define_float("RAYLEIGH_SCALE_HEIGHT", properties.rayleighScaleHeight);
        add_define_float("MIE_SCALE_HEIGHT", properties.mieScaleHeight);

        add_define_float("SUN_INTENSITY", properties.sunIntensity);
        add_define_float("RAYLEIGH_SCATTERING_RED", properties.rayleighScatteringRed);
        add_define_float("RAYLEIGH_SCATTERING_GREEN", properties.rayleighScatteringGreen);
        add_define_float("RAYLEIGH_SCATTERING_BLUE", properties.rayleighScatteringBlue);
        add_define_float("MIE_SCATTERING_RED", properties.mieScatteringRed);
        add_define_float("MIE_SCATTERING_GREEN", properties.mieScatteringGreen);
        add_define_float("MIE_SCATTERING_BLUE", properties.mieScatteringBlue);
        add_define_float("MIE_ABSORPTION_RED", properties.mieAbsorptionRed);
        add_define_float("MIE_ABSORPTION_GREEN", properties.mieAbsorptionGreen);
        add_define_float("MIE_ABSORPTION_BLUE", properties.mieAbsorptionBlue);

#undef add_define_float

        shaderCompileCommand += "shader.slang -o shader.spv";

        std::println("{}", shaderCompileCommand);

        std::system(shaderCompileCommand.c_str());

        std::ifstream file("shader.spv", std::ios::binary | std::ios::ate);
        ve_assert(file);

        ptrdiff_t size = file.tellg();
        ve_assert(size > 0);

        file.seekg(0, std::ios::beg);

        alignas(16) char shaderCode[size];

        ve_assert(file.read(shaderCode, size));

        ve::ShaderModule shaderModule {
            ve::makeShaderModuleCreateInfo(
                {},
                {},
                {
                    static_cast<size_t>(size),
                    shaderCode,
                }
            ),
        };

        ve::ComputePipelines<2_u32> computePipelines {
            {
                ve::makeComputePipelineCreateInfo(
                    {},
                    {},
                    {
                        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        {},
                        {},
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        shaderModule.handle,
                        "transmittance",
                        {},
                    },
                    transmittancePipelineLayout.handle,
                    {},
                    {}
                ),
                ve::makeComputePipelineCreateInfo(
                    {},
                    {},
                    {
                        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                        {},
                        {},
                        VK_SHADER_STAGE_COMPUTE_BIT,
                        shaderModule.handle,
                        "inscattering",
                        {},
                    },
                    inscatteringPipelineLayout.handle,
                    {},
                    {}
                ),
            },
        };

        vkBeginCommandBuffer(
            ve::singleUseCommandBuffer.handle,
            addressof(ve::commandBufferSingleUseBeginInfo)
        );

        vkCmdBindDescriptorSets(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            transmittancePipelineLayout.handle,
            0,
            1,
            addressof(transmittanceDescriptorSet.handle),
            {},
            {}
        );

        vkCmdBindPipeline(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            computePipelines.handles[0_u32]
        );

        VkImageMemoryBarrier convertGeneralImageBarrier {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            {},
            0,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_QUEUE_FAMILY_IGNORED,
            VK_QUEUE_FAMILY_IGNORED,
            transmittanceImage.handle,
            subresourceRange,
        };

        vkCmdPipelineBarrier(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            {},
            {},
            {},
            {},
            {},
            1_u32,
            addressof(convertGeneralImageBarrier)
        );

        vkCmdDispatch(
            ve::singleUseCommandBuffer.handle,
            properties.rayPoints >> 2_u32,
            properties.heightPoints >> 2_u32,
            1_u32
        );

        VkImageMemoryBarrier convertTransferImageBarrier {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            {},
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_QUEUE_FAMILY_IGNORED,
            VK_QUEUE_FAMILY_IGNORED,
            transmittanceImage.handle,
            subresourceRange,
        };

        vkCmdPipelineBarrier(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            {},
            {},
            {},
            {},
            {},
            1_u32,
            addressof(convertTransferImageBarrier)
        );

        VkBufferImageCopy transmittanceCopyRegion {
            atmosphereHeaderSize,
            {},
            {},
            {
                VK_IMAGE_ASPECT_COLOR_BIT,
                {},
                {},
                1_u32,
            },
            {},
            {
                properties.rayPoints,
                properties.heightPoints,
                1_u32,
            },
        };

        vkCmdCopyImageToBuffer(
            ve::singleUseCommandBuffer.handle,
            transmittanceImage.handle,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            stagingBuffer.handle,
            1_u32,
            addressof(transmittanceCopyRegion)
        );

        vkCmdBindDescriptorSets(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            inscatteringPipelineLayout.handle,
            0,
            1,
            addressof(inscatteringDescriptorSet.handle),
            {},
            {}
        );

        vkCmdBindPipeline(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            computePipelines.handles[1_u32]
        );

        VkImageMemoryBarrier convertGeneralImageBarriers[] {
            {
                VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                {},
                VK_ACCESS_TRANSFER_READ_BIT,
                VK_ACCESS_SHADER_READ_BIT,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                transmittanceImage.handle,
                subresourceRange,
            },
            {
                VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                {},
                0,
                VK_ACCESS_SHADER_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_GENERAL,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                inscatteringImage.handle,
                subresourceRange,
            },
        };

        vkCmdPipelineBarrier(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            {},
            {},
            {},
            {},
            {},
            lengthof(convertGeneralImageBarriers),
            convertGeneralImageBarriers
        );

        vkCmdDispatch(
            ve::singleUseCommandBuffer.handle,
            properties.rayPoints >> 2_u32,
            properties.sunPoints >> 2_u32,
            properties.heightPoints >> 2_u32
        );

        convertTransferImageBarrier.image = inscatteringImage.handle;

        vkCmdPipelineBarrier(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT,
            {},
            {},
            {},
            {},
            {},
            1_u32,
            addressof(convertTransferImageBarrier)
        );

        VkBufferImageCopy inscatteringCopyRegion {
            atmosphereHeaderSize + transmittanceDataSize,
            {},
            {},
            {
                VK_IMAGE_ASPECT_COLOR_BIT,
                {},
                {},
                1_u32,
            },
            {},
            {
                properties.rayPoints,
                properties.sunPoints,
                properties.heightPoints,
            },
        };

        vkCmdCopyImageToBuffer(
            ve::singleUseCommandBuffer.handle,
            inscatteringImage.handle,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            stagingBuffer.handle,
            1_u32,
            addressof(inscatteringCopyRegion)
        );

        vkEndCommandBuffer(
            ve::singleUseCommandBuffer.handle
        );

        VkSubmitInfo submitInfo {
            VK_STRUCTURE_TYPE_SUBMIT_INFO,
            {},
            {},
            {},
            {},
            1_u32,
            addressof(ve::singleUseCommandBuffer.handle),
            {},
            {},
        };

        ve_vkcheck(vkQueueSubmit(
            ve::device.queue,
            1_u32,
            addressof(submitInfo),
            ve::singleUseFence.handle
        ));

        ve_vkcheck(vkWaitForFences(
            ve::device.handle,
            1_u32,
            addressof(ve::singleUseFence.handle),
            true,
            UINT64_MAX
        ));

        ve_vkcheck(vkResetFences(
            ve::device.handle,
            1_u32,
            addressof(ve::singleUseFence.handle)
        ));

        ve_vkcheck(vkResetCommandBuffer(
            ve::singleUseCommandBuffer.handle,
            {}
        ));

        // magic number 'ATMO'
        reinterpret_cast<uint32_t*>(stagingBuffer.data)[0_u32] = 0x41544D4F_u32;

        // version
        reinterpret_cast<uint32_t*>(stagingBuffer.data)[1_u32] = 1_u32;

        // properties
        reinterpret_cast<uint32_t*>(stagingBuffer.data)[2_u32] = properties.imageFormat;
        reinterpret_cast<uint32_t*>(stagingBuffer.data)[3_u32] = properties.bytesPerPixel;

        reinterpret_cast<uint32_t*>(stagingBuffer.data)[4_u32] = properties.rayPoints;
        reinterpret_cast<uint32_t*>(stagingBuffer.data)[5_u32] = properties.sunPoints;
        reinterpret_cast<uint32_t*>(stagingBuffer.data)[6_u32] = properties.heightPoints;

        reinterpret_cast<float*>(stagingBuffer.data)[7_u32] = properties.atmosphereStart;
        reinterpret_cast<float*>(stagingBuffer.data)[8_u32] = properties.seaLevel;
        reinterpret_cast<float*>(stagingBuffer.data)[9_u32] = properties.atmosphereEnd;

        reinterpret_cast<float*>(stagingBuffer.data)[10_u32] = properties.rayleighScaleHeight;
        reinterpret_cast<float*>(stagingBuffer.data)[11_u32] = properties.mieScaleHeight;

        reinterpret_cast<float*>(stagingBuffer.data)[12_u32] = properties.sunIntensity;

        reinterpret_cast<float*>(stagingBuffer.data)[13_u32] = properties.rayleighScatteringRed;
        reinterpret_cast<float*>(stagingBuffer.data)[14_u32] = properties.rayleighScatteringGreen;
        reinterpret_cast<float*>(stagingBuffer.data)[15_u32] = properties.rayleighScatteringBlue;

        reinterpret_cast<float*>(stagingBuffer.data)[16_u32] = properties.mieScatteringRed;
        reinterpret_cast<float*>(stagingBuffer.data)[17_u32] = properties.mieScatteringGreen;
        reinterpret_cast<float*>(stagingBuffer.data)[18_u32] = properties.mieScatteringBlue;

        reinterpret_cast<float*>(stagingBuffer.data)[19_u32] = properties.mieAbsorptionRed;
        reinterpret_cast<float*>(stagingBuffer.data)[20_u32] = properties.mieAbsorptionGreen;
        reinterpret_cast<float*>(stagingBuffer.data)[21_u32] = properties.mieAbsorptionBlue;

        std::ofstream(fileName, std::ios::binary).write(reinterpret_cast<char*>(stagingBuffer.data), stagingBufferSize);
    }
}

int main(int, char*[])
{
    constexpr ve::AtmosphereLutGenerationProperties jupiterProperties {
        VK_FORMAT_B10G11R11_UFLOAT_PACK32,
        4_u32,
        128_u32,
        96_u32,
        48_u32,
        100000_u32,
        100000_u32,
        2.9575e6_f,
        3.0e6_f,
        3.3e6_f,
        27000.0_f,
        12000.0_f,
        8.0_f,
        1.5e-6_f,
        1.0e-6_f,
        0.8e-6_f,
        2.0e-5_f,
        1.8e-5_f,
        1.6e-5_f,
        1.0e-5_f,
        1.8e-5_f,
        3.0e-5_f,
    };

    ve::generateLut(
        jupiterProperties,
        "lut.atmosphere"
    );

    return 0;
}
