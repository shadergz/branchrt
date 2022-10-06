#pragma once

#include <cstdint>

namespace hookrt::info {

class Native_Info {

public:
    Native_Info() = default;
    Native_Info(const char* nativeName);
    /* Native library name, tipically a shared object loaded by JVM native interface
     * at runtime.
    */
    ~Native_Info() = default;
    auto getObjectName() const {
        return mNativeAddr;
    }
    auto getNativeAddr() const {
        return mNativeAddr;
    }
    Native_Info(Native_Info& nativeInfo) 
        : mNativeLibName(nativeInfo.mNativeLibName), 
        mNativeAddr(nativeInfo.mNativeAddr) {}

private:
    const char* mNativeLibName;
    uintptr_t mNativeAddr = 0;
};

}

