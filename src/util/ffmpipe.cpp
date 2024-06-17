#include "ffmpipe.hpp"
#include "log.hpp"
#include "text.hpp"
#include <ctime> // time
#include <unistd.h> // getpid, close
#include <sys/socket.h> // send

namespace ffmpipe {

    PipeStatus PipeStatus::Capture(Type type) {
        return PipeStatus{type};
    }

    std::string PipeStatus::ToString() const
    {
        if (type == PipeStatus::Type::OK)
            return "No error.";

        std::stringstream message;

        switch (type)
        {
        case PipeStatus::Type::CREATE_PIPE:     message << "Failed to create pipe."; break;
        case PipeStatus::Type::CREATE_PROCESS:  message << "Failed to create process."; break;
        case PipeStatus::Type::WRITE_PIPE:      message << "Failed to write to pipe."; break;
        case PipeStatus::Type::WAIT_FAILURE:    message << "Failed to wait for object(s)."; break;
        case PipeStatus::Type::OTHER:           message << "Failed to create pipe."; break;
        default:
            message << "Unknown error.";
            break;
        }

        return message.str();
    }

    PipePtr Pipe::Create(
        const std::filesystem::path& ffmpeg_path,
        const std::filesystem::path& output_path,
        std::string_view input_args,
        std::string_view output_args,
        uint32_t timeout_ms,
        PipeStatus* status
    ) {
        // Create unique path for FFmpeg socket
        char tmp_path[64];
        std::snprintf(tmp_path, sizeof(tmp_path), "/tmp/cs2rec-%ld-%d", std::time(nullptr), getpid());

        Util::Log::Write("Formatting cmdline\n");

        // TODO: Proper string conversions.
        // For now, I will assume std::filesystem::path is always char.
        static_assert(sizeof(std::filesystem::path::value_type) == 1, "std::filesystem::path must use 1-byte chars");
        std::string cmdline = Util::Sprintf(
            // NOTE(Cade): Uses '&' to run FFmpeg in background.
            //             Includes 10s timeout for ffmpeg to receive a client.
            "%s  -listen 1 -timeout 10000 -nostdin %.*s -i unix:%s %.*s %s &",
            ffmpeg_path.c_str(),
            (int)input_args.length(), input_args.data(),
            tmp_path,
            (int)output_args.length(), output_args.data(),
            output_path.c_str()
        );
        
        // Create FFmpeg process with args.
        // This uses a server to do it, because CS2 does not like being forked.
        Util::Log::Write(Util::Sprintf("Pipe::Create -> %s\n", cmdline.c_str()));
        {
            LocalSocket cmdclient;
            if (cmdclient.Connect("/tmp/cs2rec-exec", 0, 0) == -1) {
                if (status) *status = PipeStatus::Capture(PipeStatus::Type::CREATE_PROCESS);
                return nullptr;
            }

            if (send(cmdclient.fd, cmdline.c_str(), cmdline.size(), 0) == -1) {
                if (status) *status = PipeStatus::Capture(PipeStatus::Type::CREATE_PROCESS);
                return nullptr;
            }
        }

        // Connect to FFmpeg process
        PipePtr pipe {new Pipe};
        if (pipe->m_socket.Connect(tmp_path, (timeout_ms + 1) / 50, timeout_ms) == -1) {
            if (status) *status = PipeStatus::Capture(PipeStatus::Type::CREATE_PIPE);
            return nullptr;
        }

        if (status) *status = PipeStatus::Capture(PipeStatus::Type::OK);
        return pipe;
    }

    PipeStatus Pipe::Write(const void* data, size_t length) {
        if (m_socket.fd == -1)
            return PipeStatus::Capture(PipeStatus::Type::WRITE_PIPE);
        
        size_t count = send(m_socket.fd, data, length, 0);
        if (count < length)
            return PipeStatus::Capture(PipeStatus::Type::WRITE_PIPE);
        
        return PipeStatus::Capture(PipeStatus::Type::OK);
    }

    void Pipe::Close(uint32_t timeout_ms, bool terminate) {
        m_socket.Close();
    }

}
