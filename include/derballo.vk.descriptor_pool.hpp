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

#include "derballo.containers.array_view.hpp"
#include "derballo.vk.device.hpp"

namespace ve {
    struct DescriptorPool {
        VkDescriptorPool handle;

        void freeDescriptorSets(ArrayView<VkDescriptorSet> descriptorSets)
        {
            vkFreeDescriptorSets(
                device.handle,
                this->handle,
                descriptorSets.length,
                descriptorSets.address
            );
        }

        DescriptorPool() = delete;
        ve_delete_copy_constructors(DescriptorPool);
        ve_delete_move_constructors(DescriptorPool);

        DescriptorPool(const VkDescriptorPoolCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateDescriptorPool(
                device.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));
        }

        ~DescriptorPool()
        {
            vkDestroyDescriptorPool(
                device.handle,
                this->handle,
                {}
            );
        }
    };

    constexpr VkDescriptorPoolCreateInfo makeDescriptorPoolCreateInfo(
        const void* pNext,
        VkDescriptorPoolCreateFlags flags,
        uint32_t maxSets,
        ArrayView<VkDescriptorPoolSize> poolSizes
    )
    {
        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            pNext,
            flags,
            maxSets,
            static_cast<uint32_t>(poolSizes.length),
            poolSizes.address,
        };
    }
}
