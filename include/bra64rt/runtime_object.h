#pragma once

#include "object_native_info.h"

namespace RuntimeHook::RuntimeObject {

struct Native_Object : public ObjectInfo::Native_Info {
    public:
    Native_Object() {}
    ~Native_Object() {}
};
};

