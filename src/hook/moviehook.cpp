#include "moviehook.hpp"
#include <mem/module.h>
#include <modulename.hpp>
#include <interfaces.hpp>
#include <util/text.hpp>
#include <util/log.hpp>
#include <util/ffmpipe.hpp>
#include <afx/WrpConsole.h>

/*
 * NOTE(Cade):
 *   You can find TGAWriter with the string:
 *     "TGAWriter: Failed to serialize screenshot pixels (%d x %d, %s).\n"
 * 
 *   TGAWriter is a virtual function, but its class's inheritance is wacky.
 *   I've already wasted time thinking of better (cross-platform) methods.
 *   This will have to do. The alternative involves wrangling different vtable layouts.
 */
#if defined(_WIN32)
    constexpr const char* SIG_TGAWRITER = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 81 EC ? ? ? ? 48 8B 9C 24";
#elif defined(__linux__)
    constexpr const char* SIG_TGAWRITER = "55 4D 89 C2 48 89 E5 41 57 41 56";
#endif

using namespace DynLibUtils;

static ffmpipe::PipePtr ffmpeg = nullptr;

CON_COMMAND(sf_record, "Begin recording stuff") {
    ffmpipe::PipeStatus status;
    ffmpeg = ffmpipe::Pipe::Create("ffmpeg",
        L"-y -loglevel warning -c:v rawvideo -f rawvideo "
        L"-pix_fmt rgb0 -s:v 1920x1080 -framerate 60 -i - test.mp4",
        10'000, &status
    );

    if (ffmpeg == nullptr) {
        Util::Log::Write(Util::Sprintf("FFmpipe status: %s\n", status.ToString().c_str()));
        return;
    }
}

CON_COMMAND(sf_stoprecord, "Stop recording stuff") {
    if (ffmpeg != nullptr) {
        ffmpeg->Close();
        ffmpeg = nullptr;
    }
}

void MovieHook::Hook() {
    CModule engine;
    engine.InitFromName(ModuleName::Engine, true);
    void* tgawriter = (void*)engine.FindPattern(SIG_TGAWRITER).GetPtr();
    Interfaces::AssertPointer(tgawriter, "CScreenshotService::TGAWriter");

    m_tgawriter_jmp.Hook(tgawriter, (void*)Hooked_TGAWriter);
}

int MovieHook::Hooked_TGAWriter(
    void* this_,
    int width, int height, unsigned int bytesPerPixel, void *pixels,
    int unused, KeyValues *optionsKv
) {
    //Util::Log::Write(Util::Sprintf("Hooked_TGAWriter(%p, %d, %d, %u, %p, %d, %p)\n",
    //    this_, width, height, bytesPerPixel, pixels, unused, optionsKv
    //));

    if (ffmpeg == nullptr)
        return 0;
    
    auto status = ffmpeg->Write(pixels, (size_t)width * (size_t)height * (size_t)bytesPerPixel);
    if (!status.IsOk()) {
        Util::Log::Write(Util::Sprintf("FFmpipe status: %s\n", status.ToString().c_str()));
    }

    //using FunctionType = decltype(Hooked_TGAWriter)*;
    //FunctionType o = (FunctionType)g_movie_hook.m_tgawriter_jmp.GetOriginal();
    //o(this_, width, height, bytesPerPixel, pixels, unused, optionsKv);
    return 0;
}