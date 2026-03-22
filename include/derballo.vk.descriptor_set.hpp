#pragma once

#include "derballo.containers.array_view.hpp"
#include "derballo.debug.macros.hpp"
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
