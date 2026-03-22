#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct Fence {
        VkFence handle;

        Fence() = delete;
        ve_delete_copy_constructors(Fence);
        ve_delete_move_constructors(Fence);

        Fence(const VkFenceCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateFence(
                device.handle,
                addressof(createInfo),
                nullptr,
                addressof(this->handle)
            ));
        }

        ~Fence()
        {
            vkDestroyFence(
                device.handle,
                this->handle,
                nullptr
            );
        }
    };

    template <uint32_t N1>
    struct Fences {
        VkFence handles[N1];

        constexpr uint32_t count() const noexcept { return N1; }

        Fences() = delete;
        ve_delete_copy_constructors(Fences);
        ve_delete_move_constructors(Fences);

        Fences(const VkFenceCreateInfo& createInfo)
        {
            for (uint32_t i {}; i < N1; i++) {
                ve_vkcheck(vkCreateFence(
                    device.handle,
                    addressof(createInfo),
                    nullptr,
                    this->handles + i
                ));
            }
        }

        ~Fences()
        {
            for (uint32_t i {}; i < N1; i++) {
                vkDestroyFence(
                    device.handle,
                    this->handles[i],
                    nullptr
                );
            }
        }
    };

    constexpr VkFenceCreateInfo makeFenceCreateInfo(
        const void* pNext,
        VkFenceCreateFlags flags
    )
    {
        return {
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            pNext,
            flags,
        };
    }

    inline constexpr VkFenceCreateInfo defaultFenceCreateInfo {
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        {},
        {},
    };

    inline constexpr VkFenceCreateInfo defaultSignaledFenceCreateInfo {
        VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        {},
        VK_FENCE_CREATE_SIGNALED_BIT,
    };

    extern Fence singleUseFence;
}
