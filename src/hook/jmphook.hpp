#pragma once

class JmpHook {
public:
    JmpHook();
    ~JmpHook();
    JmpHook(JmpHook&&) = delete;
    JmpHook(const JmpHook&) = delete;

    bool IsHooked() const;
    void Hook(void* from_code, void* to_code);
    void Unhook();

    /// Get a pointer to execute the original code
    void* GetOriginal() const;

private:
    // This is a pointer so we don't have to include the C header.
    // Otherwise it screws up other C++ headers.
    // It's really not a big deal.
    struct cdl_jmp_patch* m_patch{0};
};