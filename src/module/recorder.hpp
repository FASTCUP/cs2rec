#pragma once
#include <string>
#include <filesystem>
#include <util/ffmpipe.hpp>
#include <vector>

class Recorder {
public:
    // Start recording with the current arguments
    void StartRecording(const std::filesystem::path& output);
    void StopRecording();
    inline bool IsRecording() const { return !m_pipes.empty(); }

    /**
     * Call when the game is writing a movie frame.
     * @param data Array of pixels
     * @param length Length of data in bytes
     */
    void OnWriteFrame(void* data, size_t length);

    std::filesystem::path m_ffmpeg_path = "ffmpeg";
    /// Args before the `-i` flag
    std::string m_ffmpeg_input_args;
    /// Args after the `-i` flag, excluding the output name
    std::string m_ffmpeg_output_args;

private:
    std::vector<ffmpipe::PipePtr> m_pipes;
};

inline Recorder g_recorder;