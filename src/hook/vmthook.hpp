#pragma once
#include <cstddef>

class VmtHook
{
public:
    VmtHook() = default;

    // Initializes and immediately hooks
    explicit VmtHook(void* Instance, bool AllInstances = false) { Hook(Instance, AllInstances); }
    ~VmtHook() { Unhook(); }

    bool IsHooked() const { return m_inst; }
    void Hook(void* Instance, bool AllInstances = false);
    void Unhook();

    /// Get the original address of a function
    template <class T>
    T Get(const size_t Index) { return reinterpret_cast<T>(m_oldvmt[Index]); }

    /// Immediately set the new address of a function
    void Set(size_t Index, void* Function);

private:
    /// The hooked instance
    void* m_inst = nullptr;
    void **m_oldvmt = nullptr, **m_newvmt = nullptr;
    size_t m_count = 0;
    /**
     * Indicates that the original VMT data has been overwritten.
     * 
     * When true, `m_oldvmt` is a new copy of that original data.
     * Otherwise, `m_oldvmt` is simply the original VMT pointer.
     */
    bool m_all = false;
};
