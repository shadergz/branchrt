#pragma once

#include <cstdint>

namespace RuntimeHook::ObjectInfo {

typedef int Hook_I32_t;
typedef uint_fast64_t Native_Addr_t;

typedef enum { 
    HOOK_EVENT_SUCCESS, 
    HOOK_EVENT_INFO, 
    HOOK_EVENT_FAILED 
} Hook_Event_t;

class Native_Info {

public:
    Native_Info() = default;
    ~Native_Info() = default;

    auto get_Object_Name() const {
        return m_Native_Lib_Name;
    }
    auto get_Native_Addr() const {
        return m_Native_Addr;
    }

    Native_Addr_t make_Pointer(const uint_fast64_t libAddr);

    template<typename T>
    auto inline make_Object(const uint_fast64_t localAddr) {
        return reinterpret_cast<T>(make_Pointer(localAddr));
    }

    template<typename T>
    auto inline make_Method(const uint_fast64_t methodAddress) {
        auto funcAddr = make_Object<uintptr_t>(methodAddress);
        return reinterpret_cast<T*>(funcAddr);
    }

    template<typename T, typename... Args>
    auto invoke_Method(const uint_fast64_t methodAddress, Args... method_Args) {
       *(make_Method<T>(methodAddress))(method_Args...);
    }

    bool find_Base_Address(const char* nativeName);
    /* Native library name, tipically a shared object loaded by JVM native interface at runtime. */
    Native_Info(Native_Info& native_info) 
        : m_Native_Lib_Name(native_info.m_Native_Lib_Name), 
        m_Native_Addr(native_info.m_Native_Addr) {}

protected:

    virtual Hook_I32_t hookMessage_Release(Hook_Event_t status, const char* message) {
        /* This function should be implemented by the derivate struct or class */
        int* __cant_be_called = nullptr;
        *__cant_be_called = 0;
        return *__cant_be_called;
    }

    Hook_I32_t a64_HookMessage(const Hook_Event_t event_status, const char* format, ...);

private:
    const char* m_Native_Lib_Name = nullptr;
    Native_Addr_t m_Native_Addr = 0;
};

}

