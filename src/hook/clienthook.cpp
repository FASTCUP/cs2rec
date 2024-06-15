#include <stdafx.h>

#include "clienthook.hpp"
#include "vmthook.hpp"
#include <interfaces.hpp>
#include <util/log.hpp>
#include <util/text.hpp>
#include <afx/WrpConsole.h>
#include <cstdio>

using Interfaces::Msg;

static const int INDEX_FRAME_STAGE_NOTIFY = 36;

#if SF_DEBUG

    #include "allhooks.hpp"
    CON_COMMAND(sf_debug, "Resume execution") {
        //#if defined(__GNUC__)
        //    __builtin_trap();
        //#elif defined(_MSC_VER)
        //    __debugbreak();
        //#endif
        Util::Log::Write("AttachDeferredHooks\n");
        _AttachDeferredHooks();
    }

#endif

void ClientHook::Hook() {
    m_vmt.Hook(Interfaces::client);
    m_vmt.Set(INDEX_FRAME_STAGE_NOTIFY, (void*)Hooked_FrameStageNotify);
}

void ClientHook::FrameStageNotify(ClientFrameStage_t stage) {
    auto o = m_vmt.Get<decltype(Hooked_FrameStageNotify)*>(INDEX_FRAME_STAGE_NOTIFY);
    o(Interfaces::client, stage);
}

void ClientHook::Hooked_FrameStageNotify(void* this_, ClientFrameStage_t stage) {
    static bool init = false;
    if (stage == ClientFrameStage_t::FRAME_START && !init) {
        init = true;
        WrpRegisterCommands();
    }

    g_client_hook.FrameStageNotify(stage);
}