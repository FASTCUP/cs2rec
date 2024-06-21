#include "interfaces.hpp"
#include "modulename.hpp"
#include <util/log.hpp>
#include <util/library.hpp>
#include <util/text.hpp>
#include <cstdlib> // std::exit
#include <thread> // std::this_thread::sleep

namespace Log = Util::Log;

namespace Interfaces {
    void AssertPointer(void* ptr, const char* name) {
        if (ptr == nullptr) {
            Log::Write(Util::Sprintf("Missing %s (nullptr)\n", name));
            std::abort();
        }
    }

}