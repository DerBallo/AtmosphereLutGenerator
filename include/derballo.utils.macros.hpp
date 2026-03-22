#pragma once

#define ve_stringify2(x) #x
#define ve_stringify(x) ve_stringify2(x)

#define ve_delete_copy_constructors(type) \
    type(type& other) = delete; \
    type& operator=(type& other) = delete; \
    type(const type& other) = delete; \
    type& operator=(const type& other) = delete;

#define ve_delete_move_constructors(type) \
    type(type&& other) = delete; \
    type& operator=(type&& other) = delete;
