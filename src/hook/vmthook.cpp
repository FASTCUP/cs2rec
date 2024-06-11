#include "vmthook.hpp"
#include <cstring> // std::memcpy
#include <mem/vmem.hpp>

void VmtHook::Hook(void* Instance, bool AllInstances)
{
	m_inst = Instance;
	m_all = AllInstances;
	void** vmt = *(void***)m_inst;
	for (m_count = 0; vmt[m_count]; m_count++);

	void** copy = new void* [m_count];
	std::memcpy(copy, vmt, sizeof(vmt[0]) * m_count);

	if (m_all)
	{
		m_oldvmt = copy;
		m_newvmt = vmt;
	}
	else
	{
		m_oldvmt = vmt;
		m_newvmt = copy;
		*(void***)m_inst = m_newvmt;
	}
}

void VmtHook::Unhook()
{
	if (!IsHooked())
		return;

	size_t size = sizeof(m_oldvmt[0]) * m_count;

	if (m_all)
	{
		Vmem::NativeProtectFlags old_flags;
		Vmem::NativeProtectFlags new_flags = Vmem::ToNativeFlags(
			Vmem::PROTECTFLAG_READ | Vmem::PROTECTFLAG_WRITE | Vmem::PROTECTFLAG_EXEC
		);
        Vmem::SetProtection(m_newvmt, size, new_flags, &old_flags);
		std::memcpy(m_newvmt, m_oldvmt, size);
        Vmem::SetProtection(m_newvmt, size, old_flags, nullptr);
		delete[] m_oldvmt;
	}
	else
	{
		*(void***)m_inst = m_oldvmt;
		delete[] m_newvmt;
	}

	m_inst = 0;
	m_oldvmt = m_newvmt = 0;
}

void VmtHook::Set(size_t Index, void* Function)
{
	if (m_all)
	{
		size_t size = sizeof(m_newvmt[Index]);
		
		Vmem::NativeProtectFlags old_flags;
		Vmem::NativeProtectFlags new_flags = Vmem::ToNativeFlags(
			Vmem::PROTECTFLAG_READ | Vmem::PROTECTFLAG_WRITE | Vmem::PROTECTFLAG_EXEC
		);
        Vmem::SetProtection(m_newvmt, size, new_flags, &old_flags);
		m_newvmt[Index] = Function;
        Vmem::SetProtection(m_newvmt, size, old_flags, nullptr);
	}
	else
		m_newvmt[Index] = Function;
}