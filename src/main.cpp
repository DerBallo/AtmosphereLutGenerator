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

int main()
{
    constexpr uint32_t lutRayPoints { 128_u32 };
    constexpr uint32_t lutSunPoints { 96_u32 };
    constexpr uint32_t lutHeightPoints { 48_u32 };

    static_assert(((lutRayPoints & 3) == 0) && ((lutSunPoints & 3) == 0) && ((lutHeightPoints & 3) == 0), "Dimensions must be divisible by 4!");

    uint64_t lutDataSize { 4_u64 * lutRayPoints * lutSunPoints * lutHeightPoints };
    constexpr VkFormat imageFormat { VK_FORMAT_B10G11R11_UFLOAT_PACK32 };

    ve::Image lutImage {
        ve::makeImageCreateInfo(
            {},
            {},
            VK_IMAGE_TYPE_3D,
            imageFormat,
            {
                lutRayPoints,
                lutSunPoints,
                lutHeightPoints,
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
            imageFormat,
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

    std::system("slangc shader.slang -target spirv -profile spirv_1_5 -O3 -fvk-use-entrypoint-name -entry compute -o shader.spv");

    std::ifstream file("shader.spv", std::ios::binary | std::ios::ate);
    if (!file) {
        return -1;
    }

    std::streamsize size = file.tellg();
    if (size < 1) {
        return -1;
    }

    file.seekg(0, std::ios::beg);

    alignas(16) char shaderCode[size];

    if (!file.read(shaderCode, size)) {
        return -1;
    }

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
        {},
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
        lutRayPoints >> 2_u32,
        lutSunPoints >> 2_u32,
        lutHeightPoints >> 2_u32
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
            lutRayPoints,
            lutSunPoints,
            lutHeightPoints,
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

    std::ofstream("atmosphere_lut.bin", std::ios::binary).write(reinterpret_cast<char*>(lutStagingBuffer.data), lutDataSize);

    /*
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
    */

    return 0;
}
