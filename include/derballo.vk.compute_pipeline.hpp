/*
 * MIT License
 *
 * Copyright (c) 2026 DerBallo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
