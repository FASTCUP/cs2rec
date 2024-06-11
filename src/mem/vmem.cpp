#include "vmem.hpp"

#if defined(_WIN32)
    #include <Windows.h>
#elif defined(__linux__)
    #include <sys/mman.h>
#endif

namespace Vmem {

#if defined(_WIN32)

    // TODO: Implement :)

#elif defined(__linux__)

    
    ProtectFlags FromNativeFlags(NativeProtectFlags native_flags) {
        ProtectFlags flags = 0;
        if (native_flags & PROT_READ)  flags |= PROTECTFLAG_READ;
        if (native_flags & PROT_WRITE) flags |= PROTECTFLAG_WRITE;
        if (native_flags & PROT_EXEC)  flags |= PROTECTFLAG_EXEC;
        return flags;
    }
    NativeProtectFlags ToNativeFlags(ProtectFlags flags) {
        NativeProtectFlags native_flags = 0;
        if (flags & PROTECTFLAG_READ)  native_flags |= PROT_READ;
        if (flags & PROTECTFLAG_WRITE) native_flags |= PROT_WRITE;
        if (flags & PROTECTFLAG_EXEC)  native_flags |= PROT_EXEC;
        return native_flags;
    }

    void* Alloc(size_t len, NativeProtectFlags flags) {
        return mmap(nullptr, len, flags, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    }
    void Free(void* mem, size_t len) {
        munmap(mem, len);
    }

    NativeProtectFlags GetProtection(void* mem) {
        // FIXME: Implement this correctly.
        // For now this will return all the main permissions.
        return PROT_READ | PROT_WRITE | PROT_EXEC;
    }

    void SetProtection(void* mem, size_t len, NativeProtectFlags flags, NativeProtectFlags* old_flags) {
        if (old_flags != nullptr)
            *old_flags = GetProtection(mem);
        mprotect(mem, len, flags);
    }

#endif
}