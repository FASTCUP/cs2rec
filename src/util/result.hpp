#pragma once
#include <string>
#include <cstdio> // std::sprintf
#include <utility> // std::move, std::forward
#include "text.hpp"
#include "log.hpp"

namespace Util {

    /**
     * The result is a success when `err` is empty
     */
    template <class T>
    struct Result {
        T value;
        std::string err;

        inline bool IsOk() const { return err.empty(); }
        inline operator bool() const { return IsOk(); }
        T& GetOrAbort() {
            if (!IsOk()) {
                Log::Write(Sprintf("%s\n", err.c_str()));
                exit(-1);
            }
            return value;
        }

        template <class ...FmtArgs>
        static Result Err(const char* fmt, FmtArgs&&... args) {
            Result res;
            Sprintf(res.err, fmt, args...);
            return res;
        }

        static Result Ok(T&& value) {
            return Result{std::move(value), std::string{}};
        }
    };

}