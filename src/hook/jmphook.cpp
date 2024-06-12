#include "jmphook.hpp"
#include <cdl.h>

JmpHook::JmpHook() : m_patch(new cdl_jmp_patch{0}) {}
JmpHook::~JmpHook() {
    delete m_patch;
}

bool JmpHook::IsHooked() const {
    return m_patch->active;
}

void JmpHook::Hook(void* from_code, void* to_code) {
    if (IsHooked())
        Unhook();
    *m_patch = cdl_jmp_attach(&from_code, to_code);
}

void JmpHook::Unhook() {
    cdl_jmp_detach(m_patch);
}

void* JmpHook::GetOriginal() const {
    return m_patch->trampoline;
}