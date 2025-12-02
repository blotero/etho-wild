# Installation

## Download

Pre-built binaries are available for all major platforms. Download the latest release from the [GitHub Releases](https://github.com/blotero/etho-wild/releases) page.

| Platform | Architecture | Download |
|----------|--------------|----------|
| Windows | x64 | `EthoWild-windows-x64.zip` |
| Linux | x64 | `EthoWild-linux-x64.zip` |
| macOS | ARM64 (Apple Silicon) | `EthoWild-darwin-arm64.zip` |
| macOS | x64 (Intel) | `EthoWild-darwin-x64.zip` |

### Running the Application

1. Extract the downloaded archive
2. Ensure `behaviors.json` is in the same directory as the executable
3. Run `EthoWild` (or `EthoWild.exe` on Windows)

!!! note "Linux Users"
    You may need to make the binary executable: `chmod +x EthoWild`

---

## Building from Source

If you prefer to build EthoWild yourself, follow these instructions.

### Prerequisites

- **CMake** 3.20 or higher
- **C++20** compatible compiler (GCC 11+, Clang 14+, MSVC 2022)
- **Qt6** (Widgets, Core, Gui)
- **OpenCV 4.x**
- **Ninja** (recommended) or Make

### Using vcpkg (Recommended)

[vcpkg](https://github.com/microsoft/vcpkg) is the easiest way to manage dependencies.

=== "Linux / macOS"

    ```bash
    # Clone vcpkg (if you don't have it)
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg && ./bootstrap-vcpkg.sh
    export VCPKG_ROOT=$(pwd)
    
    # Clone EthoWild
    git clone https://github.com/blotero/etho-wild.git
    cd etho-wild
    
    # Install dependencies
    $VCPKG_ROOT/vcpkg install qtbase opencv4
    
    # Configure and build
    cmake -B build -S . \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
    
    cmake --build build
    ```

=== "Windows"

    ```powershell
    # Clone vcpkg (if you don't have it)
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    $env:VCPKG_ROOT = (Get-Location).Path
    
    # Clone EthoWild
    git clone https://github.com/blotero/etho-wild.git
    cd etho-wild
    
    # Install dependencies
    & "$env:VCPKG_ROOT\vcpkg.exe" install qtbase:x64-windows opencv4:x64-windows
    
    # Configure and build
    cmake -B build -S . `
        -G Ninja `
        -DCMAKE_BUILD_TYPE=Release `
        -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
    
    cmake --build build
    ```

### Using System Packages

If you prefer to use your system's package manager:

=== "Ubuntu / Debian"

    ```bash
    sudo apt-get update
    sudo apt-get install -y \
        build-essential \
        cmake \
        ninja-build \
        qt6-base-dev \
        libopencv-dev
    
    git clone https://github.com/blotero/etho-wild.git
    cd etho-wild
    
    cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    ```

=== "Arch Linux"

    ```bash
    sudo pacman -S base-devel cmake ninja qt6-base opencv
    
    git clone https://github.com/blotero/etho-wild.git
    cd etho-wild
    
    cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    ```

=== "macOS (Homebrew)"

    ```bash
    brew install cmake ninja qt@6 opencv
    
    git clone https://github.com/blotero/etho-wild.git
    cd etho-wild
    
    cmake -B build -S . -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)
    
    cmake --build build
    ```

### Build Output

After a successful build, the executable will be located at:

```
build/EthoWild        # Linux / macOS
build/EthoWild.exe    # Windows
```

Copy `behaviors.json` to the same directory before running:

```bash
cp behaviors.json build/
./build/EthoWild
```

