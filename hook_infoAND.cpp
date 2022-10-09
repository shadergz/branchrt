#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <algorithm>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <hookrt/info.h>

/* Almost 64 bytes are sufficiently enough for store all full pathname for the native
 * libary buffer, even with a bigger native name it's provide!
*/
constexpr std::uint16_t NATIVE_LIB_FSZ = 64 * sizeof(char); 

namespace hookrt::info {

Hook_I32_t Native_Info::notify_Event_Sync(Notify_Event_t event_status, const char* format, ...)
{
    constexpr short NOTIFY_EVENT_SZ = 0x10f;
    va_list vars;
    va_start(vars, format);
    char notify_local[NOTIFY_EVENT_SZ];
    vsnprintf(notify_local, sizeof(notify_local), format, vars);
    va_end(vars);

    return native_Notify(event_status, notify_local);
}

bool Native_Info::find_Base_Address(const char* native_name) {
    if (native_name == nullptr) {
        notify_Event_Sync(HOOK_FAILED, "No library name has been passed");
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

    notify_Event_Sync(HOOK_INFO, "Opening proc map file in %s to search for %s\n", LFS_libMap_filename, native_library);
    FILE* map_file = fopen(LFS_libMap_filename, "r");
    if (map_file == nullptr) {
        notify_Event_Sync(HOOK_FAILED, "Can't open proc map file because %s\n", strerror(errno));
        return false;
    }
    constexpr short MAP_FILE_DATA_SZ = 0x2ff;
    char map_content[MAP_FILE_DATA_SZ];

    #define FOR_EACH_READ(output, file)\
        for (; fgets(output, sizeof(output), file) != nullptr;)
    #define STOP break

    FOR_EACH_READ(map_content, map_file) {
        /* What we're interesting to search for e.g: 
         * 7fb2cf9c2000-7fb2cf9ea000 r--p 00000000 00:19 31795                      /usr/lib/x86_64-linux-gnu/libc.so.6
         * 7fb2cf9c2000 is the base address pointer that we're trying to locate.
        */
        notify_Event_Sync(HOOK_INFO, "Looking at: %s\n", map_content);
        if (strstr(map_content, native_library)) {
            m_Native_Addr = (Native_Addr_t)strtoul(map_content, 0, 0x10);
            notify_Event_Sync(HOOK_SUCCESS, "%s base address found in %#llx\n", 
                native_library, m_Native_Addr);
            STOP;
        }
    }

    fclose(map_file);
    return m_Native_Addr != 0;
}

}
