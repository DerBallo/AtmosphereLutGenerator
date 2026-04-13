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
    struct Image {
        static constexpr VkImageSubresourceRange subresourceRange {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0u,
            1u,
            0u,
            1u,
        };

        VkImage handle;
        VkDeviceMemory memory;

        ve_delete_copy_constructors(Image);
        ve_delete_move_constructors(Image);

#define CONSTRUCTOR \
    ve_vkcheck(vkCreateImage( \
        device.handle, \
        addressof(createInfo), \
        {}, \
        addressof(this->handle) \
    )); \
\
    VkMemoryRequirements imageMemoryRequirements; \
    vkGetImageMemoryRequirements( \
        device.handle, \
        this->handle, \
        addressof(imageMemoryRequirements) \
    ); \
\
    VkMemoryAllocateInfo memoryAllocateInfo { \
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, \
        {}, \
        imageMemoryRequirements.size, \
        gpu.findMemoryTypeIndex( \
            imageMemoryRequirements.memoryTypeBits, \
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT \
        ), \
    }; \
\
    ve_vkcheck(vkAllocateMemory( \
        device.handle, \
        addressof(memoryAllocateInfo), \
        {}, \
        addressof(this->memory) \
    )); \
\
    ve_vkcheck(vkBindImageMemory( \
        device.handle, \
        this->handle, \
        this->memory, \
        {} \
    ));

#define DESTRUCTOR \
    vkFreeMemory( \
        device.handle, \
        this->memory, \
        {} \
    ); \
\
    vkDestroyImage( \
        device.handle, \
        this->handle, \
        {} \
    );

        Image() noexcept
        {
        }

        void construct(const VkImageCreateInfo& createInfo) noexcept
        {
            CONSTRUCTOR;
        }

        void destroy() noexcept
        {
            DESTRUCTOR;
        }

        void reconstruct(const VkImageCreateInfo& createInfo) noexcept
        {
            DESTRUCTOR;
            CONSTRUCTOR;
        }

        Image(const VkImageCreateInfo& createInfo)
        {
            CONSTRUCTOR;
        }

        ~Image()
        {
            DESTRUCTOR;
        }

#undef CONSTRUCTOR
#undef DESTRUCTOR
    };

    constexpr VkImageCreateInfo makeImageCreateInfo(
        const void* pNext,
        VkImageCreateFlags flags,
        VkImageType imageType,
        VkFormat format,
        VkExtent3D extent,
        uint32_t mipLevels,
        uint32_t arrayLayers,
        VkSampleCountFlagBits samples,
        VkImageTiling tiling,
        VkImageUsageFlags usage,
        VkSharingMode sharingMode,
        ArrayView<uint32_t> queueFamilyIndices,
        VkImageLayout initialLayout
    )
    {
        return {
            VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            pNext,
            flags,
            imageType,
            format,
            extent,
            mipLevels,
            arrayLayers,
            samples,
            tiling,
            usage,
            sharingMode,
            queueFamilyIndices.length,
            queueFamilyIndices.address,
            initialLayout,
        };
    }
}
