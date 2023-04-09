cmake_minimum_required(VERSION 3.18)
project(bra64rt)

add_library(bra64rt STATIC)
target_sources(bra64rt PRIVATE 
    hook_info_objects.cpp
    rw_addr.cpp
    )

target_include_directories(bra64rt
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>
)

