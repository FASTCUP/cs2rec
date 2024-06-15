#include "ffmpipe.hpp"

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
        const std::filesystem::path& ffmpeg_path, std::wstring_view ffmpeg_args, uint32_t timeout_ms, PipeStatus *status
    ) {
        // TODO: Proper utf8 conversion.
        std::string cmdline = ffmpeg_path.string();
        cmdline += ' ';
        cmdline.reserve(cmdline.length() + ffmpeg_args.length());
        for (wchar_t ch : ffmpeg_args)
            cmdline += ch;

        FILE* fptr = popen(cmdline.c_str(), "w");
        if (fptr == nullptr) {
            if (status) *status = PipeStatus::Capture(PipeStatus::Type::CREATE_PROCESS);
            return nullptr;
        }
        
        if (status) *status = PipeStatus::Capture(PipeStatus::Type::OK);
        return PipePtr{new Pipe(fptr)};
    }

    PipeStatus Pipe::Write(const void* data, size_t length) {
        if (m_stdin_w == nullptr)
            return PipeStatus::Capture(PipeStatus::Type::WRITE_PIPE);
        
        size_t count = std::fwrite(data, 1, length, m_stdin_w);
        if (count < length)
            return PipeStatus::Capture(PipeStatus::Type::WRITE_PIPE);
        
        return PipeStatus::Capture(PipeStatus::Type::OK);
    }

    void Pipe::Close(uint32_t timeout_ms, bool terminate) {
        std::fclose(m_stdin_w);
        m_stdin_w = nullptr;
    }
}