# Loading Screen Locker

An SKSE plugin for Skyrim SE/AE/VR that prevents user-driven rotation and zoom of 3D models on loading screens.

## Features

- **Disables mouse/thumbstick input** from rotating/zooming loading screen 3D models
- **Preserves all other inputs** - buttons, keyboard, D-pad, triggers, mouse buttons, and scroll wheel still work normally
- **Compatible with loading screen overhaul mods** - only touches rotation/zoom axes. Tested with Loading Menu Overhaul
- **Configurable** via INI file

## Why?

This mod is intended to be used with 2D loading screen replacers, such as this one.

## Installation

1. Install [SKSE](https://skse.silverlock.org/) for your Skyrim version
2. Install the Address Library matching your game: [SE/AE Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/32444) or [VR Address Library](https://www.nexusmods.com/skyrimspecialedition/mods/58101). For 1.7.104, use an AE library containing `versionlib-1-7-104-0.bin`.
3. Copy `LoadingScreenLocker.dll` to `Data/SKSE/Plugins/`
4. (Optional) Copy `LoadingScreenLocker.ini` to customize settings

## Configuration

Edit `Data/SKSE/Plugins/LoadingScreenLocker.ini`:

```ini
[General]
bEnable=true              ; Master enable/disable

[Filtering]
bDisableMouseMove=true    ; Block mouse movement rotation
bDisableThumbsticks=true  ; Block both thumbstick axes

[UserEventGate]
bUserEventGate=true       ; Only filter specific user events (safer)
sUserEventAllowlist=rotate,look  ; User events to filter

[Debug]
bDebugLogging=false       ; Enable debug log output
```

### User Event Gating

When `bUserEventGate=true` (default), the plugin only filters input events that have user event names matching the allowlist. This improves compatibility with loading screen overhaul mods that may repurpose mouse/thumbstick input for custom UI interactions.

Set `bUserEventGate=false` for more aggressive filtering that zeros all mouse movement and thumbstick input during loading screens.

## Compatibility

- **Skyrim SE 1.5.97** - Enabled in the shared DLL
- **Skyrim AE 1.6.x / 1.7.x** - Enabled in the shared DLL, including 1.6.1130, 1.6.1170, GOG 1.6.1179, and 1.7.104
- **Skyrim VR 1.4.15** - Enabled in the shared DLL
- **Loading screen overhaul mods** - Compatible, other inputs remain functional

The plugin uses CommonLibSSE-NG's runtime selection and Address Library relocations rather than fixed executable addresses. Each runtime requires its matching SKSE and Address Library. These build targets do not imply in-game testing on every listed version or guarantee compatibility with future game updates.

To verify a runtime, launch through SKSE, check `LoadingScreenLocker.log` for `MenuControls::ProcessEvent hooked`, and confirm that mouse movement and thumbstick axes cannot manipulate loading screen models. Buttons and normal gameplay input should still work. VR testing should also check controller input during loading screens.

## Building from Source

### Requirements
- Visual Studio 2022 with C++ workload
- CMake 3.21+
- vcpkg with `VCPKG_ROOT` environment variable set
- Git and internet access for the pinned CommonLibSSE-NG source and its OpenVR submodule

CommonLibSSE-NG is built from [alandtse's `ng` branch](https://github.com/alandtse/CommonLibSSE-NG/tree/ng), pinned to version **11.0.0** (`94faaed0c60eddd8347767f2d4d29a97c93bde8c`). SE, AE, and VR are enabled together. vcpkg supplies the supporting libraries using a pinned registry baseline.

### Build Steps
```bash
# Configure
cmake --preset default

# Build
cmake --build build --config Release
```

Output: `build/Release/LoadingScreenLocker.dll`

Use a fresh build directory after upgrading dependencies if an existing CMake cache refers to an older project location. A Debug build is also available with `cmake --build build --config Debug`.

Optional compatibility checks (no game installation required):

```bash
cmake --preset default -DLSL_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

These checks cover runtime selection and Address Library fixture loading for SE, AE, and VR. The 1.7.104 fixture is synthetic and verifies format 5 handling; it does not validate addresses against the game executable.

## License

GPL-3.0 License
