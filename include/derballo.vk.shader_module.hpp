#pragma once

#include "derballo.vk.device.hpp"

namespace ve {
    struct ShaderModule {
        VkShaderModule handle;

        ShaderModule() = delete;
        ve_delete_copy_constructors(ShaderModule);
        ve_delete_move_constructors(ShaderModule);

        ShaderModule(const VkShaderModuleCreateInfo& createInfo)
        {
            if (createInfo.pCode) {
                ve_vkcheck(vkCreateShaderModule(
                    device.handle,
                    addressof(createInfo),
                    {},
                    addressof(this->handle)
                ));
            } else {
                this->handle = nullptr;
            }
        }

        ~ShaderModule()
        {
            vkDestroyShaderModule(
                device.handle,
                this->handle,
                {}
            );
        }
    };

    constexpr VkShaderModuleCreateInfo makeShaderModuleCreateInfo(
        const void* pNext,
        VkShaderModuleCreateFlags flags,
        ArrayView<char> code
    )
    {
        return {
            VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            pNext,
            flags,
            code.length,
            reinterpret_cast<const uint32_t*>(code.address)
        };
    }
}
