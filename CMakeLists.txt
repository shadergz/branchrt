cmake_minimum_required(VERSION 3.22)
project(bra64rt)

add_library(bra64rt STATIC)

if (CMAKE_DEBUG_TYPE STREQUAL Debug)
    set(HOOK_RT_DEBUG True)
endif()

target_sources(bra64rt PRIVATE 
    hook_info_objects.cpp
    rw_addr.cpp
    )

target_include_directories(bra64rt
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>
)

