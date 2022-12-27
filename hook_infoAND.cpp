#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <algorithm>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <hookRuntime/ObjectNativeInfo.h>

/* Almost 64 bytes are sufficiently enough for store all full pathname for the native
 * library buffer, even with a bigger native name it's provide! */
constexpr std::uint16_t NATIVE_LIB_FSZ = 64 * sizeof(char); 

namespace hookRuntime::ObjectInfo {

Hook_I32_t Native_Info::event_Sync(Hook_Event_t event_status, const char* format, ...) {
    constexpr short HOOK_EVENT_SZ = 0x10f;
    va_list vars;
    va_start(vars, format);
    char event_local_message[HOOK_EVENT_SZ];
    vsnprintf(event_local_message, sizeof(event_local_message), format, vars);
    va_end(vars);

    return event_Release(event_status, event_local_message);
}

bool Native_Info::find_Base_Address(const char* native_name) {
    if (native_name == nullptr) {
        event_Sync(HOOK_FAILED, "No library name has been passed");
        return false;
    }
    char native_library[NATIVE_LIB_FSZ / 2];
    char LFS_libMap_filename[NATIVE_LIB_FSZ];

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

    event_Sync(HOOK_INFO, "Opening proc map file in %s to search for %s\n", LFS_libMap_filename, native_library);
    FILE* map_file = fopen(LFS_libMap_filename, "r");
    if (map_file == nullptr) {
        event_Sync(HOOK_FAILED, "Can't open proc map file because %s\n", strerror(errno));
        return false;
    }
    constexpr short MAP_FILE_DATA_SZ = 0x2ff;
    char map_content[MAP_FILE_DATA_SZ];

    #define MAP_ENTRIES_FOREACH(output, file)\
        for (; fgets(output, sizeof(output), file) != nullptr;)
    #define STOP break

    MAP_ENTRIES_FOREACH(map_content, map_file) {
        /* What we're interesting to search for e.g: 
         * 7fb2cf9c2000-7fb2cf9ea000 r--p 00000000 00:19 31795                      /usr/lib/x86_64-linux-gnu/libc.so.6
         * 7fb2cf9c2000 is the base address pointer that we're trying to locate.
        */
        event_Sync(HOOK_INFO, "Looking at: %s\n", map_content);
        if (strstr(map_content, native_library)) {
            m_Native_Addr = (Native_Addr_t)strtoul(map_content, 0, 0x10);
            event_Sync(HOOK_SUCCESS, "%s base address found in %#llx\n", 
                native_library, m_Native_Addr);
            STOP;
        }
    }

    fclose(map_file);
    return m_Native_Addr != 0;
}

}
