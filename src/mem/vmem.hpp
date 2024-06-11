#pragma once
#include <cstdint>
#include <cstddef>

/**
 * APIs for managing virtual memory
 */
namespace Vmem {
    /// A combination of values from @ref _ProtectFlags
    using ProtectFlags = uint8_t;
    using NativeProtectFlags = uintptr_t;

    enum _ProtectFlags {
        PROTECTFLAG_READ  = 1<<0,
        PROTECTFLAG_WRITE = 1<<1,
        PROTECTFLAG_EXEC  = 1<<2
    };

    ProtectFlags FromNativeFlags(NativeProtectFlags native_flags);
    NativeProtectFlags ToNativeFlags(ProtectFlags flags);

    void* Alloc(size_t len, NativeProtectFlags flags);
    void Free(void* mem, size_t len);
    NativeProtectFlags GetProtection(void* mem);
    /**
     * @param old_flags (optional) Place to save the previous protection flags
     */
    void SetProtection(void* mem, size_t len, NativeProtectFlags flags, NativeProtectFlags* old_flags);
}