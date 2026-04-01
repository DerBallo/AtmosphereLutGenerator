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
