#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct ComputePipeline {
        VkPipeline handle;

        ve_delete_copy_constructors(ComputePipeline);
        ve_delete_move_constructors(ComputePipeline);

        ComputePipeline()
            : handle {}
        {
        }

        ComputePipeline(const VkComputePipelineCreateInfo& createInfo)
        {
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

    template <uint32_t N>
    struct ComputePipelines {
        VkPipeline handles[N];

        ve_delete_copy_constructors(ComputePipelines);
        ve_delete_move_constructors(ComputePipelines);

        ComputePipelines()
            : handles {}
        {
        }

        ComputePipelines(ArrayView<VkComputePipelineCreateInfo> createInfos)
        {
            ve_vkcheck(vkCreateComputePipelines(
                device.handle,
                VK_NULL_HANDLE,
                createInfos.length,
                createInfos.address,
                {},
                this->handles
            ));
        }

        ~ComputePipelines()
        {
            for (uint32_t i {}; i < N; ++i) {
                vkDestroyPipeline(
                    device.handle,
                    this->handles[i],
                    {}
                );
            }
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
