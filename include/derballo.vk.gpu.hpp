#pragma once

#ifndef ve_vulkan_core_h_included
    #define ve_vulkan_core_h_included
    #include <vulkan/vulkan_core.h>
#endif
#include "derballo.debug.macros.hpp"
#include "derballo.utils.ptr.hpp"
#include "derballo.vk.instance.hpp"

namespace ve {
    struct Gpu {
        VkPhysicalDevice handle;
        float gpuTimestampPeriod;
        uint32_t memoryTypeCount;
        VkMemoryType memoryTypes[VK_MAX_MEMORY_TYPES];
        VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingPipelineProperties;
        uint32_t shaderGroupHandleStride;

        uint32_t findMemoryTypeIndex(VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags flags)
        {
            uint32_t memoryTypeIndex {};
            for (; memoryTypeIndex < memoryTypeCount; memoryTypeIndex++) {
                if (
                    (memoryRequirements.memoryTypeBits & (1_u32 << memoryTypeIndex))
                    && ((memoryTypes[memoryTypeIndex].propertyFlags & flags) == flags)
                ) {
                    break;
                }
            }
            ve_assert(memoryTypeIndex < memoryTypeCount);
            return memoryTypeIndex;
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
                &count,
                nullptr
            ));
            VkPhysicalDevice gpus[count];
            ve_vkcheck(vkEnumeratePhysicalDevices(
                vulkanInstance.handle,
                &count,
                gpus
            ));

            for (uint32_t i {}; i < count; i++) {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(
                    gpus[i],
                    &properties
                );
                std::println(ANSI_MESG "Device {}: {}\n    Driver Version: {}\n    Api Version: {}" ANSI_RSET, i, properties.deviceName, properties.driverVersion, properties.apiVersion);
            }

            std::println(ANSI_SCCS "Using Device {}!" ANSI_RSET, gpuIndex);
            this->handle = gpus[gpuIndex];

            this->rayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
            this->rayTracingPipelineProperties.pNext = nullptr;
            VkPhysicalDeviceProperties2 gpuProperties2;
            gpuProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
            gpuProperties2.pNext = &this->rayTracingPipelineProperties;
            vkGetPhysicalDeviceProperties2(
                this->handle,
                &gpuProperties2
            );

            this->gpuTimestampPeriod = gpuProperties2.properties.limits.timestampPeriod;
            this->shaderGroupHandleStride = alignup(
                this->rayTracingPipelineProperties.shaderGroupHandleSize,
                this->rayTracingPipelineProperties.shaderGroupBaseAlignment
            );

            VkPhysicalDeviceMemoryProperties gpuMemoryProperties;
            vkGetPhysicalDeviceMemoryProperties(
                this->handle,
                &gpuMemoryProperties
            );

            this->memoryTypeCount = gpuMemoryProperties.memoryTypeCount;

            for (uint32_t i {}; i < memoryTypeCount; ++i) {
                this->memoryTypes[i] = gpuMemoryProperties.memoryTypes[i];
            }
        }
    };

    extern Gpu gpu;
}
