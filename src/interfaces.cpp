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
        Log::Write("Interfaces::Create\n");
        
        auto clientFactory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Client, "CreateInterface").GetOrAbort();
        auto engineFactory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Engine, "CreateInterface").GetOrAbort();
        auto tier0Factory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Tier0, "CreateInterface").GetOrAbort();

        engineToClient = (ISource2EngineToClient*)clientFactory(SOURCESDK_CS2_Source2Client_VERSION, nullptr);
        Log::Write(Util::Sprintf("engineToClient: %p\n", engineToClient));

        cvar = (ICvar*)tier0Factory(SOURCESDK_CS2_CVAR_INTERFACE_VERSION, nullptr);
        Log::Write(Util::Sprintf("cvar: %p\n", cvar));

        // NOTE(Cade): This is redundant to avoid patching AdvancedFX code
        g_pCVar = cvar;
    }

    void Destroy() {
        engineToClient = nullptr;
        cvar = nullptr;
        g_pCVar = nullptr;
    }

}