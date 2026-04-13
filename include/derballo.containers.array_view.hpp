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

#include "derballo.utils.hpp"

namespace ve {
    template <typename T>
    struct ArrayView {
        uint32_t length;
        const T* address;

        ve_delete_move_constructors(ArrayView);

        constexpr ArrayView()
            : length { 0_u32 }, address { nullptr }
        {
        }

        template <uint32_t N>
        constexpr ArrayView(const T (&array)[N])
            : length { N }, address { array }
        {
        }

        constexpr ArrayView(std::initializer_list<T> initializerList)
            : length { static_cast<uint32_t>(initializerList.size()) }, address { initializerList.begin() }
        {
        }

        constexpr ArrayView(uint32_t length_, const T* address_)
            : length { length_ }, address { address_ }
        {
        }

        ArrayView(ArrayView& other)
            : length { other.length }, address { other.address }
        {
        }

        ArrayView& operator=(ArrayView& other)
        {
            this->length = other.length;
            this->address = other.address;
        }

        ArrayView(const ArrayView& other)
            : length { other.length }, address { other.address }
        {
        }

        ArrayView& operator=(const ArrayView& other)
        {
            this->length = other.length;
            this->address = other.address;
        }
    };

    template <typename T, uint32_t N>
    ArrayView(const T (&)[N]) -> ArrayView<T>;

    template <typename T1, typename T2>
    struct DualArrayView {
        uint32_t length;
        const T1* address1;
        const T2* address2;

        ve_delete_move_constructors(DualArrayView);

        constexpr DualArrayView()
            : length { 0_u32 }, address1 { nullptr }, address2 { nullptr }
        {
        }

        template <uint32_t N>
        constexpr DualArrayView(const T1 (&array1)[N], const T2 (&array2)[N])
            : length { N }, address1 { array1 }, address2 { array2 }
        {
        }

        constexpr DualArrayView(std::initializer_list<T1> initializerList1, std::initializer_list<T2> initializerList2)
            : length { static_cast<uint32_t>(initializerList1.size()) }, address1 { initializerList1.begin() }, address2 { initializerList2.begin() }
        {
            ve_debugassert(initializerList1.size() == initializerList2.size());
        }

        constexpr DualArrayView(const T1* address, std::initializer_list<T2> initializerList)
            : length { static_cast<uint32_t>(initializerList.size()) }, address1 { address }, address2 { initializerList.begin() }
        {
        }

        constexpr DualArrayView(std::initializer_list<T2> initializerList, const T1* address)
            : length { static_cast<uint32_t>(initializerList.size()) }, address1 { initializerList.begin() }, address2 { address }
        {
        }

        constexpr DualArrayView(uint32_t length_, const T1* address1_, const T2* address2_)
            : length { length_ }, address1 { address1_ }, address2 { address2_ }
        {
        }

        DualArrayView(DualArrayView& other)
            : length { other.length }, address1 { other.address1 }, address2 { other.address2 }
        {
        }

        DualArrayView& operator=(DualArrayView& other)
        {
            this->length = other.length;
            this->address1 = other.address1;
            this->address2 = other.address2;
        }

        DualArrayView(const DualArrayView& other)
            : length { other.length }, address1 { other.address1 }, address2 { other.address2 }
        {
        }

        DualArrayView& operator=(const DualArrayView& other)
        {
            this->length = other.length;
            this->address1 = other.address1;
            this->address2 = other.address2;
        }
    };

    template <typename T1, typename T2, uint32_t N>
    DualArrayView(const T1 (&)[N], const T2 (&)[N]) -> DualArrayView<T1, T2>;
}
