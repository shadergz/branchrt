#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <algorithm>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <bra64rt/object_native_info.h>

/* Almost 64 bytes are sufficiently enough to store all full pathname for the native
 * library buffer, even with a bigger native name it's provide! */
constexpr std::uint16_t NATIVE_LIBNAME_FSZ = 64 * sizeof (char); 

namespace RuntimeHook::ObjectInfo {

Hook_I32_t Native_Info::a64_HookMessage(const Hook_Event_t event_status, const char* format, ...) {
    constexpr short HOOK_EVENTBF_SZ = 0x10f;
    va_list vars;
    va_start(vars, format);
    char local_msgbf[HOOK_EVENTBF_SZ];

    vsnprintf(local_msgbf, sizeof local_msgbf, format, vars);
    va_end(vars);

    return hookMessage_Release(event_status, local_msgbf);
}

bool Native_Info::find_Base_Address(const char* native_name) {
    if (native_name == nullptr) {
        a64_HookMessage(HOOK_EVENT_FAILED, "No library name has been passed\n");
        return false;
    }
    char native_library[NATIVE_LIBNAME_FSZ / 2];
    char LFS_libMap_filename[NATIVE_LIBNAME_FSZ];

    memcpy(native_library, native_name, sizeof(native_library));
    /* On Linux File System hierarchy there's a file into proc high-level communication pseudo-file thats 
     * hold mapped memory sections information.
    */
    #define USE_SELF_PROC 1
    #if USE_SELF_PROC
    strncpy(LFS_libMap_filename, "/proc/self/maps", sizeof(LFS_libMap_filename));
    #else
    snprintf(LFS_libMap_filename, sizeof(LFS_libMap_filename), "/proc/%d/maps", getpid());
    #endif

    a64_HookMessage(HOOK_EVENT_INFO, "Opening proc map file in %s to search for %s\n", LFS_libMap_filename, native_library);
    FILE* map_file = fopen(LFS_libMap_filename, "r");
    if (map_file == nullptr) {
        a64_HookMessage(HOOK_EVENT_FAILED, "Can't open proc map file because %s\n", strerror(errno));
        return false;
    }
    constexpr short MAP_FILE_DATA_SZ = 0x2ff;
    char map_content[MAP_FILE_DATA_SZ];

    #define MAP_ENTRIES_FOREACH(output, file)\
        for (; fgets(output, sizeof(output), file) != nullptr;)

    MAP_ENTRIES_FOREACH(map_content, map_file) {
        /* What we're interesting to search for e.g: 
         * 7fb2cf9c2000-7fb2cf9ea000 r--p 00000000 00:19 31795                      /usr/lib/x86_64-linux-gnu/libc.so.6
         * 7fb2cf9c2000 is the base address pointer that we're trying to locate.
        */
        a64_HookMessage(HOOK_EVENT_INFO, "Looking at: %s", map_content);
        if (strstr(map_content, native_library)) {
            m_Native_Addr = (Native_Addr_t)strtoul(map_content, 0, 0x10);
            a64_HookMessage(HOOK_EVENT_SUCCESS, "%s base address found in %#llx\n", 
                native_library, m_Native_Addr);
            break;
        }
    }

    fclose(map_file);
    return m_Native_Addr != 0;
}

}
