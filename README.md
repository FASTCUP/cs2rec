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
cp -r addons/ ~/.local/share/Steam/steamapps/common/Counter-Strike\ Global\ Offensive/game/csgo
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
// This value is "ffmpeg" by default
sf_ffmpeg_path ffmpeg

// "-y" to overwrite existing files;
// "-s:v" to set resolution
sf_ffmpeg_input_args -y -loglevel warning -c:v rawvideo -f rawvideo -pix_fmt rgb0 -s:v 1920x1080 -framerate 60

// This can be blank.
// "-c:v huffyuv" chooses the lossless HuffYUV encoder
sf_ffmpeg_output_args -c:v huffyuv

// Start FFmpeg and output to mymovie.avi
// Relative paths on Linux will be relative to cmdserver.
sf_ffmpeg_start mymovie.avi

// Start in-game recorder.
// You MUST include TGA, which is hooked by cs2rec.
// "wav" will output a separate "mymovie.wav" file in the CS2 movie dir.
startmovie mymovie tga wav

// Stop in-game recorder
endmovie

// Stop FFmpeg to finalize the video output
sf_ffmpeg_stop
```

You have to set the FFmpeg args each time the game is launched.
But afterwards you only need to use `sf_ffmpeg_start` and `sf_ffmpeg_stop`.

> In short:
> - Start FFmpeg
> - Start in-game recorder (to trigger hooks that write to FFmpeg)
> - Stop in-game recorder
> - Stop FFmpeg to finish the video
>
> FFmpeg must have every detail manually supplied. This includes:
> - Pixel format (`-pixfmt <fmt>`)
> - Resolution (`-s:v <width>x<height>`)
> - Framerate (`-framerate <fps>`)
>
> This is because cs2rec does not know which resolution and framerate
> will be used with `startmovie`.
