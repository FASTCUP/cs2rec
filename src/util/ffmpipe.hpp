/*
The following source code is a Linux port of ffmpipe.
Source: https://github.com/cademtz/ffmpipe

This may (and should) be merged into the ffmpipe repo in the future.
*/

#pragma once
#include <cstdio>
#include <cstdint>
#include <memory>
#include <filesystem>
#include <string_view>
#include "localsocket.hpp"

namespace ffmpipe {
    struct PipeStatus
    {
        enum class Type
        {
            OK = 0,
            /* Failed to create a named pipe */
            CREATE_PIPE,
            /* Failed to spawn the ffmpeg process */
            CREATE_PROCESS,
            /* Failed to write to named pipe */
            WRITE_PIPE,
            /* WaitForMultipleObjects failure/timeout */
            WAIT_FAILURE,
            /* Other WinAPI error */
            OTHER
        };

        Type type;
        //DWORD last_error; // The value returned by Win32 `GetLastError` function

        // Create a status containing the current win32 error
        static PipeStatus Capture(Type type);
        /// @brief Create a detailed formatted status message 
        std::string ToString() const;
        inline bool IsOk() const { return type == Type::OK; }
    };

    using PipePtr = std::shared_ptr<class Pipe>;

    /**
     * @brief Run FFmpeg and write to stdin.
     * 
     * Operations are not thread-safe.
     */
    class Pipe {
    public:
        /**
         * @brief Create a new pipe and run FFmpeg.
         *
         * FFmpeg will not read from stdin unless you pass the appropriate argument.
         *
         * Example:
         *   ffmpeg {{stdargs}} -c:v rawvideo -f rawvideo -pix_fmt rgb24 -s:v 720x1280 -framerate 60 -i {{video_input}} -y output.mp4
         * This takes 720x1280 RGB frames as input. `-i -` is required to read from stdin. `-y` allows overwriting files.
         *
         * @param command The ffmpeg command. " &" is appended to run it in the background.
         * 
         * Templates use "{{name}}" syntax and are expanded in the commmand string.
         * - {{stdargs}}: Standard arguments. Use after ffmpeg like "ffmpeg {{stdargs}}".
         * - {{video_input}}: Video input. May expand to a named socket.
         * 
         * Templates are not expanded if the name does not exist.
         * 
         * @param timeout_ms Timeout in milliseconds for reading and writing.
         * @param status Pointer to optional status code.
         * @return `nullptr` on failure.
         */
        static PipePtr Create(
            std::string_view command,
            uint32_t timeout_ms = 10'000,
            PipeStatus* status = nullptr
        );

        /// @brief Write all data to pipe. Blocking.
        /// @return `false` on failure.
        PipeStatus WriteFrame(const void* data, size_t length);

        /**
         * @brief Close the pipe and wait for program exit. Blocking.
         * @details Don't call during write operations.
         * @param timeout_ms Timeout in milliseconds.
         * @param terminate If true, the child process is terminated after timeout.
         */
        void Close(uint32_t timeout_ms = (uint32_t)-1, bool terminate = true);

        Pipe(Pipe&&) = delete;
        Pipe(const Pipe&) = delete;
        ~Pipe() { Close(); }

    private:
        Pipe() {}

        LocalSocket m_videosocket; ///< Streams data to FFmpeg
    };
}