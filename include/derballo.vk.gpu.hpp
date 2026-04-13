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
#include "derballo.vk.instance.hpp"

namespace ve {
    struct Gpu {
        VkPhysicalDevice handle;
        uint32_t memoryTypeCount;
        VkMemoryType memoryTypes[VK_MAX_MEMORY_TYPES];
        uint32_t queueFamilyIndex;

        uint32_t findMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags flags)
        {
            for (uint32_t i {}; i < memoryTypeCount; i++) {
                if ((memoryTypeBits & (1 << i))
                    && (memoryTypes[i].propertyFlags & flags) == flags) {
                    return i;
                }
            }
            ve_fail("No memory type index found with requested flags!");
        }

        VkSurfaceCapabilitiesKHR getSurfaceCapabilities(VkSurfaceKHR surface)
        {
            VkSurfaceCapabilitiesKHR surfaceCapabilities;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
                this->handle,
                surface,
                addressof(surfaceCapabilities)
            );

            return surfaceCapabilities;
        }

        Gpu() = delete;
        ve_delete_copy_constructors(Gpu);
        ve_delete_move_constructors(Gpu);

        Gpu(uint32_t gpuIndex)
        {
            uint32_t count;
            ve_vkcheck(vkEnumeratePhysicalDevices(
                vulkanInstance.handle,
                addressof(count),
                {}
            ));

            VkPhysicalDevice gpus[count];
            ve_vkcheck(vkEnumeratePhysicalDevices(
                vulkanInstance.handle,
                addressof(count),
                gpus
            ));

#ifndef NDEBUG
            for (uint32_t i {}; i < count; i++) {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(
                    gpus[i],
                    addressof(properties)
                );

                std::println(ANSI_MESG "Device {}:" ANSI_RSET " {}\n    Driver Version: {}\n    Api Version: {}.{}.{}.{}", i, properties.deviceName, properties.driverVersion, VK_API_VERSION_VARIANT(properties.apiVersion), VK_API_VERSION_MAJOR(properties.apiVersion), VK_API_VERSION_MINOR(properties.apiVersion), VK_API_VERSION_PATCH(properties.apiVersion));
            }

            std::println(ANSI_SCCS "Using Device {}!" ANSI_RSET, gpuIndex);
#endif

            this->handle = gpus[gpuIndex];

            VkPhysicalDeviceMemoryProperties memoryProperties;

            vkGetPhysicalDeviceMemoryProperties(
                this->handle,
                addressof(memoryProperties)
            );

            this->memoryTypeCount = memoryProperties.memoryTypeCount;
            for (uint32_t i {}; i < this->memoryTypeCount; ++i) {
                this->memoryTypes[i] = memoryProperties.memoryTypes[i];
            }

            uint32_t queueFamilyCount {};
            vkGetPhysicalDeviceQueueFamilyProperties(
                this->handle,
                addressof(queueFamilyCount),
                {}
            );

            VkQueueFamilyProperties queueFamilyProperties[queueFamilyCount];
            vkGetPhysicalDeviceQueueFamilyProperties(
                this->handle,
                addressof(queueFamilyCount),
                queueFamilyProperties
            );

            for (uint32_t i {}; i < queueFamilyCount; ++i) {
                if (
                    static_cast<bool>(queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
                    && static_cast<bool>(queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
                ) {
                    this->queueFamilyIndex = i;
                    return;
                }
            }

            ve_fail("No compatible queue family index found!");
        }
    };

    extern Gpu gpu;
}
