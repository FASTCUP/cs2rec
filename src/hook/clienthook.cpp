#include <stdafx.h>

#include "clienthook.hpp"
#include "vmthook.hpp"
#include <interfaces.hpp>
#include <util/log.hpp>
#include <util/text.hpp>
#include <afx/WrpConsole.h>
#include <cstdio>

static const int INDEX_FRAME_STAGE_NOTIFY = 36;

CON_COMMAND(sf_record, "Record stuff") {
    printf("Args object: %p\n", args);
    printf("argc: %d\n", args->ArgC());
    printf("argv[0]: %s\n", args->ArgV(0));
    if (args->ArgC() >= 2) {
        printf("argv[1]: %s\n", args->ArgV(1));
    }
}

void ClientHook::Hook() {
    m_vmt.Hook(Interfaces::engineToClient);
    m_vmt.Set(INDEX_FRAME_STAGE_NOTIFY, (void*)Hooked_FrameStageNotify);
}

void ClientHook::FrameStageNotify(ClientFrameStage_t stage) {
    auto o = m_vmt.Get<decltype(Hooked_FrameStageNotify)*>(INDEX_FRAME_STAGE_NOTIFY);
    o(Interfaces::engineToClient, stage);
}

void ClientHook::Hooked_FrameStageNotify(void* this_, ClientFrameStage_t stage) {
    static bool init = false;
    if (stage == ClientFrameStage_t::FRAME_START && !init) {
        init = true;
        Util::Log::Write("Initializing commands");
        WrpRegisterCommands();
    }

    g_client_hook.FrameStageNotify(stage);
}