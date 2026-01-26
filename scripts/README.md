# VneEvents Scripts

This directory contains utility scripts for building, testing, and maintaining the VneEvents library.

## Build Scripts

### Linux Build (`build_linux.sh`)

Builds VneEvents on Linux using GCC or Clang.

```bash
# Basic build
./scripts/build_linux.sh -t Debug -a configure_and_build

# Select compiler and parallel jobs
./scripts/build_linux.sh -c clang -j 20 -t Release

# Interactive mode
./scripts/build_linux.sh -interactive

# Clean build
./scripts/build_linux.sh -clean -t Debug
```

**Options:**
- `-t, --build-type <type>` - Build type: Debug, Release, RelWithDebInfo, MinSizeRel
- `-c, --compiler <compiler>` - Compiler: gcc or clang
- `-a, --action <action>` - Action: configure, build, configure_and_build, test
- `-j, --jobs <N>` - Number of parallel jobs
- `-clean` - Clean build directory before building
- `-interactive` - Interactive mode for configuration
- `-h, --help` - Show help message

### macOS Build (`build_macos.sh`)

Builds VneEvents on macOS using Clang or Xcode.

```bash
# Basic build
./scripts/build_macos.sh -t Debug -a configure_and_build

# Generate Xcode project
./scripts/build_macos.sh -xcode -t Release

# Interactive mode
./scripts/build_macos.sh -interactive
```

**Options:**
- `-t, --build-type <type>` - Build type
- `-xcode` - Generate Xcode project instead of building
- `-a, --action <action>` - Action: configure, build, configure_and_build, test
- `-j, --jobs <N>` - Number of parallel jobs
- `-clean` - Clean build directory
- `-interactive` - Interactive mode
- `-h, --help` - Show help

### Windows Build

#### Python Script (`build_windows.py`) - Recommended

Native Windows build script with better error handling.

```bash
# Basic build
python scripts/build_windows.py -t Debug -a configure_and_build

# Select Visual Studio version
python scripts/build_windows.py --vs-version 2022 -t Release

# Interactive mode
python scripts/build_windows.py --interactive
```

**Options:**
- `-t, --build-type <type>` - Build type
- `--vs-version <version>` - Visual Studio version (2019, 2022)
- `-a, --action <action>` - Action: configure, build, configure_and_build, test
- `-j, --jobs <N>` - Number of parallel jobs
- `--clean` - Clean build directory
- `--interactive` - Interactive mode
- `-h, --help` - Show help

#### Bash Script (`build_windows.sh`)

For Git Bash or WSL users.

```bash
./scripts/build_windows.sh -t Debug -a configure_and_build
./scripts/build_windows.sh -j 8 -t Release
```

### Web Build (`build_web.sh`)

Builds VneEvents for Web using Emscripten.

```bash
# Basic build
./scripts/build_web.sh Debug

# Force rebuild with parallel jobs
./scripts/build_web.sh -f -j4 Release
```

**Options:**
- `<build_type>` - Build type (Debug, Release, etc.)
- `-f, --force` - Force rebuild
- `-j, --jobs <N>` - Number of parallel jobs
- `-h, --help` - Show help

### iOS Build (`build_ios.sh`)

Builds VneEvents for iOS (Simulator or Device).

```bash
# Basic build
./scripts/build_ios.sh -t Debug -a configure_and_build

# Build for device
./scripts/build_ios.sh -device -deployment-target 15.0

# Generate Xcode project only
./scripts/build_ios.sh -xcode-only -t Release
```

**Options:**
- `-t, --build-type <type>` - Build type
- `-device` - Build for device (default: simulator)
- `-deployment-target <version>` - iOS deployment target version
- `-xcode-only` - Generate Xcode project without building
- `-a, --action <action>` - Action: configure, build, configure_and_build, test
- `-j, --jobs <N>` - Number of parallel jobs
- `-clean` - Clean build directory
- `-h, --help` - Show help

## Prerequisites

### Build Tools
- CMake 3.16 or higher
- C++17 compatible compiler (GCC 8+, Clang 7+, MSVC 2019+)

### Platform-Specific Requirements

#### Linux
- GCC or Clang
- Make or Ninja

#### macOS
- Xcode Command Line Tools
- Clang (included with Xcode)

#### Windows
- Visual Studio 2019 or 2022
- CMake

#### iOS
- Xcode
- iOS SDK

#### Web
- Emscripten SDK
  ```bash
  git clone https://github.com/emscripten-core/emsdk.git ~/emsdk
  cd ~/emsdk && ./emsdk install latest && ./emsdk activate latest
  ```

## Common Workflows

### Building and Testing
```bash
# Build Debug version with tests
./scripts/build_linux.sh -t Debug -a configure_and_build

# Run tests
cd build/Debug/build-linux-gcc-12.0.0
ctest --output-on-failure
```

### Cross-Platform Development
```bash
# macOS
./scripts/build_macos.sh -t Debug -a configure_and_build

# Linux
./scripts/build_linux.sh -t Debug -a configure_and_build

# Windows (from Developer Command Prompt)
python scripts/build_windows.py -t Debug -a configure_and_build
```

## Build Directory Structure

Builds are organized by platform, compiler, and version:

```
build/
├── Debug/
│   ├── build-linux-gcc-12.0.0/
│   ├── build-macos-clang-17.0.0/
│   ├── build-windows-cl-2022/
│   ├── xcode-macos-clang-17.0.0/
│   └── build-web-emscripten-3.1.0/
└── Release/
    └── ...
```

## Notes

- All scripts support interactive mode with `-interactive` flag
- Build directories are automatically created based on platform, compiler, and version
- Tests are enabled by default (`-DBUILD_TESTS=ON`)
- Xcode projects are generated in separate directories for easier IDE integration
