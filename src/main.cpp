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
    struct AtmosphereLutProperties {
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
        VkFormat imageFormat;
        float3_t rayleighScattering;
        float3_t mieScattering;
        float3_t mieAbsorption;
    };

    void generateLut(AtmosphereLutProperties properties, const char* fileName)
    {
        ve_assert(((properties.rayPoints & 3) == 0) && ((properties.sunPoints & 3) == 0) && ((properties.heightPoints & 3) == 0));

        uint64_t lutDataSize { 4_u64 * properties.rayPoints * properties.sunPoints * properties.heightPoints };

        ve::Image lutImage {
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

        constexpr VkImageSubresourceRange lutSubresourceRange {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0u,
            1u,
            0u,
            1u,
        };

        ve::ImageView lutImageView {
            ve::makeImageViewCreateInfo(
                {},
                {},
                lutImage.handle,
                VK_IMAGE_VIEW_TYPE_3D,
                properties.imageFormat,
                {
                    VK_COMPONENT_SWIZZLE_R,
                    VK_COMPONENT_SWIZZLE_G,
                    VK_COMPONENT_SWIZZLE_B,
                    VK_COMPONENT_SWIZZLE_A,
                },
                lutSubresourceRange
            ),
        };

        ve::Buffer<ve::BufferType::HostSizeless> lutStagingBuffer {
            lutDataSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        };

        ve::DescriptorSetLayout descriptorSetLayout {
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
                        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
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

        VkDescriptorImageInfo lutImageInfo {
            {},
            lutImageView.handle,
            VK_IMAGE_LAYOUT_GENERAL,
        };

        VkWriteDescriptorSet descriptorWrites[] {
            {
                VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                {},
                descriptorSet.handle,
                0_u32,
                0_u32,
                1_u32,
                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                addressof(lutImageInfo),
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

        std::string command {
            std::format(
                std::locale::classic(),
                "slangc "
                "-target spirv "
                "-profile spirv_1_5 "
                "-O3 "
                "-fvk-use-entrypoint-name "
                "-entry compute "
                "-DRAY_POINTS={} "
                "-DSUN_POINTS={} "
                "-DHEIGHT_POINTS={} "
                "-DVIEW_SAMPLES={} "
                "-DLIGHT_SAMPLES={} "
                "-DATMOSPHERE_START={:g} "
                "-DSEA_LEVEL={:g} "
                "-DATMOSPHERE_END={:g} "
                "-DRAYLEIGH_SCALE_HEIGHT={:g} "
                "-DMIE_SCALE_HEIGHT={:g} "
                "-DSUN_INTENSITY={:g} "
                "-DRAYLEIGH_SCATTERING_RED={:g} "
                "-DRAYLEIGH_SCATTERING_GREEN={:g} "
                "-DRAYLEIGH_SCATTERING_BLUE={:g} "
                "-DMIE_SCATTERING_RED={:g} "
                "-DMIE_SCATTERING_GREEN={:g} "
                "-DMIE_SCATTERING_BLUE={:g} "
                "-DMIE_ABSORPTION_RED={:g} "
                "-DMIE_ABSORPTION_GREEN={:g} "
                "-DMIE_ABSORPTION_BLUE={:g} "
                "shader.slang "
                "-o shader.spv",
                properties.rayPoints,
                properties.sunPoints,
                properties.heightPoints,
                properties.viewSamples,
                properties.lightSamples,
                properties.atmosphereStart,
                properties.seaLevel,
                properties.atmosphereEnd,
                properties.rayleighScaleHeight,
                properties.mieScaleHeight,
                properties.sunIntensity,
                properties.rayleighScattering.x,
                properties.rayleighScattering.y,
                properties.rayleighScattering.z,
                properties.mieScattering.x,
                properties.mieScattering.y,
                properties.mieScattering.z,
                properties.mieAbsorption.x,
                properties.mieAbsorption.y,
                properties.mieAbsorption.z
            ),
        };

        std::system(command.c_str());

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

        ve::ComputePipeline computePipeline {
            ve::makeComputePipelineCreateInfo(
                {},
                {},
                {
                    VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    {},
                    {},
                    VK_SHADER_STAGE_COMPUTE_BIT,
                    shaderModule.handle,
                    "compute",
                    {},
                },
                pipelineLayout.handle,
                {},
                {}
            ),
        };

        vkBeginCommandBuffer(
            ve::singleUseCommandBuffer.handle,
            addressof(ve::commandBufferSingleUseBeginInfo)
        );

        vkCmdBindPipeline(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            computePipeline.handle
        );

        vkCmdBindDescriptorSets(
            ve::singleUseCommandBuffer.handle,
            VK_PIPELINE_BIND_POINT_COMPUTE,
            pipelineLayout.handle,
            0,
            1,
            addressof(descriptorSet.handle),
            {},
            {}
        );

        VkImageMemoryBarrier lutMemoryBarrier {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            {},
            0,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_QUEUE_FAMILY_IGNORED,
            VK_QUEUE_FAMILY_IGNORED,
            lutImage.handle,
            lutSubresourceRange,
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
            addressof(lutMemoryBarrier)
        );

        vkCmdDispatch(
            ve::singleUseCommandBuffer.handle,
            properties.rayPoints >> 2_u32,
            properties.sunPoints >> 2_u32,
            properties.heightPoints >> 2_u32
        );

        lutMemoryBarrier = {
            VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            {},
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_TRANSFER_READ_BIT,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            VK_QUEUE_FAMILY_IGNORED,
            VK_QUEUE_FAMILY_IGNORED,
            lutImage.handle,
            lutSubresourceRange,
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
            addressof(lutMemoryBarrier)
        );

        VkBufferImageCopy copyRegion {
            {},
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
            lutImage.handle,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            lutStagingBuffer.handle,
            1_u32,
            addressof(copyRegion)
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

        std::ofstream(fileName, std::ios::binary).write(reinterpret_cast<char*>(lutStagingBuffer.data), lutDataSize);
    }
}

int main(int, char*[])
{
    constexpr ve::AtmosphereLutProperties jupiterProperties {
        128_u32,
        96_u32,
        48_u32,
        5000_u32,
        1000_u32,
        2.9575e6_f,
        3.0e6_f,
        3.3e6_f,
        27000.0_f,
        12000.0_f,
        8.0_f,
        VK_FORMAT_B10G11R11_UFLOAT_PACK32,
        { 1.5e-6_f, 1.0e-6_f, 0.8e-6_f },
        { 2.0e-5_f, 1.8e-5_f, 1.6e-5_f },
        { 1.0e-5_f, 1.8e-5_f, 3.0e-5_f },
    };

    ve::generateLut(
        jupiterProperties,
        "lut.scattering"
    );

    return 0;
}
