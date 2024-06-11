#include "interfaces.hpp"
#include "modulename.hpp"
#include <util/log.hpp>
#include <util/library.hpp>
#include <util/text.hpp>
#include <prop/cs2/Source2Client.h>
#include <cstdlib> // std::exit

namespace Log = Util::Log;

namespace SOURCESDK::CS2 {
    ICvar* g_pCVar = nullptr;
}

namespace Interfaces {

    void Create() {
        Msg = Util::FindSymbol<Tier0MsgFn>(ModuleName::Tier0, "Msg").GetOrAbort();
        Warning = Util::FindSymbol<Tier0MsgFn>(ModuleName::Tier0, "Warning").GetOrAbort();
        Msg("[Cs2Rec] Creating interfaces...\n");
        
        auto client_factory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Client, "CreateInterface").GetOrAbort();
        auto engine_factory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Engine, "CreateInterface").GetOrAbort();
        auto tier0_factory = Util::FindSymbol<CreateInterfaceFn>(ModuleName::Tier0, "CreateInterface").GetOrAbort();

        client = (ISource2Client*)client_factory(SOURCESDK_CS2_Source2Client_VERSION, nullptr);
        AssertPointer(client, SOURCESDK_CS2_Source2Client_VERSION);

        engine_to_client = (RealISource2EngineToClient*)engine_factory(SOURCESDK_CS2_ENGINE_TO_CLIENT_INTERFACE_VERSION, nullptr);
        AssertPointer(engine_to_client, SOURCESDK_CS2_ENGINE_TO_CLIENT_INTERFACE_VERSION);

        cvar = (ICvar*)tier0_factory(SOURCESDK_CS2_CVAR_INTERFACE_VERSION, nullptr);
        AssertPointer(cvar, SOURCESDK_CS2_CVAR_INTERFACE_VERSION);

        movie_recorder = (CMovieRecorder*)engine_to_client->GetMovieRecorder();
        AssertPointer(movie_recorder, "IMovieRecorder");

        

        // NOTE(Cade): This is redundant to avoid patching AdvancedFX code
        g_pCVar = cvar;
    }

    void Destroy() {
        client = nullptr;
        cvar = nullptr;
        g_pCVar = nullptr;
        Msg = nullptr;
        Warning = nullptr;
    }

    void AssertPointer(void* ptr, const char* name) {
        if (ptr == nullptr) {
            Log::Write(Util::Sprintf("%p is missing (nullptr)\n", ptr));
            std::exit(-1);
        }
    }

}