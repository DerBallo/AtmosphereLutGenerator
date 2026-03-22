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
