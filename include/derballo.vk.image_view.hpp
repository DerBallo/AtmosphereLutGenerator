#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct ImageView {
        VkImageView handle;

        ImageView() = delete;
        ve_delete_copy_constructors(ImageView);
        ve_delete_move_constructors(ImageView);

        ImageView(const VkImageViewCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateImageView(
                device.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));
        }

        ~ImageView()
        {
            vkDestroyImageView(
                device.handle,
                this->handle,
                {}
            );
        }
    };

    constexpr VkImageViewCreateInfo makeImageViewCreateInfo(
        const void* pNext,
        VkImageViewCreateFlags flags,
        VkImage image,
        VkImageViewType viewType,
        VkFormat format,
        VkComponentMapping components,
        VkImageSubresourceRange subresourceRange
    )
    {
        return {
            VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            pNext,
            flags,
            image,
            viewType,
            format,
            components,
            subresourceRange,
        };
    }
}
