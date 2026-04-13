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

#include "derballo.containers.array_view.hpp"
#include "derballo.vk.gpu.hpp"

namespace ve {
    struct Device {
        VkDevice handle;
        VkQueue mainQueue;
        PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
#ifndef NDEBUG
        PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
#endif

        void mainQueueWaitIdle()
        {
            ve_vkcheck_switch(
                vkQueueSubmit(
                    this->mainQueue,
                    {},
                    {},
                    VK_NULL_HANDLE
                ),
                ve_check_case(VK_SUCCESS, {
                    ve_vkcheck(vkQueueWaitIdle(
                        this->mainQueue
                    ));
                });
            );
        }

        void mainQueueSubmit(ArrayView<VkSubmitInfo> submits, VkFence fence)
        {
            ve_vkcheck(vkQueueSubmit(
                this->mainQueue,
                submits.length,
                submits.address,
                fence
            ));
        }

#ifndef NDEBUG
        void setObjectDebugName(VkObjectType type, uint64_t objectHandle, const char* name)
        {
            VkDebugUtilsObjectNameInfoEXT info {
                VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                {},
                type,
                objectHandle,
                name,
            };

            this->vkSetDebugUtilsObjectNameEXT(
                this->handle,
                &info
            );
        }
#endif

        Device() = delete;
        ve_delete_copy_constructors(Device);
        ve_delete_move_constructors(Device);

        Device(const VkDeviceCreateInfo& createInfo)
        {
#ifndef NDEBUG
            if (createInfo.enabledLayerCount > 0_u32) {
                std::println(ANSI_MESG "Checking device layers..." ANSI_RSET);

                uint32_t availableDeviceLayerCount {};
                ve_vkcheck(vkEnumerateDeviceLayerProperties(
                    gpu.handle,
                    addressof(availableDeviceLayerCount),
                    {}
                ));
                VkLayerProperties availableDeviceLayers[availableDeviceLayerCount];
                ve_vkcheck(vkEnumerateDeviceLayerProperties(
                    gpu.handle,
                    addressof(availableDeviceLayerCount),
                    availableDeviceLayers
                ));

                for (uint32_t i {}; i < createInfo.enabledLayerCount; i++) {
                    const char* layerName { createInfo.ppEnabledLayerNames[i] };
                    bool foundLayer { false };
                    for (uint32_t j {}; j < availableDeviceLayerCount; j++) {
                        foundLayer |= (strcmp(availableDeviceLayers[j].layerName, layerName) == 0);
                    }

                    if (foundLayer) {
                        std::println("    Layer \"{}\": " ANSI_SCCS "supported" ANSI_RSET, layerName);
                    } else {
                        std::println("    Layer \"{}\": " ANSI_ERRR "unsupported" ANSI_RSET, layerName);
                    }
                }
            }

            if (createInfo.enabledExtensionCount > 0_u32) {
                std::println(ANSI_MESG "Checking device extensions..." ANSI_RSET);

                uint32_t availableDeviceExtensionsCount {};
                ve_vkcheck(vkEnumerateDeviceExtensionProperties(
                    gpu.handle,
                    {},
                    addressof(availableDeviceExtensionsCount),
                    {}
                ));
                VkExtensionProperties availableDeviceExtensions[availableDeviceExtensionsCount];
                ve_vkcheck(vkEnumerateDeviceExtensionProperties(
                    gpu.handle,
                    {},
                    addressof(availableDeviceExtensionsCount),
                    availableDeviceExtensions
                ));

                for (uint32_t i {}; i < createInfo.enabledExtensionCount; i++) {
                    const char* extensionName { createInfo.ppEnabledExtensionNames[i] };
                    bool foundExtension { false };
                    for (uint32_t j {}; j < availableDeviceExtensionsCount; j++) {
                        foundExtension |= (strcmp(availableDeviceExtensions[j].extensionName, extensionName) == 0);
                    }

                    if (foundExtension) {
                        std::println("    Extension \"{}\": " ANSI_SCCS "supported" ANSI_RSET, extensionName);
                    } else {
                        std::println("    Extension \"{}\": " ANSI_ERRR "unsupported" ANSI_RSET, extensionName);
                    }
                }
            }
#endif

            ve_vkcheck(vkCreateDevice(
                gpu.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));

            vkGetDeviceQueue(
                this->handle,
                gpu.queueFamilyIndex,
                0_u32,
                addressof(this->mainQueue)
            );

#define ve_load_device_function(name) name = (PFN_##name)vkGetDeviceProcAddr(this->handle, #name)
            ve_load_device_function(vkGetBufferDeviceAddress);
#ifndef NDEBUG
            ve_load_device_function(vkSetDebugUtilsObjectNameEXT);
#endif
#undef ve_load_device_function
        }

        ~Device()
        {
            vkDestroyDevice(
                this->handle,
                nullptr
            );
        }
    };

    constexpr VkDeviceCreateInfo makeDeviceCreateInfo(
        const void* pNext,
        VkDeviceCreateFlags flags,
        ArrayView<VkDeviceQueueCreateInfo> queueCreateInfos,
        ArrayView<const char*> enabledLayerNames,
        ArrayView<const char*> enabledExtensionNames
    )
    {
        return {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            pNext,
            flags,
            queueCreateInfos.length,
            queueCreateInfos.address,
            enabledLayerNames.length,
            enabledLayerNames.address,
            enabledExtensionNames.length,
            enabledExtensionNames.address,
            {},
        };
    }

    constexpr VkDeviceQueueCreateInfo makeDeviceQueueCreateInfo(
        const void* pNext,
        VkDeviceQueueCreateFlags flags,
        uint32_t queueFamilyIndex,
        ArrayView<float> queuePriorities
    )
    {
        return {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            pNext,
            flags,
            queueFamilyIndex,
            queuePriorities.length,
            queuePriorities.address,
        };
    }

    constexpr VkSubmitInfo makeSubmitInfo(
        const void* pNext,
        DualArrayView<VkSemaphore, VkPipelineStageFlags> waitSemaphoresAndWaitDstStageMasks,
        ArrayView<VkCommandBuffer> commandBuffers,
        ArrayView<VkSemaphore> signalSemaphores
    )
    {
        return {
            VK_STRUCTURE_TYPE_SUBMIT_INFO,
            pNext,
            waitSemaphoresAndWaitDstStageMasks.length,
            waitSemaphoresAndWaitDstStageMasks.address1,
            waitSemaphoresAndWaitDstStageMasks.address2,
            commandBuffers.length,
            commandBuffers.address,
            signalSemaphores.length,
            signalSemaphores.address,
        };
    }

    extern Device device;
}
