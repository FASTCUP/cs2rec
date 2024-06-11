#include "log.hpp"

namespace Util::Log {
    void Write(std::string_view text) {
        std::scoped_lock lock{_mtx};
        _GetFile() << text;
    }

    std::ofstream _GetFile() {
        return std::ofstream{"recorder_log.txt", std::ios::app};
    }
}