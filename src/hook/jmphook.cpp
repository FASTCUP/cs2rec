#include "jmphook.hpp"
#include <util/log.hpp>

extern "C" {
#include <cdl.h>
}

JmpHook::JmpHook() : m_patch(new cdl_jmp_patch{0}) {}
JmpHook::~JmpHook() {
    delete m_patch;
}

bool JmpHook::IsHooked() const {
    Util::Log::Write("IsHooked\n");
    return m_patch->active;
}

void JmpHook::Hook(void* from_code, void* to_code) {
    if (IsHooked())
        Unhook();
    Util::Log::Write("Calling cdl_jmp_attach\n");
    *m_patch = cdl_jmp_attach(&from_code, to_code);
}

void JmpHook::Unhook() {
    Util::Log::Write("Unhook\n");
    cdl_jmp_detach(m_patch);
}

void* JmpHook::GetOriginal() const {
    return m_patch->trampoline;
}