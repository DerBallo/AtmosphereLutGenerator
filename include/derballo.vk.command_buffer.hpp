#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct CommandBuffer {
        VkCommandBuffer handle;

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

    template <uint32_t N1>
    struct CommandBuffers {
        VkCommandBuffer handles[N1];

        constexpr uint32_t count() const noexcept { return N1; }

        void reset(VkCommandBufferResetFlags flags)
        {
            for (uint32_t i {}; i < N1; i++) {
                ve_vkcheck(vkResetCommandBuffer(
                    this->handles[i],
                    flags
                ));
            }
        }

        CommandBuffers() = delete;
        ve_delete_copy_constructors(CommandBuffers);
        ve_delete_move_constructors(CommandBuffers);

        CommandBuffers(const VkCommandBufferAllocateInfo& allocateInfo)
        {
            ve_vkcheck(vkAllocateCommandBuffers(
                device.handle,
                addressof(allocateInfo),
                this->handles
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

    inline constexpr VkCommandBufferBeginInfo commandBufferSingleUseBeginInfo {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext {},
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo {},
    };

    extern CommandBuffer singleUseCommandBuffer;
}
