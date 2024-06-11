#pragma once
#include "result.hpp"
#include <dlfcn.h>

namespace Util {

    template <class T = void*>
    Result<T> FindSymbol(const char* libName, const char* symName) {
        void* lib = dlopen(libName, RTLD_LAZY | RTLD_NOLOAD | RTLD_LOCAL);
        if (lib == nullptr) {
            return Result<T>::Err("Library \"%s\" is not loaded", libName);
        }

        void* sym = dlsym(lib, symName);
        if (sym == nullptr) {
            return Result<T>::Err("Symbol \"%s\" not found in library \"%s\"", symName, libName);
        }

        return {(T)sym};
    }
    
}