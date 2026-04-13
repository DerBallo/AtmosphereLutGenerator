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

#include "derballo.utils.hpp"
#include "derballo.vk.command_buffer.hpp"
#include "derballo.vk.device.hpp"
#include "derballo.vk.fence.hpp"
#include "derballo.vk.image.hpp"

namespace ve {
    struct NoBufferFeature { };

    using DeviceBufferFeatureFlags = uint32_t;

    enum DeviceBufferFeatureFlagBits : DeviceBufferFeatureFlags {
        DEVICE_BUFFER_FEATURE_NONE = 0_u32,
        DEVICE_BUFFER_FEATURE_CAPACITY_BIT = 1_u32 << 0_u32,
    };

    struct DeviceBufferFeatureCapacity {
        VkDeviceSize capacity;
    };

    template <DeviceBufferFeatureFlags Features>
    struct DeviceBuffer
        : std::conditional_t<
              has_flag(Features, DEVICE_BUFFER_FEATURE_CAPACITY_BIT),
              DeviceBufferFeatureCapacity,
              NoBufferFeature> {
        static constexpr VkMemoryAllocateFlagsInfo deviceAddressAllocateFlagsInfo {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
            {},
            VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT,
            {},
        };

        VkBuffer handle;
        VkDeviceMemory memory;

        VkDeviceAddress getDeviceAddress() const
        {
            VkBufferDeviceAddressInfoKHR bufferDeviceAddressInfo {
                VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR,
                {},
                this->handle,
            };

            return device.vkGetBufferDeviceAddress(
                device.handle,
                addressof(bufferDeviceAddressInfo)
            );
        }

        ve_delete_copy_constructors(DeviceBuffer);
        ve_delete_move_constructors(DeviceBuffer);

#define CONSTRUCTOR \
    if constexpr ( \
        has_flag(Features, DEVICE_BUFFER_FEATURE_CAPACITY_BIT) \
    ) { \
        this->capacity = createInfo.size; \
    } \
\
    ve_vkcheck(vkCreateBuffer( \
        device.handle, \
        addressof(createInfo), \
        {}, \
        addressof(this->handle) \
    )); \
\
    VkMemoryRequirements bufferMemoryRequirements; \
    vkGetBufferMemoryRequirements( \
        device.handle, \
        this->handle, \
        addressof(bufferMemoryRequirements) \
    ); \
\
    VkMemoryAllocateInfo memoryAllocateInfo { \
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, \
        has_flag(createInfo.usage, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT) \
            ? addressof(deviceAddressAllocateFlagsInfo) \
            : nullptr, \
        bufferMemoryRequirements.size, \
        gpu.findMemoryTypeIndex( \
            bufferMemoryRequirements.memoryTypeBits, \
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
    ve_vkcheck(vkBindBufferMemory( \
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
    vkDestroyBuffer( \
        device.handle, \
        this->handle, \
        {} \
    );

        DeviceBuffer() noexcept
        {
        }

        void construct(const VkBufferCreateInfo& createInfo) noexcept
        {
            CONSTRUCTOR;
        }

        void destruct() noexcept
        {
            DESTRUCTOR;
        }

        void reconstruct(const VkBufferCreateInfo& createInfo) noexcept
        {
            DESTRUCTOR;
            CONSTRUCTOR;
        }

        DeviceBuffer(const VkBufferCreateInfo& createInfo)
        {
            CONSTRUCTOR
        }

        ~DeviceBuffer()
        {
            DESTRUCTOR
        }

#undef CONSTRUCTOR
#undef DESTRUCTOR
    };

    using HostBufferFeatureFlags = uint32_t;

    enum HostBufferFeatureFlagBits : HostBufferFeatureFlags {
        HOST_BUFFER_FEATURE_NONE = 0_u32,
        HOST_BUFFER_FEATURE_CAPACITY_BIT = 1_u32 << 0_u32,
        HOST_BUFFER_FEATURE_MAPPED_BIT = 1_u32 << 1_u32,
    };

    struct HostBufferFeatureCapacity {
        VkDeviceSize capacity;
    };

    struct HostBufferFeatureMapped {
        void* data;
    };

    template <HostBufferFeatureFlags Features> struct HostBuffer
        : std::conditional_t<
              has_flag(Features, HOST_BUFFER_FEATURE_CAPACITY_BIT),
              HostBufferFeatureCapacity,
              NoBufferFeature>,
          std::conditional_t<
              has_flag(Features, HOST_BUFFER_FEATURE_MAPPED_BIT),
              HostBufferFeatureMapped,
              NoBufferFeature> {
        VkBuffer handle;
        VkDeviceMemory memory;

        void* mapMemory(VkDeviceSize size)
            requires(
                not has_flag(Features, HOST_BUFFER_FEATURE_CAPACITY_BIT)
            )
        {
            void* data;
            ve_vkcheck(vkMapMemory(
                device.handle,
                this->memory,
                {},
                size,
                {},
                addressof(data)
            ));
            return data;
        }

        void* mapMemory()
            requires(
                has_flag(Features, HOST_BUFFER_FEATURE_CAPACITY_BIT)
            )
        {
            void* data;
            ve_vkcheck(vkMapMemory(
                device.handle,
                this->memory,
                {},
                this->capacity,
                {},
                addressof(data)
            ));
            return data;
        }

        void unmapMemory()
            requires(
                has_flag(Features, HOST_BUFFER_FEATURE_MAPPED_BIT)
            )
        {
            vkUnmapMemory(
                device.handle,
                this->memory
            );
        }

        void transferImmediate(VkBuffer dstBuffer, ArrayView<VkBufferCopy> copyRegions)
        {
            singleUseCommandBuffer.begin(
                CommandBuffer::defaultOneTimeSubmitBeginInfo
            );

            singleUseCommandBuffer.cmdCopyBuffer(
                this->handle,
                dstBuffer,
                copyRegions
            );

            singleUseCommandBuffer.end();

            device.mainQueueSubmit(
                {
                    makeSubmitInfo(
                        {},
                        {},
                        {
                            singleUseCommandBuffer.handle,
                        },
                        {}
                    ),
                },
                singleUseFence.handle
            );

            singleUseFence.wait(
                UINT64_MAX
            );

            singleUseFence.reset();

            singleUseCommandBuffer.reset(
                {}
            );
        }

        void transferImmediate(VkImage dstImage, VkImageLayout dstImageLayout, ArrayView<VkBufferImageCopy> copyRegions)
        {
            singleUseCommandBuffer.begin(
                CommandBuffer::defaultOneTimeSubmitBeginInfo
            );

            singleUseCommandBuffer.cmdPipelineBarrier(
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                {},
                {},
                {},
                {
                    {
                        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                        {},
                        {},
                        VK_ACCESS_TRANSFER_WRITE_BIT,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_QUEUE_FAMILY_IGNORED,
                        VK_QUEUE_FAMILY_IGNORED,
                        dstImage,
                        Image::subresourceRange,
                    },
                }
            );

            singleUseCommandBuffer.cmdCopyBufferToImage(
                this->handle,
                dstImage,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                copyRegions
            );

            singleUseCommandBuffer.cmdPipelineBarrier(
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                {},
                {},
                {},
                {
                    {
                        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                        {},
                        VK_ACCESS_TRANSFER_WRITE_BIT,
                        {},
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        dstImageLayout,
                        VK_QUEUE_FAMILY_IGNORED,
                        VK_QUEUE_FAMILY_IGNORED,
                        dstImage,
                        Image::subresourceRange,
                    },
                }
            );

            singleUseCommandBuffer.end();

            device.mainQueueSubmit(
                {
                    makeSubmitInfo(
                        {},
                        {},
                        {
                            singleUseCommandBuffer.handle,
                        },
                        {}
                    ),
                },
                singleUseFence.handle
            );

            singleUseFence.wait(
                UINT64_MAX
            );

            singleUseFence.reset();

            singleUseCommandBuffer.reset(
                {}
            );
        }

        ve_delete_copy_constructors(HostBuffer);
        ve_delete_move_constructors(HostBuffer);

#define CONSTRUCTOR \
    if constexpr ( \
        has_flag(Features, HOST_BUFFER_FEATURE_CAPACITY_BIT) \
    ) { \
        this->capacity = createInfo.size; \
    } \
\
    ve_vkcheck(vkCreateBuffer( \
        device.handle, \
        addressof(createInfo), \
        {}, \
        addressof(this->handle) \
    )); \
\
    VkMemoryRequirements bufferMemoryRequirements; \
    vkGetBufferMemoryRequirements( \
        device.handle, \
        this->handle, \
        addressof(bufferMemoryRequirements) \
    ); \
\
    VkMemoryAllocateInfo memoryAllocateInfo { \
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, \
        {}, \
        bufferMemoryRequirements.size, \
        gpu.findMemoryTypeIndex( \
            bufferMemoryRequirements.memoryTypeBits, \
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT \
                | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT \
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
    ve_vkcheck(vkBindBufferMemory( \
        device.handle, \
        this->handle, \
        this->memory, \
        {} \
    )); \
\
    if constexpr ( \
        has_flag(Features, HOST_BUFFER_FEATURE_MAPPED_BIT) \
    ) { \
        ve_vkcheck(vkMapMemory( \
            device.handle, \
            this->memory, \
            {}, \
            createInfo.size, \
            {}, \
            addressof(this->data) \
        )); \
    }

#define DESTRUCTOR \
    if constexpr ( \
        has_flag(Features, HOST_BUFFER_FEATURE_MAPPED_BIT) \
    ) { \
        vkUnmapMemory( \
            device.handle, \
            this->memory \
        ); \
    } \
\
    vkFreeMemory( \
        device.handle, \
        this->memory, \
        {} \
    ); \
\
    vkDestroyBuffer( \
        device.handle, \
        this->handle, \
        {} \
    );

        HostBuffer() noexcept
        {
        }

        void construct(const VkBufferCreateInfo& createInfo) noexcept
        {
            CONSTRUCTOR;
        }

        void destroy() noexcept
        {
            DESTRUCTOR;
        }

        void reconstruct(const VkBufferCreateInfo& createInfo) noexcept
        {
            DESTRUCTOR;
            CONSTRUCTOR;
        }

        HostBuffer(const VkBufferCreateInfo& createInfo)
        {
            CONSTRUCTOR
        }

        ~HostBuffer()
        {
            DESTRUCTOR
        }

#undef CONSTRUCTOR
#undef DESTRUCTOR
    };

    constexpr VkBufferCreateInfo makeBufferCreateInfo(
        const void* pNext,
        VkBufferCreateFlags flags,
        VkDeviceSize size,
        VkBufferUsageFlags usage,
        VkSharingMode sharingMode,
        ArrayView<uint32_t> queueFamilyIndices
    )
    {
        return {
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            pNext,
            flags,
            size,
            usage,
            sharingMode,
            queueFamilyIndices.length,
            queueFamilyIndices.address,
        };
    }

    extern HostBuffer<HOST_BUFFER_FEATURE_CAPACITY_BIT | HOST_BUFFER_FEATURE_MAPPED_BIT> singleUseStagingBuffer;
}
