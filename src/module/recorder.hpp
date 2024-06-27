#pragma once
#include <string>
#include <filesystem>
#include <util/ffmpipe.hpp>
#include <vector>
#include <fstream>

class Recorder {
public:
    /// Start recording with the current arguments
    /// @param command An FFmpeg command
    void StartRecording(std::string_view command);
    void StopRecording();
    inline bool IsRecording() const { return !m_pipes.empty(); }

    /**
     * Call when the game is writing a movie frame.
     * @param data Array of pixels
     * @param length Length of data in bytes
     */
    void OnWriteFrame(void* data, size_t length);

private:
    /// Clear all state regardless of state.
    void Cleanup();

    std::vector<ffmpipe::PipePtr> m_pipes;
    size_t m_current_frame = 0;
};

inline Recorder g_recorder;