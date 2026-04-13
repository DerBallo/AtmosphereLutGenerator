cmake_minimum_required(VERSION 3.20)

project(AtmosphereLutGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_FILES}
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2
    -Wnull-dereference -Wdouble-promotion -Wimplicit-fallthrough -Wno-vla
    -flto -fno-rtti -fno-exceptions
    #-fsanitize=address,undefined,leak,thread
    $<$<CONFIG:Debug>:-g3 -ggdb -O0 -fno-omit-frame-pointer>
    $<$<CONFIG:Release>:-O3 -march=native -mtune=native -ftree-vectorize -fomit-frame-pointer>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Release>:NDEBUG>
)

target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-rdynamic>
    $<$<CONFIG:Release>:-s -flto -fuse-linker-plugin>
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include/pch.hpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)
