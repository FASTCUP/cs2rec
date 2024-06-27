#include "recorder.hpp"
#include <interfaces.hpp>
#include <util/log.hpp>
#include <util/text.hpp>
#include <filesystem>

CON_COMMAND(sf_ffmpeg_start, "Start ffmpeg before recording") {
    if (args.ArgC() <= 1) {
        Msg(
            "Usage: sf_ffmpeg_start <command>\n"
            "Spawns a new FFmpeg instance.\n"
            "`command` will expand valid variables with the \"{{name}}\" syntax.\n"
            "Variables:\n"
            "- {{stdargs}}: Required arguments to be included at the start.\n"
            "- {{video_input}}: The video input. May be a random pipe or socket name.\n"
            "Example:\n"
            "  sf_ffmpeg_start ffmpeg {{stdargs}} -y -loglevel warning\n"
            "    -c:v rawvideo -f rawvideo -pix_fmt rgb0 -s:v 1920x1080 -framerate 60\n"
            "    -i {{video_input}} -c:v huffyuv output.avi\n"
            "The input format must be raw. The pixel format is commonly rgb0.\n"
            "The input framerate, resolution, and format must be provided exactly.\n"
        );
        return;
    }
    g_recorder.StartRecording(args.ArgS());
}
CON_COMMAND(sf_ffmpeg_stop, "Stop ffmpeg after recording") {
    g_recorder.StopRecording();
}

static std::filesystem::path FindLatestWav();

void Recorder::StartRecording(std::string_view command) {
    if (command.empty()) {
        Warning("[Recorder] Command is empty\n");
        return;
    } else if (IsRecording()) {
        Warning("[Recorder] Spawning another FFmpeg process\n");
    }

    ffmpipe::PipeStatus status;
    ffmpipe::PipePtr pipe = ffmpipe::Pipe::Create(command, 10'000, &status);
    if (pipe == nullptr) {
        Warning("[Recorder] Failed to create pipe: %s\n", status.ToString().c_str());
        return;
    }

    m_pipes.emplace_back(std::move(pipe));
    Msg("[Recorder] Connected to FFmpeg\n");
}

void Recorder::StopRecording() {
    if (!IsRecording()) {
        Msg("[Recorder] No recording is active\n");
        return;
    }

    // Manually close the pipe before freeing.
    // Otherwise freeing will forcefully terminate FFmpeg.
    for (ffmpipe::PipePtr pipe : m_pipes)
        pipe->Close();
    Cleanup();
}

void Recorder::OnWriteFrame(void* data, size_t length) {
    if (!IsRecording())
        return;
    
    for (ffmpipe::PipePtr pipe : m_pipes) {
        ffmpipe::PipeStatus status = pipe->WriteFrame(data, length);
        if (!status.IsOk())
            Warning("[Recorder] pipe->Write error: %s\n", status.ToString().c_str());
    }

    if (m_current_frame == 0) {
        std::filesystem::path wav_path = FindLatestWav();
        if (!wav_path.empty()) {

        } else {
            Warning("[Recorder] Failed to locate wav file\n");
        }
    }

    ++m_current_frame;
}

void Recorder::Cleanup() {
    m_pipes.clear();
    m_current_frame = 0;
}

static std::filesystem::path FindLatestWav() {
    // Find directory of game/csgo/movie.
    // CS2 runs in game/bin/linuxsteamrt64/
    std::filesystem::path path = std::filesystem::current_path();
    while (!path.empty() && path.stem() != "game") {
        path = path.parent_path();
    }

    path = path / "csgo" / "movie";

    // Paths are named yyyy_mm_dd_hh_mm_ss.
    // Choose the latest path in alphabetical order.
    std::filesystem::path latest_movie;
    for (const auto& entry : std::filesystem::directory_iterator{path}) {
        if (!entry.is_directory())
            continue;
        if (latest_movie.empty() || latest_movie > entry.path())
            latest_movie = entry.path();
    }
    
    //std::filesystem::path wav_path;
    //if (const auto& entry : std::filesystem::directory_iterator{latest_movie}) {
    //    if (entry.extension() == ".wav") {
    //        return entry.path();
    //    }
    //}

    return {};
}