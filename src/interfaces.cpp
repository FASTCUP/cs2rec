#include "interfaces.hpp"
#include "modulename.hpp"
#include <util/log.hpp>
#include <util/library.hpp>
#include <util/text.hpp>
#include <cstdlib> // std::exit
#include <thread> // std::this_thread::sleep

namespace Log = Util::Log;

namespace Interfaces {

    bool Create(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) {
        GET_V_IFACE_CURRENT(GetEngineFactory, engine_to_client, ISource2EngineToClient, SOURCE2ENGINETOCLIENT_INTERFACE_VERSION);
        GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);

        movie_recorder = (CMovieRecorder*)engine_to_client->GetMovieRecorder();
        if (!movie_recorder) {
            if (error && maxlen)
                ismm->Format(error, maxlen, "Could not find interface CMovieRecorder");
            return false;
        }

        return true;
    }

    void AssertPointer(void* ptr, const char* name) {
        if (ptr == nullptr) {
            Log::Write(Util::Sprintf("Missing %s (nullptr)\n", name));
            std::abort();
        }
    }

}