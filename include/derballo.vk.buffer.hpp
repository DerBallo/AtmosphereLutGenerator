#pragma once

#include "derballo.vk.command_buffer.hpp"
#include "derballo.vk.device.hpp"
#include "derballo.vk.fence.hpp"

namespace ve {
    enum class BufferType : uint32_t {
        Device,
        DeviceSizeless,
        Host,
        HostSizeless,
        HostUnmapped,
        HostUnmappedSizeless,
    };

    template <BufferType bufferType>
    struct BufferExtension;

    template <>
    struct BufferExtension<BufferType::Device> {
        uint64_t capacity;
    };

    template <>
    struct BufferExtension<BufferType::DeviceSizeless> {
    };

    template <>
    struct BufferExtension<BufferType::Host> {
        uint64_t capacity;
        void* data;
    };

    template <>
    struct BufferExtension<BufferType::HostSizeless> {
        void* data;
    };

    template <>
    struct BufferExtension<BufferType::HostUnmapped> {
        uint64_t capacity;
    };

    template <>
    struct BufferExtension<BufferType::HostUnmappedSizeless> {
    };

    template <BufferType bufferType>
    struct Buffer : BufferExtension<bufferType> {
        VkBuffer handle;
        VkDeviceMemory memory;

        void transferImmediate(VkBuffer dstBuffer, ArrayView<VkBufferCopy> copyRegions)
            requires(
                (bufferType == BufferType::Host
                 || bufferType == BufferType::HostSizeless
                 || bufferType == BufferType::HostUnmapped
                 || bufferType == BufferType::HostUnmappedSizeless)
            )
        {
            ve_vkcheck(vkBeginCommandBuffer(
                singleUseCommandBuffer.handle,
                addressof(commandBufferSingleUseBeginInfo)
            ));

            vkCmdCopyBuffer(
                singleUseCommandBuffer.handle,
                this->handle,
                dstBuffer,
                copyRegions.length,
                copyRegions.address
            );

            ve_vkcheck(vkEndCommandBuffer(
                singleUseCommandBuffer.handle
            ));

            VkSubmitInfo submitInfo {
                VK_STRUCTURE_TYPE_SUBMIT_INFO,
                {},
                {},
                {},
                {},
                1_u32,
                addressof(singleUseCommandBuffer.handle),
                {},
                {},
            };

            ve_vkcheck(vkQueueSubmit(
                device.queue,
                1_u32,
                addressof(submitInfo),
                singleUseFence.handle
            ));

            ve_vkcheck(vkWaitForFences(
                device.handle,
                1_u32,
                addressof(singleUseFence.handle),
                true,
                UINT64_MAX
            ));

            ve_vkcheck(vkResetFences(
                device.handle,
                1_u32,
                addressof(singleUseFence.handle)
            ));

            ve_vkcheck(vkResetCommandBuffer(
                singleUseCommandBuffer.handle,
                0_u32
            ));
        }

        void transferImmediate(VkImage dstImage, VkImageLayout dstImageLayout, ArrayView<VkBufferImageCopy> copyRegions)
            requires(
                (bufferType == BufferType::Host
                 || bufferType == BufferType::HostSizeless
                 || bufferType == BufferType::HostUnmapped
                 || bufferType == BufferType::HostUnmappedSizeless)
            )
        {
            ve_vkcheck(vkBeginCommandBuffer(
                singleUseCommandBuffer.handle,
                addressof(commandBufferSingleUseBeginInfo)
            ));

            VkImageMemoryBarrier accessMemoryBarrier1 {
                VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                {},
                0_u32,
                VK_ACCESS_TRANSFER_WRITE_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                dstImage,
                {
                    VK_IMAGE_ASPECT_COLOR_BIT,
                    0u,
                    1u,
                    0u,
                    1u,
                }
            };

            vkCmdPipelineBarrier(
                singleUseCommandBuffer.handle,
                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0_u32,
                {},
                {},
                {},
                {},
                1_u32,
                addressof(accessMemoryBarrier1)
            );

            vkCmdCopyBufferToImage(
                singleUseCommandBuffer.handle,
                this->handle,
                dstImage,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                copyRegions.length,
                copyRegions.address
            );

            VkImageMemoryBarrier accessMemoryBarrier2 {
                VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                {},
                VK_ACCESS_TRANSFER_WRITE_BIT,
                0_u32,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                dstImageLayout,
                VK_QUEUE_FAMILY_IGNORED,
                VK_QUEUE_FAMILY_IGNORED,
                dstImage,
                {
                    VK_IMAGE_ASPECT_COLOR_BIT,
                    0u,
                    1u,
                    0u,
                    1u,
                }
            };

            vkCmdPipelineBarrier(
                singleUseCommandBuffer.handle,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                0_u32,
                {},
                {},
                {},
                {},
                1_u32,
                addressof(accessMemoryBarrier2)
            );

            ve_vkcheck(vkEndCommandBuffer(
                singleUseCommandBuffer.handle
            ));

            VkSubmitInfo submitInfo {
                VK_STRUCTURE_TYPE_SUBMIT_INFO,
                {},
                {},
                {},
                {},
                1_u32,
                addressof(singleUseCommandBuffer.handle),
                {},
                {},
            };

            ve_vkcheck(vkQueueSubmit(
                device.queue,
                1_u32,
                addressof(submitInfo),
                singleUseFence.handle
            ));

            ve_vkcheck(vkWaitForFences(
                device.handle,
                1_u32,
                addressof(singleUseFence.handle),
                true,
                UINT64_MAX
            ));

            ve_vkcheck(vkResetFences(
                device.handle,
                1_u32,
                addressof(singleUseFence.handle)
            ));

            ve_vkcheck(vkResetCommandBuffer(
                singleUseCommandBuffer.handle,
                0_u32
            ));
        }

        VkDeviceAddress getDeviceAddress() const
            requires(
                bufferType == BufferType::Device
                || bufferType == BufferType::DeviceSizeless
            )
        {
            VkBufferDeviceAddressInfoKHR bufferDeviceAddressInfo {
                VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR,
                {},
                this->handle,
            };

            return device.vkGetBufferDeviceAddress(
                device.handle,
                &bufferDeviceAddressInfo
            );
        }

        void* mapMemory()
            requires(bufferType == BufferType::HostUnmapped)
        {
            void* data;
            ve_check(vkMapMemory(
                device.handle,
                this->memory,
                0_u64,
                this->capacity,
                0_u32,
                &data
            ));
            return data;
        }

        void* mapMemory(uint64_t size)
            requires(bufferType == BufferType::HostUnmappedSizeless)
        {
            void* data;
            ve_vkcheck(vkMapMemory(
                device.handle,
                this->memory,
                0_u64,
                size,
                0_u32,
                addressof(data)
            ));
            return data;
        }

        void unmapMemory()
            requires(
                bufferType == BufferType::HostUnmapped
                || bufferType == BufferType::HostUnmappedSizeless
            )
        {
            vkUnmapMemory(
                device.handle,
                this->memory
            );
        }

        Buffer() = delete;

        ve_delete_copy_constructors(Buffer);

        Buffer(uint64_t bufferCapacity, VkBufferUsageFlags usageFlags)
        {
            if constexpr (
                bufferType == BufferType::Device
                || bufferType == BufferType::Host
                || bufferType == BufferType::HostUnmapped
            ) {
                this->capacity = bufferCapacity;
            }

            if (bufferCapacity == 0_u64) {
                return;
            }

            VkBufferCreateInfo bufferCreateInfo {
                VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                {},
                {},
                bufferCapacity,
                usageFlags,
                VK_SHARING_MODE_EXCLUSIVE,
                {},
                {},
            };

            ve_vkcheck(vkCreateBuffer(
                device.handle,
                addressof(bufferCreateInfo),
                {},
                addressof(this->handle)
            ));

            VkMemoryRequirements bufferMemoryRequirements;
            vkGetBufferMemoryRequirements(
                device.handle,
                this->handle,
                addressof(bufferMemoryRequirements)
            );

            uint32_t memoryTypeIndex;
            if constexpr (
                bufferType == BufferType::Device
                || bufferType == BufferType::DeviceSizeless
            ) {
                memoryTypeIndex = gpu.findMemoryTypeIndex(
                    bufferMemoryRequirements,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
                );
            } else {
                memoryTypeIndex = gpu.findMemoryTypeIndex(
                    bufferMemoryRequirements,
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                );
            }

            static constexpr VkMemoryAllocateFlagsInfo memoryAllocateFlagsInfo {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO,
                {},
                VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT,
                {},
            };

            VkMemoryAllocateInfo memoryAllocateInfo {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                addressof(memoryAllocateFlagsInfo),
                bufferMemoryRequirements.size,
                memoryTypeIndex,
            };

            ve_vkcheck(vkAllocateMemory(
                device.handle,
                addressof(memoryAllocateInfo),
                {},
                addressof(this->memory)
            ));

            ve_vkcheck(vkBindBufferMemory(
                device.handle,
                this->handle,
                this->memory,
                {}
            ));

            if constexpr (
                bufferType == BufferType::Host
                || bufferType == BufferType::HostSizeless
            ) {
                ve_vkcheck(vkMapMemory(
                    device.handle,
                    this->memory,
                    0_u64,
                    bufferCapacity,
                    0_u32,
                    addressof(this->data)
                ));
            }
        }

        Buffer(Buffer<bufferType>&& other) noexcept
        {
            if constexpr (
                bufferType == BufferType::Device
                || bufferType == BufferType::Host
                || bufferType == BufferType::HostUnmapped
            ) {
                this->capacity = other.capacity;
                other.capacity = 0_u64;
            }
            if constexpr (
                bufferType == BufferType::Host
                || bufferType == BufferType::HostSizeless
            ) {
                this->data = other.data;
                other.data = nullptr;
            }
            this->handle = other.handle;
            other.handle = nullptr;
            this->memory = other.memory;
            other.memory = nullptr;
        }

        Buffer& operator=(Buffer<bufferType>&& other) noexcept
        {
            if constexpr (
                bufferType == BufferType::Device
                || bufferType == BufferType::Host
                || bufferType == BufferType::HostUnmapped
            ) {
                this->capacity = other.capacity;
                other.capacity = 0_u64;
            }
            if constexpr (
                bufferType == BufferType::Host
                || bufferType == BufferType::HostSizeless
            ) {
                this->data = other.data;
                other.data = nullptr;
            }
            this->handle = other.handle;
            other.handle = nullptr;
            this->memory = other.memory;
            other.memory = nullptr;
            return *this;
        }

        ~Buffer()
        {
            if constexpr (
                bufferType == BufferType::Host
                || bufferType == BufferType::HostSizeless
            ) {
                vkUnmapMemory(
                    device.handle,
                    this->memory
                );
            }

            vkFreeMemory(
                device.handle,
                this->memory,
                {}
            );

            vkDestroyBuffer(
                device.handle,
                this->handle,
                {}
            );
        }
    };
}
