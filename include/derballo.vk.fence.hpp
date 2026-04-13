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
    struct Fence {
        static constexpr VkFenceCreateInfo defaultCreateInfo {
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            {},
            {},
        };

        static constexpr VkFenceCreateInfo defaultSignaledCreateInfo {
            VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            {},
            VK_FENCE_CREATE_SIGNALED_BIT,
        };

        VkFence handle;

        void wait(uint64_t timeout)
        {
            ve_vkcheck(vkWaitForFences(
                device.handle,
                1_u32,
                addressof(this->handle),
                true,
                timeout
            ));
        }

        void reset()
        {
            ve_vkcheck(vkResetFences(
                device.handle,
                1_u32,
                addressof(this->handle)
            ));
        }

        ve_delete_copy_constructors(Fence);
        ve_delete_move_constructors(Fence);

#define CONSTRUCTOR \
    ve_vkcheck(vkCreateFence( \
        device.handle, \
        addressof(createInfo), \
        {}, \
        addressof(this->handle) \
    ));

#define DESTRUCTOR \
    vkDestroyFence( \
        device.handle, \
        this->handle, \
        {} \
    );

        Fence() noexcept
        {
        }

        void construct(const VkFenceCreateInfo& createInfo) noexcept
        {
            CONSTRUCTOR;
        }

        void destroy() noexcept
        {
            DESTRUCTOR;
        }

        void reconstruct(const VkFenceCreateInfo& createInfo) noexcept
        {
            DESTRUCTOR;
            CONSTRUCTOR;
        }

        Fence(const VkFenceCreateInfo& createInfo) noexcept
        {
            CONSTRUCTOR;
        }

        ~Fence() noexcept
        {
            DESTRUCTOR;
        }

#undef CONSTRUCTOR
#undef DESTRUCTOR
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

    extern Fence singleUseFence;
}
