#pragma once

#include "derballo.universal.hpp"
#include "derballo.utils.macros.hpp"

namespace ve {
    template <typename T>
    struct ArrayView {
        size_t length;
        const T* address;

        ve_delete_copy_constructors(ArrayView);
        ve_delete_move_constructors(ArrayView);

        constexpr ArrayView()
            : length { 0_size }, address { nullptr }
        {
        }

        template <size_t N>
        constexpr ArrayView(const T (&array)[N])
            : length { N }, address { array }
        {
        }

        constexpr ArrayView(std::initializer_list<T> initializerList)
            : length { initializerList.size() }, address { initializerList.begin() }
        {
        }

        constexpr ArrayView(size_t length, const T* address)
            : length { length }, address { address }
        {
        }
    };

    template <typename T, size_t N>
    ArrayView(const T (&)[N]) -> ArrayView<T>;
}
