#include "recorder.hpp"
#include <interfaces.hpp>
#include <afx/WrpConsole.h>
#include <cctype> // std::isspace

/// Combine all args after arg 0 (the command's name)
static std::string CombineArgs(advancedfx::ICommandArgs* args);

CON_COMMAND(sf_ffmpeg_path, "Path of the FFmpeg executable") {
    if (args->ArgC() != 2) {
        Interfaces::Msg("Usage: sf_ffmpeg_path <path>\n");
        Interfaces::Msg("[Recorder] FFmpeg path: %s\n", g_recorder.m_ffmpeg_path.string().c_str());
        return;
    }
    g_recorder.m_ffmpeg_path = args->ArgV(1);
}
CON_COMMAND(sf_ffmpeg_input_args, "FFmpeg input args (before the -i flag)") {
    if (args->ArgC() <= 1) {
        Interfaces::Msg("Usage: sf_ffmpeg_input_args ...\n");
        Interfaces::Msg(
            "Example args:\n"
            "-y -loglevel warning -c:v rawvideo -f rawvideo -pix_fmt rgb0 -s:v 1920x1080 "
            "-framerate 60\n"
        );
        Interfaces::Msg("[Recorder] FFmpeg input args: %s\n", g_recorder.m_ffmpeg_input_args.c_str());
        return;
    }
    g_recorder.m_ffmpeg_input_args = CombineArgs(args);
}
CON_COMMAND(sf_ffmpeg_output_args, "FFmpeg output args (after the -i flag, excluding output path)") {
    if (args->ArgC() <= 1) {
        Interfaces::Msg("Usage: sf_ffmpeg_output_args ...\n");
        Interfaces::Msg(
            "Example args:\n"
            "-c:v huffyuv\n"
        );
        Interfaces::Msg("[Recorder] FFmpeg output args: %s\n", g_recorder.m_ffmpeg_output_args.c_str());
        return;
    }
    g_recorder.m_ffmpeg_output_args = CombineArgs(args);
}
CON_COMMAND(sf_ffmpeg_start, "Start ffmpeg before recording") {
    if (args->ArgC() != 2) {
        Interfaces::Msg("Usage: sf_ffmpeg_start <output-file>\n");
        return;
    }
    g_recorder.StartRecording(args->ArgV(1));
}
CON_COMMAND(sf_ffmpeg_stop, "Stop ffmpeg after recording") {
    g_recorder.StopRecording();
}

void Recorder::StartRecording(const std::filesystem::path& output) {
    if (output.empty()) {
        Interfaces::Warning("[Recorder] Output path is empty\n");
        return;
    } else if (m_ffmpeg_path.empty()) {
        Interfaces::Warning("[Recorder] FFmpeg path is empty\n");
        return;
    } else if (IsRecording()) {
        Interfaces::Warning("[Recorder] Already recording\n");
        return;
    }

    ffmpipe::PipeStatus status;
    m_pipe = ffmpipe::Pipe::Create(
        m_ffmpeg_path, output, m_ffmpeg_input_args, m_ffmpeg_output_args,
        10'000, &status
    );

    if (m_pipe == nullptr) {
        Interfaces::Warning("[Recorder] Failed to create pipe: %s\n", status.ToString().c_str());
        return;
    }

    Interfaces::Msg("[Recorder] Connected to FFmpeg\n");
}

void Recorder::StopRecording() {
    if (!IsRecording()) {
        Interfaces::Msg("[Recorder] No recording is active\n");
        return;
    }

    // Manually close the pipe before freeing.
    // Otherwise freeing will forcefully terminate FFmpeg.
    m_pipe->Close();
    m_pipe = nullptr;
}

void Recorder::OnWriteFrame(void* data, size_t length) {
    if (!IsRecording())
        return;
    
    ffmpipe::PipeStatus status = m_pipe->Write(data, length);
    if (!status.IsOk())
        Interfaces::Warning("[Recorder] pipe->Write error: %s\n", status.ToString().c_str());
}

static std::string CombineArgs(advancedfx::ICommandArgs* args) {
    const char* begin = args->AllArgs();
    while (*begin && !std::isspace(*begin)) // Skip non-whitespace
        ++begin;
    while (*begin && std::isspace(*begin)) // Skip whitespace
        ++begin;
    return begin;
}