#include "vmthook.hpp"
#include <cstring> // std::memcpy
#include <sys/mman.h> // mprotect

static void SetWriteable(void* addr, size_t len, int* oldProt) {
    mprotect(addr, len, PROT_READ | PROT_WRITE | PROT_EXEC);
    *oldProt = PROT_READ | PROT_EXEC;
}

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
		int oldflags;
        SetWriteable(m_newvmt, size, &oldflags);
		std::memcpy(m_newvmt, m_oldvmt, size);
        mprotect(m_newvmt, size, oldflags);
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
		int oldflags;
		SetWriteable(&m_newvmt[Index], size, &oldflags);
		m_newvmt[Index] = Function;
		mprotect(&m_newvmt[Index], size, oldflags);
	}
	else
		m_newvmt[Index] = Function;
}