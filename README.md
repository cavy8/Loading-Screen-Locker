# Loading Screen Locker

An SKSE plugin for Skyrim SE/AE that prevents user-driven rotation and zoom of 3D models on loading screens.

## Compatibility

- **Skyrim SE** (1.5.x) - Compatible (in theory)
- **Skyrim AE** (1.6.x) - Compatible (tested)
- **Skyrim VR** - Not supported
- **Loading screen overhaul mods** - Compatible, other inputs remain functional

## Building from Source

### Requirements
- Visual Studio 2022 with C++ workload
- CMake 3.21+
- vcpkg with `VCPKG_ROOT` environment variable set

### Build Steps
```bash
# Configure
cmake --preset default

# Build
cmake --build build --config Release
```

Output: `build/Release/LoadingScreenLocker.dll`

## License

GPL-3.0 License
