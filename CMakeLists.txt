cmake_minimum_required(VERSION 3.22)
project(branchrt)

add_library(branchrt STATIC)

if (CMAKE_DEBUG_TYPE STREQUAL Debug)
    set(HOOK_RT_DEBUG True)
endif()

target_sources(branchrt PRIVATE 
    hook_info_objects.cpp
    rw_addr.cpp
)

target_include_directories(branchrt
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDE_DIR}>
)

