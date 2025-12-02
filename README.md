# EthoWild

A desktop application for labeling animal behaviors in video recordings. Built with C++ and Qt6, EthoWild provides a fast, intuitive interface for researchers studying wildlife behavior.

## Features

- **Video Playback** — Play, pause, seek, and adjust playback speed (0.5x–2.0x)
- **Directory Navigation** — Load a folder of videos and navigate between them
- **Zoom & Pan** — Ctrl+scroll to zoom, drag to pan for detailed observation
- **Behavior Labeling** — Record events (instant) and states (duration-based) with a double-click
- **Session Metadata** — Tag, role, sex, stage, group type, group size, and observations
- **CSV Export** — Export labeled records for analysis

## Documentation

Full documentation is available at **[blotero.github.io/etho-wild](https://blotero.github.io/etho-wild/)**

- [Installation](https://blotero.github.io/etho-wild/installation/) — Download binaries or build from source
- [Configuration](https://blotero.github.io/etho-wild/configuration/) — Customize behaviors and metadata options
- [Usage Guide](https://blotero.github.io/etho-wild/usage/) — Complete guide to using the application

## Quick Start

1. Download the latest release for your platform from [Releases](https://github.com/blotero/etho-wild/releases)
2. Extract the archive
3. Ensure `behaviors.json` is in the same directory as the executable
4. Run `EthoWild`

## Supported Platforms

| Platform | Architecture |
|----------|--------------|
| Windows | x64 |
| Linux | x64 |
| macOS | ARM64 (Apple Silicon) |
| macOS | x64 (Intel) |

## Building from Source

Requires CMake 3.20+, a C++20 compiler, Qt6, and OpenCV 4.x.

```bash
git clone https://github.com/blotero/etho-wild.git
cd etho-wild

cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build

cp behaviors.json build/
./build/EthoWild
```i

See the [Installation docs](https://blotero.github.io/etho-wild/installation/) for detailed instructions including vcpkg and system package options.

## License

MIT

