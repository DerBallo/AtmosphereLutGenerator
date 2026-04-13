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
    struct CommandBuffer {
        static constexpr VkCommandBufferBeginInfo defaultOneTimeSubmitBeginInfo {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            {},
            VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            {},
        };

        static constexpr VkCommandBufferBeginInfo defaultBeginInfo {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            {},
            VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            {},
        };

        VkCommandBuffer handle;

        void begin(const VkCommandBufferBeginInfo& beginInfo)
        {
            vkBeginCommandBuffer(
                this->handle,
                addressof(beginInfo)
            );
        }

        void cmdCopyBuffer(
            VkBuffer srcBuffer,
            VkBuffer dstBuffer,
            ArrayView<VkBufferCopy> copyRegions
        )
        {
            vkCmdCopyBuffer(
                this->handle,
                srcBuffer,
                dstBuffer,
                copyRegions.length,
                copyRegions.address
            );
        }

        void cmdCopyImage(
            VkImage srcImage,
            VkImageLayout srcImageLayout,
            VkImage dstImage,
            VkImageLayout dstImageLayout,
            ArrayView<VkImageCopy> copyRegions
        )
        {
            vkCmdCopyImage(
                this->handle,
                srcImage,
                srcImageLayout,
                dstImage,
                dstImageLayout,
                copyRegions.length,
                copyRegions.address
            );
        }

        void cmdCopyBufferToImage(
            VkBuffer srcBuffer,
            VkImage dstImage,
            VkImageLayout dstImageLayout,
            ArrayView<VkBufferImageCopy> copyRegions
        )
        {
            vkCmdCopyBufferToImage(
                this->handle,
                srcBuffer,
                dstImage,
                dstImageLayout,
                copyRegions.length,
                copyRegions.address
            );
        }

        void cmdCopyImageToBuffer(
            VkImage srcImage,
            VkImageLayout srcImageLayout,
            VkBuffer dstBuffer,
            ArrayView<VkBufferImageCopy> copyRegions
        )
        {
            vkCmdCopyImageToBuffer(
                this->handle,
                srcImage,
                srcImageLayout,
                dstBuffer,
                copyRegions.length,
                copyRegions.address
            );
        }

        void cmdClearColorImage(
            VkImage image,
            VkImageLayout imageLayout,
            const VkClearColorValue& color,
            ArrayView<VkImageSubresourceRange> ranges
        )
        {
            vkCmdClearColorImage(
                this->handle,
                image,
                imageLayout,
                addressof(color),
                ranges.length,
                ranges.address
            );
        }

        void cmdBindPipeline(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipeline pipeline
        )
        {
            vkCmdBindPipeline(
                this->handle,
                pipelineBindPoint,
                pipeline
            );
        }

        void cmdBindDescriptorSets(
            VkPipelineBindPoint pipelineBindPoint,
            VkPipelineLayout layout,
            uint32_t firstSet,
            ArrayView<VkDescriptorSet> descriptorSets,
            ArrayView<uint32_t> dynamicOffsets
        )
        {
            vkCmdBindDescriptorSets(
                this->handle,
                pipelineBindPoint,
                layout,
                firstSet,
                descriptorSets.length,
                descriptorSets.address,
                dynamicOffsets.length,
                dynamicOffsets.address
            );
        }

        void cmdPipelineBarrier(
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask,
            VkDependencyFlags dependencyFlags,
            ArrayView<VkMemoryBarrier> memoryBarriers,
            ArrayView<VkBufferMemoryBarrier> bufferMemoryBarriers,
            ArrayView<VkImageMemoryBarrier> imageMemoryBarriers
        )
        {
            vkCmdPipelineBarrier(
                this->handle,
                srcStageMask,
                dstStageMask,
                dependencyFlags,
                memoryBarriers.length,
                memoryBarriers.address,
                bufferMemoryBarriers.length,
                bufferMemoryBarriers.address,
                imageMemoryBarriers.length,
                imageMemoryBarriers.address
            );
        }

        void cmdResetQueryPool(
            VkQueryPool queryPool,
            uint32_t firstQuery,
            uint32_t queryCount
        )
        {
            vkCmdResetQueryPool(
                this->handle,
                queryPool,
                firstQuery,
                queryCount
            );
        }

        void cmdWriteTimestamp(
            VkPipelineStageFlagBits pipelineStage,
            VkQueryPool queryPool,
            uint32_t query
        )
        {
            vkCmdWriteTimestamp(
                this->handle,
                pipelineStage,
                queryPool,
                query
            );
        }

        void cmdPushConstants(
            VkPipelineLayout layout,
            VkShaderStageFlags stageFlags,
            uint32_t offset,
            uint32_t size,
            const void* pValues
        )
        {
            vkCmdPushConstants(
                this->handle,
                layout,
                stageFlags,
                offset,
                size,
                pValues
            );
        }

        void end()
        {
            ve_vkcheck(vkEndCommandBuffer(
                this->handle
            ));
        }

        void reset(VkCommandBufferResetFlags flags)
        {
            ve_vkcheck(vkResetCommandBuffer(
                this->handle,
                flags
            ));
        }

        CommandBuffer() = delete;
        ve_delete_copy_constructors(CommandBuffer);
        ve_delete_move_constructors(CommandBuffer);

        CommandBuffer(const VkCommandBufferAllocateInfo& allocateInfo)
        {
            ve_vkcheck(vkAllocateCommandBuffers(
                device.handle,
                addressof(allocateInfo),
                addressof(this->handle)
            ));
        }
    };

    constexpr VkCommandBufferAllocateInfo makeCommandBufferAllocateInfo(
        const void* pNext,
        VkCommandPool commandPool,
        VkCommandBufferLevel level,
        uint32_t commandBufferCount
    )
    {
        return {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            pNext,
            commandPool,
            level,
            commandBufferCount,
        };
    }

    constexpr VkCommandBufferAllocateInfo makeCommandBufferAllocateInfo(
        const void* pNext,
        VkCommandPool commandPool,
        VkCommandBufferLevel level
    )
    {
        return {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            pNext,
            commandPool,
            level,
            1_u32,
        };
    }

    extern CommandBuffer singleUseCommandBuffer;
}
