#pragma once
#include <string_view>
#include <fstream>
#include <mutex>

namespace Util::Log {
    void Write(std::string_view text);

    inline std::mutex _mtx;
    std::ofstream _GetFile();
}