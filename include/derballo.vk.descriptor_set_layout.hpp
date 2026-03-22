#pragma once

#include "derballo.containers.array_view.hpp"
#include "derballo.vk.device.hpp"

namespace ve {
    struct DescriptorSetLayout {
        VkDescriptorSetLayout handle;

        DescriptorSetLayout() = delete;
        ve_delete_copy_constructors(DescriptorSetLayout);
        ve_delete_move_constructors(DescriptorSetLayout);

        DescriptorSetLayout(const VkDescriptorSetLayoutCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateDescriptorSetLayout(
                device.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));
        }

        ~DescriptorSetLayout()
        {
            vkDestroyDescriptorSetLayout(
                device.handle,
                this->handle,
                {}
            );
        }
    };

    constexpr VkDescriptorSetLayoutCreateInfo makeDescriptorSetLayoutCreateInfo(
        const void* pNext,
        VkDescriptorSetLayoutCreateFlags flags,
        ArrayView<VkDescriptorSetLayoutBinding> bindings
    )
    {
        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            pNext,
            flags,
            static_cast<uint32_t>(bindings.length),
            bindings.address,
        };
    }

    constexpr VkDescriptorSetLayoutBindingFlagsCreateInfo makeDescriptorSetLayoutBindingFlagsCreateInfo(
        const void* pNext,
        ArrayView<VkDescriptorBindingFlags> bindingFlags
    )
    {
        return {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO,
            pNext,
            static_cast<uint32_t>(bindingFlags.length),
            bindingFlags.address,
        };
    }
}
