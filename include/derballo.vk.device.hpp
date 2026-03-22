#pragma once

#include "derballo.containers.array_view.hpp"
#include "derballo.vk.gpu.hpp"

namespace ve {
    struct Device {
        VkDevice handle;
        VkQueue queue;
        PFN_vkQueuePresentKHR vkQueuePresentKHR;
        PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
        PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
        PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
        PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
        PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
        PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
        PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
        PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
        PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
        PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
        PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
        PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
        PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
#ifndef NDEBUG
        PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
#endif

        void queueWaitIdle()
        {
            ve_vkcheck_switch(
                vkQueueSubmit(this->queue, 0_u32, nullptr, VK_NULL_HANDLE),
                ve_check_case(VK_SUCCESS, {
                    ve_vkcheck(vkQueueWaitIdle(
                        this->queue
                    ));
                });
            );
        }

#ifndef NDEBUG
        void setObjectDebugName(VkObjectType type, uint64_t handle, const char* name)
        {
            VkDebugUtilsObjectNameInfoEXT info {
                VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
                {},
                type,
                handle,
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
                0_u32,
                0_u32,
                addressof(this->queue)
            );

#define ve_load_device_function(name) name = (PFN_##name)vkGetDeviceProcAddr(this->handle, #name)
            ve_load_device_function(vkQueuePresentKHR);
            ve_load_device_function(vkAcquireNextImageKHR);
            ve_load_device_function(vkDestroySwapchainKHR);
            ve_load_device_function(vkGetSwapchainImagesKHR);
            ve_load_device_function(vkCreateSwapchainKHR);
            ve_load_device_function(vkDestroyAccelerationStructureKHR);
            ve_load_device_function(vkCreateRayTracingPipelinesKHR);
            ve_load_device_function(vkGetRayTracingShaderGroupHandlesKHR);
            ve_load_device_function(vkGetBufferDeviceAddress);
            ve_load_device_function(vkCreateAccelerationStructureKHR);
            ve_load_device_function(vkGetAccelerationStructureBuildSizesKHR);
            ve_load_device_function(vkGetAccelerationStructureDeviceAddressKHR);
            ve_load_device_function(vkCmdBuildAccelerationStructuresKHR);
            ve_load_device_function(vkCmdTraceRaysKHR);
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
            static_cast<uint32_t>(queueCreateInfos.length),
            queueCreateInfos.address,
            static_cast<uint32_t>(enabledLayerNames.length),
            enabledLayerNames.address,
            static_cast<uint32_t>(enabledExtensionNames.length),
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
            static_cast<uint32_t>(queuePriorities.length),
            queuePriorities.address,
        };
    }

    extern Device device;
}
