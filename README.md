A recorder for Counter-Strike 2. Supports Linux on AMD64.

## Build

Requirements:
- Python 3
- GCC (For Linux users)
- [ambuild](https://github.com/alliedmodders/ambuild)

Beforehand, ensure all your git submodules are up-to-date:
```sh
git submodule update --init --recursive
```

Then you can proceed to build:
```sh
mkdir build # Make build folder
cd build
python3 ../configure.py --sdks=cs2
ambuild .
```

## Usage

First, install the mod by copying the addons folder into your game.
For Linux users, the command may look like this:
```sh
cp -r package/addons/ ~/.local/share/Steam/steamapps/common/Counter-Strike\ Global\ Offensive/game/csgo
```

Next, Linux users must host the local command server.
It is located here in the build folder:
```sh
cmdserver/linux-x86_64/cmdserver
```
> *This local server will receive and execute commands sent by cs2rec.*
> *It will primarily spawn background FFmpeg processes.*

Finally, use the in-game console commands
Example:
```
// The following command spawns FFmpeg in the background.
// You can run this many times to spawn many processes.
// The variables {{stdargs}} and {{video_input}} will be expanded.
sf_ffmpeg_start ffmpeg {{stdargs}} -y -loglevel warning -c:v rawvideo -f rawvideo -pix_fmt rgb0 -s:v 1920x1080 -framerate 60 -i {{video_input}} -c:v huffyuv output.avi

// Start in-game recorder.
// You MUST include TGA, which is hooked by cs2rec.
// "wav" will output a separate "mymovie.wav" file in the CS2 movie dir.
// "framerate 60" must match the framerate given to the FFmpeg command.
startmovie mymovie tga wav framerate 60

// Stop in-game recorder
endmovie

// Stop the spawned FFmpeg processes to finalize the video output
sf_ffmpeg_stop
```

That is all.

> FFmpeg must have every detail manually supplied. This includes:
> - Pixel format (`-pixfmt <fmt>`, commonly `rgb0`)
> - Resolution (`-s:v <width>x<height>`)
> - Framerate (`-framerate <fps>`)
>
> This is because cs2rec does not know which resolution and framerate
> will be used with `startmovie`.
