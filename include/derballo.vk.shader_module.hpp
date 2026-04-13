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
