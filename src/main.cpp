#include <cstdio>
#include <cstdlib> // std::exit
#include <thread>
#include <fstream>
#include "util/library.hpp"
#include "util/log.hpp"
#include "util/text.hpp"
#include "interfaces.hpp"
#include "hook/allhooks.hpp"

using namespace SOURCESDK;
namespace Log = Util::Log;

static void InitHook() {
    Interfaces::Create();
    AttachAllHooks();
}

int __attribute__((constructor)) Main() {
    std::thread{&InitHook}.detach();
    return 0;
}