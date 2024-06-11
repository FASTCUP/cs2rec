#pragma once
#include <stdafx.h>
#include <prop/AfxHookSource/SourceSdkShared.h>
#include <prop/AfxHookSource/SourceInterfaces.h>
#include <prop/cs2/sdk_src/public/cdll_int.h>
#include <prop/cs2/sdk_src/public/icvar.h>

namespace Interfaces {
    void Create();
    void Destroy();

    using namespace SOURCESDK::CS2;

    inline ISource2EngineToClient* engineToClient = nullptr;
    inline ICvar* cvar = nullptr;
    inline Tier0MsgFn Msg;
    inline Tier0MsgFn Warning;
}