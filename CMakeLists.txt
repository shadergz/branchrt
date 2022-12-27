cmake_minimum_required(VERSION 3.18)
project(hookRuntimeA64)

add_library(blrta64 STATIC)
target_sources(blrta64 PRIVATE hook_info_objects.cpp)

target_include_directories(blrta64
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>
)

