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
