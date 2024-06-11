Various structures and interface function indices

- Game dir: `home/user/.local/share/Steam/steamapps/common/Counter-Strike Global Offensive`
- Game binary: `game/bin/linuxsteamrt64/cs2`

iface HostStateMgr
------------------
	11. void Quit() // Quits game

iface SoundSystem
------------------
	45. void StartRecordingIdk(const char* path) // path is movie path not including file extension

iface EngineServiceMgr
----------------------
	16. void Idk()
	18. void* GetWindowPtr() // Ptr to generic window data.
		// Contains OS-specific handles, ptr to SDL state, and window title
	24. void GetScreenSize(int& width, int& height)
	
iface Avi
---------
	11. void Idk(void* ptr); // Takes IngineServiceMgr::GetWindowPtr
		// Calls Plat_WindowToOsSpecificHandle with the given ptr
	12. int16_t StartRecordingIdn(const char* absolutePath);
		// Will run the encoder choice dialog until closed.
		// Returns 0, but can be -1 also.

iface SoundSystem
------------------
	46. void Idk(int16_t) // Takes result of Avi::Idk2, if result != -1

iface CMovieRecorder
--------------------
	0. void StartMovie(
			const char* movieName, uint16_t encode_flags,
			int width, int height, float framerate, int bitrate /* Mbps, I think */,
			int jpegQuality /*0-100*/, bool show_codec_prompt, void* idk, bool usuallyFalseIdk
		) // This is called by StartMovie. Default bitrate is 45.
	1. void StartMovieCmd(CCommand* cmd) // Called using a startmovie command invocation
	2. void EndMovie()
	3. bool IsMovieNameSet() // This can indicate whether a recording is active
	4. int GetTotalNumFrames() // Number of frames recorded so far
	5. float GetFramerate()
	6. void CaptureFrame() // It's a no-op unless recording has been started.
		// Call stack:
		// GameLoop
		// -> CLoopTypeClientServer::DoFrameIdk
		// -> DispatchEvent // (before FrameBoundary(). But screenshots are handled on FrameBoundary)
		// -> DispatchAsyncEvents // A bogus name given by me
		// -> RenderService::OnClientInput
		// -> CMovieRender::CaptureFrame

Members:
```
00000128 movie_name      dq ?                    ; CUtlString
00000130 total_num_frames dd ?
00000134 record_flags    dw ?                    ; enum RecordFlag
00000136 field_136       db ?
00000137 field_137       db ?
00000138 encode_quality  dd ?
0000013C encode_fps      dd ?
00000140 encode_mbps     dd ?
```

iface CScreenshotService
------------------------
There seem to be two vtables.
One has many functions that don't do much.
The other has the following:
	1. TGAWriter(
			unsigned int width, int height, unsigned int bytesPerPixel, void *pixels, int unknown, KeyValues *optionsKv
		)
		// Options:
		// - asyncwrite: 1|0
		// - p4edit: 1|0
		// - filename
		// pixels: An array of RGBA bytes
	2. JPGWriter
	3. PNGWriter

enum Video flags
----------------
When recording AVI (no other options) the flags are 0x3B
	1. tga
	2. wav
	1|2. Set when "no_unique_dir" is not active, or when no args are given
	4. jpg
	8. png
	16. set with "no_unique_dir" cmd arg.
		AV format, as opposed to image sequence?
		Movie output is simply "name". Otherwise it is "movie/x_x_x_x_x_x/name"
	32. avi
	64. avisound

Events
------

```cpp
struct EventDispatcher {
	const char* name;
	uint32_t unk0 = 0x20;
	uint32_t unk1 = 0;
	
};

struct EventType {
	EventType* self; // Points to itself
	const char* name; // One of the type names
	const char* module; // "engine2", for example
	uint64_t unk0;
	uint64_t unk1;
	const char* unk2; // Usually says "m_LoopState"
};

EventType* v22 = off_7FFD53340A20;
Msg("%s: %5d:  DispatchEvent( %s )\n", a1->nameIdk, a1->unk0, *(v22 + 8));
// Output:
//   game: 328457:  DispatchEvent( EventClientOutput_t )
```

Event type names:
- "EventClientPollInput_t"
- "EventClientProcessInput_t"
- "EventClientProcessGameInput_t"
- "EventClientPollNetworking_t"
- "EventClientProcessNetworking_t"
- "EventSimpleLoopFrameUpdate_t"
- "EventClientFrameSimulate_t"
- "EventSetTime_t"
- "EventClientPreOutput_t"
- "EventClientOutput_t"
- "EventClientPostOutput_t"
- "EventFrameBoundary_t"
- "EventClientSceneSystemThreadStateChange_t"
- "EventServerAdvanceTick_t"
- "EventServerPollNetworking_t"
- "EventServerProcessNetworking_t"
- "EventServerSimulate_t"
- "EventServerPostSimulate_t"
- "EventServerPostAdvanceTick_t"
- "EventClientAdvanceTick_t"
- "EventClientPostAdvanceTick_t"
- "EventClientPreSimulate_t"
- "EventClientSimulate_t"
- "EventClientPostSimulate_t"