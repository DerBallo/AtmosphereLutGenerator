#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct Semaphore {
        VkSemaphore handle;

        Semaphore() = delete;
        ve_delete_copy_constructors(Semaphore);
        ve_delete_move_constructors(Semaphore);

        Semaphore(const VkSemaphoreCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateSemaphore(
                device.handle,
                addressof(createInfo),
                nullptr,
                addressof(this->handle)
            ));
        }

        ~Semaphore()
        {
            vkDestroySemaphore(
                device.handle,
                this->handle,
                nullptr
            );
        }
    };

    template <uint32_t N1>
    struct Semaphores {
        VkSemaphore handles[N1];

        constexpr uint32_t count() const noexcept { return N1; }

        Semaphores() = delete;
        ve_delete_copy_constructors(Semaphores);
        ve_delete_move_constructors(Semaphores);

        Semaphores(const VkSemaphoreCreateInfo& createInfo)
        {
            for (uint32_t i {}; i < N1; i++) {
                ve_vkcheck(vkCreateSemaphore(
                    device.handle,
                    addressof(createInfo),
                    nullptr,
                    this->handles + i
                ));
            }
        }

        ~Semaphores()
        {
            for (uint32_t i {}; i < N1; i++) {
                vkDestroySemaphore(
                    device.handle,
                    this->handles[i],
                    nullptr
                );
            }
        }
    };

    constexpr VkSemaphoreCreateInfo makeSemaphoreCreateInfo(
        const void* pNext,
        VkSemaphoreCreateFlags flags
    )
    {
        return {
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            pNext,
            flags,
        };
    }

    constexpr VkSemaphoreTypeCreateInfo makeSemaphoreTypeCreateInfo(
        const void* pNext,
        VkSemaphoreType semaphoreType,
        uint64_t initialValue
    )
    {
        return {
            VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO,
            pNext,
            semaphoreType,
            initialValue,
        };
    }

    inline constexpr VkSemaphoreCreateInfo defaultSemaphoreCreateInfo {
        VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        nullptr,
        0_u32,
    };
}
