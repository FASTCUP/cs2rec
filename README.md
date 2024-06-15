A recorder for Counter-Strike 2. Supports Linux on AMD64.

## Build

```sh
mkdir build     # Make build folder
cd build
cmake ..        # Configure
cmake --build . # Compile
```

## Debug

On Linux, a workaround is needed to debug after injecting (without execution escaping you).

Enable `SF_DEBUG` like so:
```sh
cmake .. -D SF_DEBUG=1
```
Now recompile it, inject into the game, and run `sf_debug` using the in-game console.
Execution of cs2rec will resume.