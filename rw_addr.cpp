#include <cstdint>

#include <branchrt/object_native_info.h>

namespace RuntimeHook::ObjectInfo {

// This function makes a raw pointer (without RAII information) inside the already hooked library
Native_Addr_t Native_Info::make_Pointer(const uint_fast64_t accAddr) {
    a64_HookMessage(HOOK_EVENT_INFO, "Making a pointer in %p\n", accAddr);

    Native_Addr_t rw_Ptr = m_Native_Addr + accAddr;
    if (__builtin_expect(rw_Ptr == accAddr, 0)) {
        if (!m_Native_Addr) {
            a64_HookMessage(HOOK_EVENT_FAILED, "The base address from the library can't be #0...*8\n");
            return static_cast<uint_fast64_t>(0);
        }
    }

    if (rw_Ptr < m_Native_Addr) {
        a64_HookMessage(HOOK_EVENT_INFO, "Address has overflowed to %p\n", rw_Ptr);
        return rw_Ptr;
    }

    constexpr uint8_t PREFETCH_WRITE = 1;
    constexpr uint8_t PREFETCH_MODERATE = 3;
    __builtin_prefetch(reinterpret_cast<void*>(rw_Ptr), 
        PREFETCH_WRITE, PREFETCH_MODERATE);

    return rw_Ptr;
}

};
