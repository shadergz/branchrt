cmake_minimum_required(VERSION 3.18)

project(hookrta64v8a)

add_library(hookrta64 STATIC hook_infoAND.cpp)

target_include_directories(hookrta64
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

