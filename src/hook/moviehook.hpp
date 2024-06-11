#pragma once
#include "vmthook.hpp"

class MovieHook {
public:
    void Hook();

private:
    VmtHook m_movie_vmt;
    VmtHook m_screenshot_vmt;
};