#pragma once

#include <hookrt/info.h>
/*
constexpr const char* LIB_PREFIX = "lib";
constexpr const char* LIB_SUFFIX = ".so";
*/

namespace hookrt::object {

struct Native_Object : public hookrt::info::Native_Info {
    public:
    Native_Object() {}
    ~Native_Object() {}
};
};

