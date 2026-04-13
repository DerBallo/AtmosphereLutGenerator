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
    struct Sampler {
        VkSampler handle;

        Sampler() = delete;
        ve_delete_copy_constructors(Sampler);
        ve_delete_move_constructors(Sampler);

        Sampler(const VkSamplerCreateInfo& createInfo)
        {
            ve_vkcheck(vkCreateSampler(
                device.handle,
                addressof(createInfo),
                {},
                addressof(this->handle)
            ));
        }

        ~Sampler()
        {
            vkDestroySampler(
                device.handle,
                this->handle,
                {}
            );
        }
    };

    constexpr VkSamplerCreateInfo makeSamplerCreateInfo(
        const void* pNext,
        VkSamplerCreateFlags flags,
        VkFilter magFilter,
        VkFilter minFilter,
        VkSamplerMipmapMode mipmapMode,
        VkSamplerAddressMode addressModeU,
        VkSamplerAddressMode addressModeV,
        VkSamplerAddressMode addressModeW,
        float mipLodBias,
        VkBool32 anisotropyEnable,
        float maxAnisotropy,
        VkBool32 compareEnable,
        VkCompareOp compareOp,
        float minLod,
        float maxLod,
        VkBorderColor borderColor,
        VkBool32 unnormalizedCoordinates
    )
    {
        return {
            VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            pNext,
            flags,
            magFilter,
            minFilter,
            mipmapMode,
            addressModeU,
            addressModeV,
            addressModeW,
            mipLodBias,
            anisotropyEnable,
            maxAnisotropy,
            compareEnable,
            compareOp,
            minLod,
            maxLod,
            borderColor,
            unnormalizedCoordinates,
        };
    }
}
