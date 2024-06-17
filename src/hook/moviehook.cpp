#include "moviehook.hpp"
#include <mem/module.h>
#include <modulename.hpp>
#include <interfaces.hpp>
#include <util/text.hpp>
#include <util/log.hpp>
#include <util/ffmpipe.hpp>
#include <afx/WrpConsole.h>
#include <module/recorder.hpp>

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

void MovieHook::Hook() {
    CModule engine;
    engine.InitFromName(ModuleName::Engine, true);
    void* tgawriter = (void*)engine.FindPattern(SIG_TGAWRITER).GetPtr();
    Interfaces::AssertPointer(tgawriter, "CScreenshotService::TGAWriter");

    m_tgawriter_jmp.Hook(tgawriter, (void*)Hooked_TGAWriter);
}

int MovieHook::Hooked_TGAWriter(
    void* this_,
    int width, int height, unsigned int unk0, void *pixels,
    int frame_buffer_size, KeyValues *optionsKv
) {
    // NOTE(Cade): unk0 is 0, but I've seen it as 4 on Windows.
    //             Perhaps it's an enum for pixel formats.
    //             We can deduce bytes-per-pixel using `frame_buffer_size / width / height`.
    
    g_recorder.OnWriteFrame(pixels, frame_buffer_size);

    //using FunctionType = decltype(Hooked_TGAWriter)*;
    //FunctionType o = (FunctionType)g_movie_hook.m_tgawriter_jmp.GetOriginal();
    //o(this_, width, height, bytesPerPixel, pixels, unused, optionsKv);
    return 0;
}