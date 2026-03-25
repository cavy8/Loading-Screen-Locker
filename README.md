# Loading Screen Locker

An SKSE plugin for Skyrim SE/AE that prevents user-driven rotation and zoom of 3D models on loading screens.

## Features

- **Disables mouse/thumbstick input** from rotating/zooming loading screen 3D models
- **Preserves all other inputs** - buttons, keyboard, D-pad, triggers, mouse buttons, and scroll wheel still work normally
- **Compatible with loading screen overhaul mods** - only touches rotation/zoom axes. Tested with Loading Menu Overhaul
- **Configurable** via INI file

## Why?

This mod is intended to be used with 2D loading screen replacers, such as this one.

## Installation

1. Install [SKSE](https://skse.silverlock.org/) for your Skyrim version
2. Copy `LoadingScreenLocker.dll` to `Data/SKSE/Plugins/`
3. (Optional) Copy `LoadingScreenLocker.ini` to customize settings

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
