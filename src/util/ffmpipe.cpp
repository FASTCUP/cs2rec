#include "ffmpipe.hpp"
#include "log.hpp"
#include "text.hpp"
#include <unordered_map>
#include <ctime> // time
#include <unistd.h> // getpid, close
#include <sys/socket.h> // send

namespace ffmpipe {
    
    static constexpr const char* FFMPIPE_STDARGS = "-listen 1 -timeout 10000 -nostdin";

    std::string ExpandTemplate(std::string_view str, const std::unordered_map<std::string, std::string>& vars);

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
        std::string_view command,
        uint32_t timeout_ms,
        PipeStatus* status
    ) {
        Util::Log::Write("Pipe::Create\n");
        // Create unique path for FFmpeg socket
        char tmp_path[64];
        std::snprintf(tmp_path, sizeof(tmp_path), "/tmp/cs2rec-%ld-%d", std::time(nullptr), getpid());

        std::unordered_map<std::string, std::string> vars;
        vars["stdargs"] = FFMPIPE_STDARGS;
        vars["video_input"] = std::string{"unix:"} + tmp_path;

        std::string cmdline = ExpandTemplate(command, vars);
        cmdline += " &"; // Run command in background

        Util::Log::Write("cmdserver call\n");
        Util::Log::Write(Util::Sprintf("%s\n", cmdline.c_str()));

        // Create FFmpeg process with cmdline.
        // This uses a server to do it, because CS2 does not like being forked.
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

        Util::Log::Write("Connect pipe\n");
        // Connect to FFmpeg process
        PipePtr pipe {new Pipe};
        if (pipe->m_videosocket.Connect(tmp_path, (timeout_ms + 1) / 50, timeout_ms) == -1) {
            if (status) *status = PipeStatus::Capture(PipeStatus::Type::CREATE_PIPE);
            return nullptr;
        }

        Util::Log::Write("Pipe::Create done\n");

        if (status) *status = PipeStatus::Capture(PipeStatus::Type::OK);
        return pipe;
    }

    PipeStatus Pipe::WriteFrame(const void* data, size_t length) {
        if (m_videosocket.fd == -1)
            return PipeStatus::Capture(PipeStatus::Type::WRITE_PIPE);
        
        size_t count = send(m_videosocket.fd, data, length, 0);
        if (count < length)
            return PipeStatus::Capture(PipeStatus::Type::WRITE_PIPE);
        
        return PipeStatus::Capture(PipeStatus::Type::OK);
    }

    void Pipe::Close(uint32_t timeout_ms, bool terminate) {
        m_videosocket.Close();
    }

    std::string ExpandTemplate(std::string_view str, const std::unordered_map<std::string, std::string>& vars) {
        constexpr size_t MAX_TOKEN_NAME_LEN = 32;

        // Start of current "{{name}}" token, or -1.
        size_t token_begin = (size_t)-1;
        // The gained/lost length as a result of template expansions
        ptrdiff_t offset = 0;

        std::string result;
        result.reserve(str.length());
        for (size_t i = 0; i < str.size(); ++i) {
            result.push_back(str[i]);
            
            // Number of token chars consumed, excluding the current char
            size_t token_len = i - token_begin;

            // If a token has started
            if (str[i] == '{') {
                if (token_begin == (size_t)-1)
                    token_begin = i;
                else if (token_len >= 1 && str[i-1] != '{')
                    token_begin = i; // Invalid Syntax. Restart token.
                else if (token_len == 2) // We're reading "{{{". Advance the beginning.
                    ++token_begin;
            }
            // If a token is ending.
            // Must have consumed "{{*" at minimum.
            else if (str[i] == '}' && token_begin != (size_t)-1 && token_len >= 3) {
                size_t name_begin = token_begin + 2;
                size_t name_len = i - name_begin;
                token_begin = (size_t)-1; // Clear token state now

                // Is valid syntax. Valid tokens only contain '}' where "}}" is present.
                bool is_valid = i + 1 < str.length() && str[i+1] == '}';
                if (!is_valid || name_len > MAX_TOKEN_NAME_LEN)
                    continue;

                std::string name{str.data() + name_begin, name_len};
                auto it = vars.find(name);
                if (it == vars.end())
                    continue;

                // Replace token characters in result with variable
                result.erase(result.begin() + offset + (name_begin-2), result.end());
                result += it->second;
                offset = (ptrdiff_t)result.length() - (ptrdiff_t)i - 2;
                ++i; // Skip second '}' char
            }
        }
        return result;
    }

}
