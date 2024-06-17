A recorder for Counter-Strike 2. Supports Linux on AMD64.

## Build

```sh
mkdir build     # Make build folder
cd build
cmake ..        # Configure
cmake --build . # Compile
```

## Usage

First, Linux users need the command server. In the build folder, run:
```sh
./src/cmdserver/cmdserver
```
> *This local server will receive and execute commands sent by cs2rec.*
> *It will primarily spawn background FFmpeg processes.*

Next, inject or load `cs2rec` (`libcs2rec.so` on Linux) into the game.
For Linux, I use https://github.com/kubo/injector.
Build in the source folder with `make`, then run:
```sh
cmd/injector -n cs2 path-to-libcs2rec.so
```

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

## Debug

On Linux, a workaround is needed to debug after injecting (without execution escaping you).

Enable `SF_DEBUG` like so:
```sh
cmake .. -D SF_DEBUG=1
```
Now recompile it, inject into the game, and run `sf_debug` using the in-game console.
Execution of cs2rec will resume.