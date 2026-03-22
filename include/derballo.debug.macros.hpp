#pragma once

#include "derballo.format.vkresult.hpp"
#include "derballo.globals.hpp"
#include "derballo.utils.macros.hpp"
#ifndef ve_vulkan_core_h_included
    #define ve_vulkan_core_h_included
    #include <vulkan/vulkan_core.h>
#endif
#ifndef ve_std_print_included
    #define ve_std_print_included
    #include <print>
#endif

#ifdef NDEBUG

    #define ve_fail(message) \
        { \
            std::abort(); \
        }

    #define ve_assert(expression) \
        { \
            bool _result_ = expression; \
            if (_result_ != true) { \
                std::abort(); \
            } \
        }

    #define ve_debugassert(expression) \
        { \
        }

    #define ve_assertcatch(expression, caseTrue, caseFalse) \
        { \
            bool _result_ = expression; \
            if (_result_ == true) { \
                caseTrue \
            } else { \
                caseFalse \
            } \
        }

    #define ve_vkcheck(expression) \
        { \
            VkResult _result_ = expression; \
            if (_result_ != VK_SUCCESS) { \
                std::abort(); \
            } \
        }

    #define ve_vkcheck_switch(expression, ...) \
        { \
            VkResult _result_ = expression; \
            switch (_result_) { \
                __VA_ARGS__ \
                default: { \
                    std::abort(); \
                } break; \
            } \
        }

#else

    #define ve_fail(message) \
        { \
            std::println(ANSI_ERRR message "\nIn line " ve_stringify(__LINE__) " in file " __FILE__ ANSI_RSET); \
            std::abort(); \
        }

    #define ve_assert(expression) \
        { \
            bool _result_ = static_cast<bool>(expression); \
            if (_result_ != true) { \
                std::println(ANSI_ERRR "Assert failed in line " ve_stringify(__LINE__) " in file " __FILE__ ":\n" #expression ANSI_RSET); \
                std::abort(); \
            } \
        }

    #define ve_debugassert(expression) \
        { \
            bool _result_ = static_cast<bool>(expression); \
            if (_result_ != true) { \
                std::println(ANSI_ERRR "Debug assert failed in line " ve_stringify(__LINE__) " in file " __FILE__ ":\n" #expression ANSI_RSET); \
            } \
        }

    #define ve_assertcatch(expression, caseTrue, caseFalse) \
        { \
            bool _result_ = static_cast<bool>(expression); \
            if (_result_ == true) { \
                caseTrue \
            } else { \
                caseFalse \
            } \
        }

    #define ve_vkcheck(expression) \
        { \
            VkResult _result_ = expression; \
            if (_result_ != VK_SUCCESS) { \
                std::println(ANSI_ERRR "Unexpected VkResult \"{}\"\nIn line " ve_stringify(__LINE__) " in file " __FILE__ ANSI_RSET, _result_); \
                std::abort(); \
            } \
        }

    #define ve_vkcheck_switch(expression, ...) \
        { \
            VkResult _result_ = expression; \
            switch (_result_) { \
                __VA_ARGS__ \
                default: { \
                    std::println(ANSI_ERRR "Unexpected VkResult \"{}\"\nIn line " ve_stringify(__LINE__) " in file " __FILE__ ANSI_RSET, _result_); \
                    std::abort(); \
                } break; \
            } \
        }

#endif

#define ve_check_case(value, expression) \
    case value: expression break
