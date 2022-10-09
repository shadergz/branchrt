#pragma once

#include <cstdint>

namespace hookrt::info {

typedef int Hook_I32_t;
typedef uintptr_t Native_Addr_t;

typedef enum { HOOK_SUCCESS = 00, HOOK_INFO, HOOK_FAILED } Notify_Event_t;

struct Native_Info {

public:
    Native_Info() {}
    ~Native_Info() {}

    auto get_Object_Name() const {
        return m_Native_Lib_Name;
    }
    auto get_Native_Addr() const {
        return m_Native_Addr;
    }

    bool find_Base_Address(const char* nativeName);
    /* Native library name, tipically a shared object loaded by JVM native interface at runtime. */
    Native_Info(Native_Info& native_info) 
        : m_Native_Lib_Name(native_info.m_Native_Lib_Name), 
        m_Native_Addr(native_info.m_Native_Addr) {}
protected:
    virtual Hook_I32_t native_Notify(Notify_Event_t status, const char* message) {
        /* This function might be implemented by the derivate struct or class */
        int* __this_can_be_call = nullptr;
        *__this_can_be_call = 0;
        return *__this_can_be_call;
    }
    Hook_I32_t notify_Event_Sync(Notify_Event_t event_status, const char* format, ...);
private:
    const char* m_Native_Lib_Name = nullptr;
    Native_Addr_t m_Native_Addr = 0;
};

}

