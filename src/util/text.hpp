#pragma once
#include <string>
#include <cstdio>

namespace Util {

    template <class... T>
    void Sprintf(std::string& output, const char* fmt, T&&... args) {
        int count = std::snprintf(nullptr, 0, fmt, args...);
        output.resize(count);
        std::snprintf(output.data(), output.capacity() + 1, fmt, args...);
    }

    template <class... T>
    std::string Sprintf(const char* fmt, T&&... args) {
        std::string str;
        Sprintf(str, fmt, args...);
        return str;
    }

    /// Create a string of spaced hex bytes with a leading space
    static std::string SprintBytes(uint8_t* data, size_t count) {
        std::string codeString;
        for (size_t i = 0; i < count; ++i)
            codeString += Util::Sprintf(" %X", data[i]);
        return codeString;
    }


}