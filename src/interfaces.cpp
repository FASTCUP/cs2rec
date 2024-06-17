#include "interfaces.hpp"
#include "modulename.hpp"
#include <util/log.hpp>
#include <util/library.hpp>
#include <util/text.hpp>
#include <prop/cs2/Source2Client.h>
#include <cstdlib> // std::exit
#include <thread> // std::this_thread::sleep

namespace Log = Util::Log;

namespace SOURCESDK::CS2 {
    ICvar* g_pCVar = nullptr;
}

namespace Interfaces {

    void Create() {
        // Wait for all the modules to load before starting.
        // NOTE(Cade): In Source 1, the client module is loaded last.
        //             But I'm not sure about Source 2.
        while (!Util::IsModuleLoaded(ModuleName::Client)) {
            std::this_thread::sleep_for(std::chrono::milliseconds{100});
        }

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
            Log::Write(Util::Sprintf("Missing %s (nullptr)\n", name));
            std::abort();
        }
    }

}