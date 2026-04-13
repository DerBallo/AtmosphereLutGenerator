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

namespace ve {
    struct VulkanInstance {
        VkInstance handle;
#ifndef NDEBUG
        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
        VkDebugUtilsMessengerEXT debugMessenger;

        static VkBool32 debugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT msgSeverity, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void*)
        {
            switch (msgSeverity) {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: std::println("{}", pCallbackData->pMessage); break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:    std::println("{}", pCallbackData->pMessage); break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: std::println(ANSI_WARN "{}" ANSI_RSET, pCallbackData->pMessage); break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:   std::println(ANSI_ERRR "{}" ANSI_RSET, pCallbackData->pMessage); break;
                default:                                              std::println("{}", pCallbackData->pMessage); break;
            }
            return VK_FALSE;
        }
#endif

        VulkanInstance() = delete;
        ve_delete_copy_constructors(VulkanInstance);
        ve_delete_move_constructors(VulkanInstance);

        VulkanInstance(VkInstanceCreateInfo&& createInfo)
        {
#ifndef NDEBUG
            if (createInfo.enabledLayerCount > 0_u32) {
                std::println(ANSI_MESG "Checking instance layers..." ANSI_RSET);

                uint32_t availableInstanceLayerCount {};
                ve_vkcheck(vkEnumerateInstanceLayerProperties(
                    addressof(availableInstanceLayerCount),
                    {}
                ));
                VkLayerProperties availableInstanceLayers[availableInstanceLayerCount];
                ve_vkcheck(vkEnumerateInstanceLayerProperties(
                    addressof(availableInstanceLayerCount),
                    availableInstanceLayers
                ));

                for (uint32_t i {}; i < createInfo.enabledLayerCount; i++) {
                    const char* layerName { createInfo.ppEnabledLayerNames[i] };
                    bool foundLayer { false };
                    for (uint32_t j {}; j < availableInstanceLayerCount; j++) {
                        foundLayer |= (strcmp(availableInstanceLayers[j].layerName, layerName) == 0);
                    }

                    if (foundLayer) {
                        std::println("    Layer \"{}\": " ANSI_SCCS "supported" ANSI_RSET, layerName);
                    } else {
                        std::println("    Layer \"{}\": " ANSI_ERRR "unsupported" ANSI_RSET, layerName);
                    }
                }
            }

            if (createInfo.enabledExtensionCount > 0_u32) {
                std::println(ANSI_MESG "Checking instance extensions..." ANSI_RSET);

                uint32_t availableInstanceExtensionsCount {};
                ve_vkcheck(vkEnumerateInstanceExtensionProperties(
                    {},
                    addressof(availableInstanceExtensionsCount),
                    {}
                ));
                VkExtensionProperties availableInstanceExtensions[availableInstanceExtensionsCount];
                ve_vkcheck(vkEnumerateInstanceExtensionProperties(
                    {},
                    addressof(availableInstanceExtensionsCount),
                    availableInstanceExtensions
                ));

                for (uint32_t i {}; i < createInfo.enabledExtensionCount; i++) {
                    const char* extensionName { createInfo.ppEnabledExtensionNames[i] };
                    bool foundExtension { false };
                    for (uint32_t j {}; j < availableInstanceExtensionsCount; j++) {
                        foundExtension |= (strcmp(availableInstanceExtensions[j].extensionName, extensionName) == 0);
                    }

                    if (foundExtension) {
                        std::println("    Extension \"{}\": " ANSI_SCCS "supported" ANSI_RSET, extensionName);
                    } else {
                        std::println("    Extension \"{}\": " ANSI_ERRR "unsupported" ANSI_RSET, extensionName);
                    }
                }
            }
#endif

            ve_vkcheck(vkCreateInstance(
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));

#define ve_load_instance_function(name) name = (PFN_##name)vkGetInstanceProcAddr(this->handle, #name)

#ifndef NDEBUG
            ve_load_instance_function(vkCreateDebugUtilsMessengerEXT);
            ve_load_instance_function(vkDestroyDebugUtilsMessengerEXT);
#endif

#undef ve_load_instance_function

#ifndef NDEBUG
            constexpr VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo {
                VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                {},
                {},
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                debugMessengerCallback,
                {},
            };

            ve_vkcheck(this->vkCreateDebugUtilsMessengerEXT(this->handle, &debugMessengerCreateInfo, nullptr, &this->debugMessenger));
#endif
        }

        ~VulkanInstance()
        {
#ifndef NDEBUG
            this->vkDestroyDebugUtilsMessengerEXT(
                this->handle,
                this->debugMessenger,
                nullptr
            );
#endif
            vkDestroyInstance(
                this->handle,
                nullptr
            );
        }
    };

    constexpr VkInstanceCreateInfo makeInstanceCreateInfo(
        const void* pNext,
        VkInstanceCreateFlags flags,
        const VkApplicationInfo* pApplicationInfo,
        ArrayView<const char*> enabledLayerNames,
        ArrayView<const char*> enabledExtensionNames
    )
    {
        return {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            pNext,
            flags,
            pApplicationInfo,
            static_cast<uint32_t>(enabledLayerNames.length),
            enabledLayerNames.address,
            static_cast<uint32_t>(enabledExtensionNames.length),
            enabledExtensionNames.address,
        };
    }

    constexpr VkApplicationInfo makeApplicationInfo(
        const void* pNext,
        const char* pApplicationName,
        uint32_t applicationVersion,
        const char* pEngineName,
        uint32_t engineVersion,
        uint32_t apiVersion
    )
    {
        return {
            VK_STRUCTURE_TYPE_APPLICATION_INFO,
            pNext,
            pApplicationName,
            applicationVersion,
            pEngineName,
            engineVersion,
            apiVersion,
        };
    }

    extern VulkanInstance vulkanInstance;
}
