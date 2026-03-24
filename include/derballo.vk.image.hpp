#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct Image {
        VkImage handle;
        VkDeviceMemory memory;

        Image() = delete;
        ve_delete_copy_constructors(Image);
        ve_delete_move_constructors(Image);

        Image(const VkImageCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateImage(
                device.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));

            VkMemoryRequirements imageMemoryRequirements;
            vkGetImageMemoryRequirements(
                device.handle,
                this->handle,
                addressof(imageMemoryRequirements)
            );

            uint32_t memoryTypeIndex {
                gpu.findMemoryTypeIndex(imageMemoryRequirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
            };

            VkMemoryAllocateInfo memoryAllocateInfo {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                {},
                imageMemoryRequirements.size,
                memoryTypeIndex,
            };

            ve_vkcheck(vkAllocateMemory(
                device.handle,
                addressof(memoryAllocateInfo),
                {},
                addressof(this->memory)
            ));

            ve_vkcheck(vkBindImageMemory(
                device.handle,
                this->handle,
                this->memory,
                {}
            ));
        }

        ~Image()
        {
            vkFreeMemory(
                device.handle,
                this->memory,
                {}
            );

            vkDestroyImage(
                device.handle,
                this->handle,
                {}
            );
        }
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
            static_cast<uint32_t>(queueFamilyIndices.length),
            queueFamilyIndices.address,
            initialLayout,
        };
    }
}
