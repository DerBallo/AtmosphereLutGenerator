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
    struct DescriptorSet {
        VkDescriptorSet handle;

        DescriptorSet() = delete;
        ve_delete_copy_constructors(DescriptorSet);
        ve_delete_move_constructors(DescriptorSet);

        DescriptorSet(const VkDescriptorSetAllocateInfo& allocateInfo)
        {
            ve_vkcheck(vkAllocateDescriptorSets(
                device.handle,
                addressof(allocateInfo),
                addressof(this->handle)
            ));
        }
    };

    template <uint32_t N1>
    struct DescriptorSets {
        VkDescriptorSet handles[N1];

        constexpr uint32_t count() const noexcept { return N1; }

        DescriptorSets() = delete;
        ve_delete_copy_constructors(DescriptorSets);
        ve_delete_move_constructors(DescriptorSets);

        DescriptorSets(const VkDescriptorSetAllocateInfo& allocateInfo)
        {
            ve_check(vkAllocateDescriptorSets(
                device.handle,
                addressof(allocateInfo),
                this->handles
            ));
        }
    };

    constexpr VkDescriptorSetAllocateInfo makeDescriptorSetAllocateInfo(
        const void* pNext,
        VkDescriptorPool descriptorPool,
        ArrayView<VkDescriptorSetLayout> setLayouts
    )
    {
        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            pNext,
            descriptorPool,
            static_cast<uint32_t>(setLayouts.length),
            setLayouts.address,
        };
    }

    constexpr VkDescriptorSetVariableDescriptorCountAllocateInfo makeDescriptorSetsVariableDescriptorCountAllocateInfo(
        const void* pNext,
        ArrayView<uint32_t> descriptorCounts
    )
    {
        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO,
            pNext,
            static_cast<uint32_t>(descriptorCounts.length),
            descriptorCounts.address,
        };
    }
}
