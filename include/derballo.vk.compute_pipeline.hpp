#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct ComputePipeline {
        VkPipeline handle;

        ComputePipeline() = delete;
        ve_delete_copy_constructors(ComputePipeline);
        ve_delete_move_constructors(ComputePipeline);

        ComputePipeline(const VkComputePipelineCreateInfo& createInfo)
        {
            if (createInfo.stage.module == VK_NULL_HANDLE) {
                this->handle = VK_NULL_HANDLE;
                return;
            }

            ve_vkcheck(vkCreateComputePipelines(
                device.handle,
                VK_NULL_HANDLE,
                1_u32,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));
        }

        ~ComputePipeline()
        {
            vkDestroyPipeline(
                device.handle,
                this->handle,
                {}
            );
        }
    };

    constexpr VkComputePipelineCreateInfo makeComputePipelineCreateInfo(
        const void* pNext,
        VkPipelineCreateFlags flags,
        VkPipelineShaderStageCreateInfo stage,
        VkPipelineLayout layout,
        VkPipeline basePipelineHandle,
        int32_t basePipelineIndex
    )
    {
        return {
            VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            pNext,
            flags,
            stage,
            layout,
            basePipelineHandle,
            basePipelineIndex,
        };
    }
}
