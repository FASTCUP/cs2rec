#include <cstdio>
#include <cstdlib> // std::exit
#include <thread>
#include <fstream>
#include "util/library.hpp"
#include "util/log.hpp"
#include "util/text.hpp"

// Hooks
#include "hook/clienthook.hpp"

// SDK
#include "interfaces.hpp"
#include <prop/AfxHookSource/SourceInterfaces.h>
#include <prop/AfxHookSource/SourceSdkShared.h>
#include <prop/cs2/Source2Client.h>

using namespace SOURCESDK;
namespace Log = Util::Log;

static void InitHook() {
    Interfaces::Create();
    g_client_hook.Hook();
}

int __attribute__((constructor)) Main() {
    std::thread{&InitHook}.detach();
    return 0;
}