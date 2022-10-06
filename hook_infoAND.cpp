#include <sys/types.h>
#include <unistd.h>

#include <hookrt/info.h>

/* 
 * Almost 64 bytes are sufficiently enough for store all full pathname for the native
 * libary buffer, even with a bigger native name it's provide!
*/
constexpr std::uint16_t NATIVE_LIB_FSZ = 64 * sizeof(char); 

namespace hookrt::info {

Native_Info::Native_Info(const char* nativeName) {
    /* char nativeLibraryFile[NATIVE_LIB_FSZ]{}; */
    mNativeAddr = 0;
}

}
