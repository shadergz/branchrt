#pragma once

#include "object_native_info.h"

/*
    constexpr const char* LIB_PREFIX = "lib";
    constexpr const char* LIB_SUFFIX = ".so";
*/

namespace RuntimeHook::LogicalObject {

struct Native_Object : public ObjectInfo::Native_Info {
    public:
    Native_Object() {}
    ~Native_Object() {}
};
};

