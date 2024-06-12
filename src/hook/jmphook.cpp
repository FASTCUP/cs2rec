#include "jmphook.hpp"
#include <cdl.h>

JmpHook::JmpHook() : m_patch(new cdl_jmp_patch) {}
JmpHook::~JmpHook() {
    delete m_patch;
}

void JmpHook::Hook(void* from_code, void* to_code) {
    if (IsHooked())
        Unhook();
    *m_patch = cdl_jmp_attach(&from_code, to_code);
}

void JmpHook::Unhook() {
    cdl_jmp_detach(m_patch);
}