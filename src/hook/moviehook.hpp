#pragma once
#include "vmthook.hpp"
#include "jmphook.hpp"

class KeyValues;

class MovieHook {
public:
    void Hook();

private:
    static int Hooked_TGAWriter(
        void* this_,
        int width, int height, unsigned int bytesPerPixel, void *pixels,
        int unused, KeyValues *optionsKv
    );

    VmtHook m_movie_vmt;
    JmpHook m_tgawriter_jmp;
};

inline MovieHook g_movie_hook;