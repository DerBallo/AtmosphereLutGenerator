#pragma once

#ifndef ve_vulkan_core_h_included
    #define ve_vulkan_core_h_included
    #include <vulkan/vulkan_core.h>
#endif
#ifndef ve_std_concepts_included
    #define ve_std_concepts_included
    #include <concepts>
#endif
#ifndef ve_std_tuple_included
    #define ve_std_tuple_included
    #include <tuple>
#endif
#include "derballo.utils.macros.hpp"

namespace ve {
    template <typename T>
    concept VulkanStruct = std::is_standard_layout_v<T>
        && requires(T& t) { t.sType; t.pNext; }
        && std::same_as<std::remove_cvref_t<decltype(std::declval<T&>().sType)>, VkStructureType>
        && std::same_as<std::remove_cvref_t<decltype(std::declval<T&>().pNext)>, void*>
        && (offsetof(T, sType) == offsetof(VkBaseOutStructure, sType))
        && (offsetof(T, pNext) == offsetof(VkBaseOutStructure, pNext));

    template <VulkanStruct... Members>
    struct PnextChain {
        static_assert(sizeof...(Members) > 0, "VePNextChain requires at least one member.");

        static constexpr size_t size = sizeof...(Members);

    private:
        std::tuple<Members...> storage {};

        template <size_t... Indexes>
        constexpr void link(std::index_sequence<Indexes...>) noexcept
        {
            VkBaseOutStructure* nodes[] = { reinterpret_cast<VkBaseOutStructure*>(addressof(std::get<Indexes>(storage)))... };
            for (size_t i {}; (i + 1) < size; ++i) {
                nodes[i]->pNext = nodes[i + 1];
            }
            nodes[size - 1]->pNext = nullptr;
        }

    public:
        constexpr void* head() noexcept
        {
            return addressof(std::get<0>(storage));
        }

        constexpr operator void*() noexcept
        {
            return addressof(std::get<0>(storage));
        }

        PnextChain() = delete;
        ve_delete_copy_constructors(PnextChain);
        ve_delete_move_constructors(PnextChain);

        explicit constexpr PnextChain(Members... members) noexcept((std::is_nothrow_move_constructible_v<Members> && ...))
            : storage(std::move(members)...)
        {
            link(std::make_index_sequence<size> {});
        }

        template <typename... Args>
            requires(sizeof...(Args) == sizeof...(Members)) && (std::constructible_from<Members, Args &&> && ...)
        explicit constexpr PnextChain(Args&&... args) noexcept((std::is_nothrow_constructible_v<Members, Args&&> && ...))
            : storage(Members { std::forward<Args>(args) }...)
        {
            link(std::make_index_sequence<size> {});
        }
    };

    template <typename... Ts>
        requires(VulkanStruct<std::remove_cvref_t<Ts>> && ...)
    PnextChain(Ts&&...) -> PnextChain<std::remove_cvref_t<Ts>...>;
}
