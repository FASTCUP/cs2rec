#pragma once
#include <stdafx.h>
#include <cstdint>
#include <prop/AfxHookSource/SourceSdkShared.h>
#include <prop/AfxHookSource/SourceInterfaces.h>
#include <prop/cs2/sdk_src/public/cdll_int.h>
#include <prop/cs2/sdk_src/public/icvar.h>

class CMovieRecorder;
class RealISource2EngineToClient;
typedef uint32_t LoggerHandle;

namespace Interfaces {
    void Create();
    void Destroy();

    using namespace SOURCESDK::CS2;
    // NOTE(Cade): advancedfx mistakenly named this after a similar but unrelated interface.
    //             This alias will remain until it is fixed upstream in advancedfx.
    using ISource2Client = SOURCESDK::CS2::ISource2EngineToClient;

    inline ISource2Client* client = nullptr;
    inline RealISource2EngineToClient* engine_to_client = nullptr;
    inline ICvar* cvar = nullptr;
    inline CMovieRecorder* movie_recorder;
    inline Tier0MsgFn Msg = nullptr;
    inline Tier0MsgFn Warning = nullptr;
}

class CMovieRecorder {
    enum EncodeFlags {
        ENCODE_TGA  = 1<<0,
        ENCODE_WAV  = 1<<1,
        ENCODE_JPG  = 1<<2,
        ENCODE_PNG  = 1<<3,
        ENCODE_UNK0 = 1<<4, // Set when "no_unique_dir" arg is present
        ENCODE_AVI  = 1<<5,
        ENCODE_AVISOUND = 1<<6,
    };

    virtual void StartMovie(
        const char* name, uint16_t encode_flags, int width, int height,
        float framerate, int bitrate, int jpeg_quality,
        bool show_codec_prompt, void* unk0, bool unk1_false
    ) = 0;
    virtual void StartMovieCmd(SOURCESDK::CS2::CCommand* cmd) = 0;
    virtual void EndMovie() = 0;
    virtual bool IsRecording() = 0;
    virtual int GetTotalNumFrames() = 0;
    virtual float GetFramerate() = 0;
    virtual void CaptureFrame() = 0;
    virtual void* CaptureSoundIdk(void* unk0, float framerate) = 0;
    virtual void PrintConsoleHelp(LoggerHandle log) = 0;
    virtual uint16_t GetEncodeFlags() = 0; ///<  A combation of @ref EncodeFlags
};

// NOTE(Cade): This name is temporary until issues are resolved in advancedfx
class RealISource2EngineToClient
{
public:
    virtual void _Unknown_000(void) = 0;
    virtual void _Unknown_001(void) = 0;
    virtual void _Unknown_002(void) = 0;
    virtual void _Unknown_003(void) = 0;
    virtual void _Unknown_004(void) = 0;
    virtual void _Unknown_005(void) = 0;
    virtual void _Unknown_006(void) = 0;
    virtual void _Unknown_007(void) = 0;
    virtual void _Unknown_008(void) = 0;
    virtual void _Unknown_009(void) = 0;
    virtual void _Unknown_010(void) = 0;
    virtual void _Unknown_011(void) = 0;
    virtual void _Unknown_012(void) = 0;
    virtual void _Unknown_013(void) = 0;
    virtual void _Unknown_014(void) = 0;
    virtual void _Unknown_015(void) = 0;
    virtual void _Unknown_016(void) = 0;
    virtual void _Unknown_017(void) = 0;
    virtual void _Unknown_018(void) = 0;
    virtual void _Unknown_019(void) = 0;
    virtual void _Unknown_020(void) = 0;
    virtual void _Unknown_021(void) = 0;
    virtual void _Unknown_022(void) = 0;
    virtual void _Unknown_023(void) = 0;
    virtual void _Unknown_024(void) = 0;
    virtual void _Unknown_025(void) = 0;
    virtual void _Unknown_026(void) = 0;
    virtual void _Unknown_027(void) = 0;
    virtual void _Unknown_028(void) = 0;
    virtual void _Unknown_029(void) = 0;
    virtual void _Unknown_030(void) = 0;
    virtual void _Unknown_031(void) = 0;
    virtual void _Unknown_032(void) = 0;
    virtual void _Unknown_033(void) = 0;
    virtual void _Unknown_034(void) = 0;
    virtual void _Unknown_035(void) = 0;
    virtual void _Unknown_036(void) = 0;
    virtual void _Unknown_037(void) = 0;
    virtual bool _Unknown_038(void) = 0;
    virtual void _Unknown_039(void) = 0;
    virtual void _Unknown_040(void) = 0;
    virtual void _Unknown_041(void) = 0;
    virtual void _Unknown_042(void) = 0;
    virtual void _Unknown_043(void) = 0;
    virtual void _Unknown_044(void) = 0;
    virtual void _Unknown_045(void) = 0;
    virtual void _Unknown_046(void) = 0;
    virtual void _Unknown_047(void) = 0;
    virtual void _Unknown_048(void) = 0;
    virtual void _Unknown_049(void) = 0;
    virtual void _Unknown_050(void) = 0;
    virtual void _Unknown_051(void) = 0;
    virtual void _Unknown_052(void) = 0;
    virtual void _Unknown_053(void) = 0;
    virtual void _Unknown_054(void) = 0;
    virtual void _Unknown_055(void) = 0;
    virtual void _Unknown_056(void) = 0;
    virtual void _Unknown_057(void) = 0;
    virtual void _Unknown_058(void) = 0;
    virtual void _Unknown_059(void) = 0;
    virtual CMovieRecorder* GetMovieRecorder(void) = 0; //:60
};