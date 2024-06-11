#pragma once

namespace ModuleName {
#ifdef _WIN32
    #define _MODULENAME_PREFIX
    #define _MODULENAME_SUFFIX ".dll"
#else
    #define _MODULENAME_PREFIX "lib"
    #define _MODULENAME_SUFFIX ".so"
#endif

#define _MODULENAME(x) _MODULENAME_PREFIX x _MODULENAME_SUFFIX

    constexpr const char* Client = _MODULENAME("client");
    constexpr const char* Engine = _MODULENAME("engine2");
    constexpr const char* Tier0 = _MODULENAME("tier0");
}