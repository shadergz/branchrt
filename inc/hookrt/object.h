#pragma once

#include <memory>

#include <hookrt/info.h>

/*
constexpr const char* LIB_PREFIX = "lib";
constexpr const char* LIB_SUFFIX = ".so";
*/

namespace hookrt::object {

    class Native_Object {
    protected:
        Native_Object() = default;

        Native_Object(const char* objNativeName) {
            /* TODO: If objNativeName haven't a library prefix then add now,
             * e.g: objiativeName = "module"; and after this operations below: libmodule.so */
            nativeObjInfo = std::make_unique<info::Native_Info>(objNativeName);
        }

        Native_Object(info::Native_Info& info) {
            nativeObjInfo = std::make_unique<info::Native_Info>(info);
        }
        auto getLibraryName() {
            return nativeObjInfo->getObjectName();
        }

        std::unique_ptr<info::Native_Info> nativeObjInfo;
    };
};

