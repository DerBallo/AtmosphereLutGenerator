#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct CommandPool {
        VkCommandPool handle;

        void freeCommandBuffers(
            VkCommandBuffer* pCommandBuffers,
            uint32_t count
        )
        {
            vkFreeCommandBuffers(
                device.handle,
                this->handle,
                count,
                pCommandBuffers
            );
        }

        CommandPool() = delete;
        ve_delete_copy_constructors(CommandPool);
        ve_delete_move_constructors(CommandPool);

        CommandPool(const VkCommandPoolCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateCommandPool(
                device.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));
        }

        ~CommandPool()
        {
            vkDestroyCommandPool(
                device.handle,
                this->handle,
                {}
            );
        }
    };

    constexpr VkCommandPoolCreateInfo makeCommandPoolCreateInfo(
        const void* pNext,
        VkCommandPoolCreateFlags flags,
        uint32_t queueFamilyIndex
    )
    {
        return {
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            pNext,
            flags,
            queueFamilyIndex,
        };
    }

    extern CommandPool commandPool;
}
