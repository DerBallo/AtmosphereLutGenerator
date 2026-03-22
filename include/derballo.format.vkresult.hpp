#pragma once

#ifndef ve_vulkan_core_h_included
    #define ve_vulkan_core_h_included
    #include <vulkan/vulkan_core.h>
#endif
#ifndef ve_std_format_included
    #define ve_std_format_included
    #include <format>
#endif

namespace std {
    template <>
    struct formatter<VkResult> {
        bool as_number;

        template <class Out>
        static constexpr Out appendString(Out out, const char* string) noexcept
        {
            while (*string) {
                *out++ = *string++;
            }
            return out;
        }

        template <typename FormatParseContext>
        constexpr auto parse(FormatParseContext& formatParseContext)
        {
            auto iterator = formatParseContext.begin();
            if (iterator == formatParseContext.end()) {
                as_number = false;
                return iterator;
            } else if (*iterator == '#') {
                as_number = true;
                ++iterator;
                return iterator;
            } else {
                as_number = false;
                return iterator;
            }
        }

        template <typename FormatContext>
        auto format(VkResult result, FormatContext& formatContext) const
        {
            if (as_number) {
                return std::format_to(formatContext.out(), "{:d}", static_cast<int32_t>(result));
            } else {
#define EXPAND_CASE(resultCase) \
    case resultCase: return appendString(formatContext.out(), #resultCase);

                switch (result) {
                    EXPAND_CASE(VK_SUCCESS)
                    EXPAND_CASE(VK_NOT_READY)
                    EXPAND_CASE(VK_TIMEOUT)
                    EXPAND_CASE(VK_EVENT_SET)
                    EXPAND_CASE(VK_EVENT_RESET)
                    EXPAND_CASE(VK_INCOMPLETE)
                    EXPAND_CASE(VK_ERROR_OUT_OF_HOST_MEMORY)
                    EXPAND_CASE(VK_ERROR_OUT_OF_DEVICE_MEMORY)
                    EXPAND_CASE(VK_ERROR_INITIALIZATION_FAILED)
                    EXPAND_CASE(VK_ERROR_DEVICE_LOST)
                    EXPAND_CASE(VK_ERROR_MEMORY_MAP_FAILED)
                    EXPAND_CASE(VK_ERROR_LAYER_NOT_PRESENT)
                    EXPAND_CASE(VK_ERROR_EXTENSION_NOT_PRESENT)
                    EXPAND_CASE(VK_ERROR_FEATURE_NOT_PRESENT)
                    EXPAND_CASE(VK_ERROR_INCOMPATIBLE_DRIVER)
                    EXPAND_CASE(VK_ERROR_TOO_MANY_OBJECTS)
                    EXPAND_CASE(VK_ERROR_FORMAT_NOT_SUPPORTED)
                    EXPAND_CASE(VK_ERROR_FRAGMENTED_POOL)
                    EXPAND_CASE(VK_ERROR_UNKNOWN)
                    EXPAND_CASE(VK_ERROR_OUT_OF_POOL_MEMORY)
                    EXPAND_CASE(VK_ERROR_INVALID_EXTERNAL_HANDLE)
                    EXPAND_CASE(VK_ERROR_FRAGMENTATION)
                    EXPAND_CASE(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS)
                    EXPAND_CASE(VK_PIPELINE_COMPILE_REQUIRED)
                    EXPAND_CASE(VK_ERROR_NOT_PERMITTED)
                    EXPAND_CASE(VK_ERROR_SURFACE_LOST_KHR)
                    EXPAND_CASE(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)
                    EXPAND_CASE(VK_SUBOPTIMAL_KHR)
                    EXPAND_CASE(VK_ERROR_OUT_OF_DATE_KHR)
                    EXPAND_CASE(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR)
                    EXPAND_CASE(VK_ERROR_INVALID_SHADER_NV)
                    EXPAND_CASE(VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR)
                    EXPAND_CASE(VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR)
                    EXPAND_CASE(VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR)
                    EXPAND_CASE(VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR)
                    EXPAND_CASE(VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR)
                    EXPAND_CASE(VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR)
                    EXPAND_CASE(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT)
                    EXPAND_CASE(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT)
                    EXPAND_CASE(VK_THREAD_IDLE_KHR)
                    EXPAND_CASE(VK_THREAD_DONE_KHR)
                    EXPAND_CASE(VK_OPERATION_DEFERRED_KHR)
                    EXPAND_CASE(VK_OPERATION_NOT_DEFERRED_KHR)
                    EXPAND_CASE(VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR)
                    EXPAND_CASE(VK_ERROR_COMPRESSION_EXHAUSTED_EXT)
                    EXPAND_CASE(VK_INCOMPATIBLE_SHADER_BINARY_EXT)
                    EXPAND_CASE(VK_PIPELINE_BINARY_MISSING_KHR)
                    EXPAND_CASE(VK_ERROR_NOT_ENOUGH_SPACE_KHR)
                    default:
                        return std::format_to(formatContext.out(), "Unknow VkResult \"{:d}\"", static_cast<int32_t>(result));
                }

#undef EXPAND_CASE
            }
        }
    };
};
