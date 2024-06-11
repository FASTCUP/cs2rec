#include "interfaces.hpp"
#include "modulename.hpp"
#include <util/log.hpp>
#include <util/library.hpp>
#include <util/text.hpp>
#include <prop/cs2/Source2Client.h>

namespace Log = Util::Log;

namespace SOURCESDK::CS2 {
    ICvar* g_pCVar = nullptr;
}

namespace Interfaces {

    void Create() {
        Msg = Util::FindSymbol<Tier0MsgFn>(ModuleName::Tier0, "Msg").GetOrAbort();
        Warning = Util::FindSymbol<Tier0MsgFn>(ModuleName::Tier0, "Warning").GetOrAbort();
        Msg("[Cs2Rec] Creating interfaces...\n");
        
        auto clientFactory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Client, "CreateInterface").GetOrAbort();
        auto engineFactory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Engine, "CreateInterface").GetOrAbort();
        auto tier0Factory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Tier0, "CreateInterface").GetOrAbort();

        engineToClient = (ISource2EngineToClient*)clientFactory(SOURCESDK_CS2_Source2Client_VERSION, nullptr);
        cvar = (ICvar*)tier0Factory(SOURCESDK_CS2_CVAR_INTERFACE_VERSION, nullptr);

        // NOTE(Cade): This is redundant to avoid patching AdvancedFX code
        g_pCVar = cvar;
    }

    void Destroy() {
        engineToClient = nullptr;
        cvar = nullptr;
        g_pCVar = nullptr;
        Msg = nullptr;
        Warning = nullptr;
    }

}