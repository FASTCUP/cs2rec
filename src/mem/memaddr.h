#ifndef DYNLIBUTILS_MEMADDR_H
#define DYNLIBUTILS_MEMADDR_H
#ifdef _WIN32
#pragma once
#endif

#include <cstdint>
#include <stddef.h>

namespace DynLibUtils {

class CMemory
{
public:
	CMemory() : ptr(0) {}
	CMemory(const uintptr_t ptr) : ptr(ptr) {}
	CMemory(const void* ptr) : ptr(uintptr_t(ptr)) {}

	inline operator uintptr_t() const noexcept
	{
		return ptr;
	}

	inline operator void*() const noexcept
	{
		return reinterpret_cast<void*>(ptr);
	}

	explicit inline operator bool() const noexcept
	{
		return ptr != 0;
	}

	inline bool operator!= (const CMemory& addr) const noexcept
	{
		return ptr != addr.ptr;
	}

	inline bool operator== (const CMemory& addr) const noexcept
	{
		return ptr == addr.ptr;
	}

	inline bool operator== (const uintptr_t& addr) const noexcept
	{
		return ptr == addr;
	}

	[[nodiscard]] inline uintptr_t GetPtr() const noexcept
	{
		return ptr;
	}

	template<class T> [[nodiscard]] inline T GetValue() const noexcept
	{
		return *reinterpret_cast<T*>(ptr);
	}

	template<typename T> [[nodiscard]] inline T CCast() const noexcept
	{
		return (T)ptr;
	}

	template<typename T> [[nodiscard]] inline T RCast() const noexcept
	{
		return reinterpret_cast<T>(ptr);
	}

	template<typename T> [[nodiscard]] inline T UCast() const noexcept
	{
		union { uintptr_t ptr; T cptr; } cast;
		return cast.ptr = ptr, cast.cptr;
	}

	[[nodiscard]] inline CMemory Offset(ptrdiff_t offset) const noexcept
	{
		return ptr + offset;
	}

	inline CMemory OffsetSelf(ptrdiff_t offset) noexcept
	{
		ptr += offset;
		return *this;
	}

	[[nodiscard]] inline CMemory Deref(int deref = 1) const
	{
		uintptr_t reference = ptr;

		while (deref--)
		{
			if (reference)
				reference = *reinterpret_cast<uintptr_t*>(reference);
		}

		return reference;
	}

	inline CMemory DerefSelf(int deref = 1)
	{
		while (deref--)
		{
			if (ptr)
				ptr = *reinterpret_cast<uintptr_t*>(ptr);
		}

		return *this;
	}

	[[nodiscard]] inline CMemory FollowNearCall(const ptrdiff_t opcodeOffset = 0x1, const ptrdiff_t nextInstructionOffset = 0x5) const
	{
		return ResolveRelativeAddress(opcodeOffset, nextInstructionOffset);
	}

	inline CMemory FollowNearCallSelf(const ptrdiff_t opcodeOffset = 0x1, const ptrdiff_t nextInstructionOffset = 0x5)
	{
		return ResolveRelativeAddressSelf(opcodeOffset, nextInstructionOffset);
	}

	[[nodiscard]] inline CMemory ResolveRelativeAddress(const ptrdiff_t registerOffset = 0x0, const ptrdiff_t nextInstructionOffset = 0x4) const
	{
		const uintptr_t skipRegister = ptr + registerOffset;
		const int32_t relativeAddress = *reinterpret_cast<int32_t*>(skipRegister);
		const uintptr_t nextInstruction = ptr + nextInstructionOffset;
		return nextInstruction + relativeAddress;
	}

	inline CMemory ResolveRelativeAddressSelf(const ptrdiff_t registerOffset = 0x0, const ptrdiff_t nextInstructionOffset = 0x4)
	{
		const uintptr_t skipRegister = ptr + registerOffset;
		const int32_t relativeAddress = *reinterpret_cast<int32_t*>(skipRegister);
		const uintptr_t nextInstruction = ptr + nextInstructionOffset;
		ptr = nextInstruction + relativeAddress;

		return *this;
	}

private:
	uintptr_t ptr;
};

} // namespace DynLibUtils

#endif // DYNLIBUTILS_MEMADDR_H
