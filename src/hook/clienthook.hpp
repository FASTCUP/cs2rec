#pragma once
#include "vmthook.hpp"
#include <prop/AfxHookSource/SourceSdkShared.h>
#include <prop/cs2/sdk_src/public/cdll_int.h>

class ClientHook {
public:
    using ClientFrameStage_t = SOURCESDK::CS2::ClientFrameStage_t;

    void Hook();
    void Unhook();

    void FrameStageNotify(ClientFrameStage_t stage);

private:
    static void Hooked_FrameStageNotify(void* this_, ClientFrameStage_t stage);

    VmtHook m_vmt;
};

inline ClientHook g_client_hook;