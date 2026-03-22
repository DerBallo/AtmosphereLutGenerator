#pragma once

#include "derballo.containers.array_view.hpp"
#include "derballo.vk.device.hpp"

namespace ve {
    struct PipelineLayout {
        VkPipelineLayout handle;

        PipelineLayout() = delete;
        ve_delete_copy_constructors(PipelineLayout);
        ve_delete_move_constructors(PipelineLayout);

        PipelineLayout(const VkPipelineLayoutCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreatePipelineLayout(
                device.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));
        }

        ~PipelineLayout()
        {
            vkDestroyPipelineLayout(
                device.handle,
                this->handle,
                nullptr
            );
        }
    };

    constexpr VkPipelineLayoutCreateInfo makePipelineLayoutCreateInfo(
        const void* pNext,
        VkPipelineLayoutCreateFlags flags,
        ArrayView<VkDescriptorSetLayout> setLayouts,
        ArrayView<VkPushConstantRange> pushConstantranges
    )
    {
        return {
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            pNext,
            flags,
            static_cast<uint32_t>(setLayouts.length),
            setLayouts.address,
            static_cast<uint32_t>(pushConstantranges.length),
            pushConstantranges.address,
        };
    }
}
