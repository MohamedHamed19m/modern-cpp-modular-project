# Modern CMake Mastery Project

> A reference project for Modern CMake (3.20+) best practices. Demonstrates multi-level subdirectories, exported targets, package configuration (`find_package` support), and automated dependency management using `FetchContent`.

## Overview

This project serves as a learning template for professional CMake usage. It demonstrates the "Modern CMake" approach (CMake 3.15+) where everything is a **target** with properties, rather than using global commands.

## Key Concepts Demonstrated

| Concept | Modern CMake Approach | Anti-Pattern (Avoid) |
|---------|----------------------|---------------------|
| Include directories | `target_include_directories()` | `include_directories()` |
| Linking libraries | `target_link_libraries()` | `link_libraries()` |
| Compile definitions | `target_compile_definitions()` | `add_definitions()` |
| C++ standard | `target_compile_features()` | `CMAKE_CXX_FLAGS` |
| Dependencies | Imported targets (`MyLib::mylib`) | Global variables |
| External deps | `FetchContent`/`find_package()` | Manual include paths |
| Exporting | `install(EXPORT ...)` | Manual `.cmake` files |

### Visibility Keywords

- **PUBLIC**: Both the target AND its consumers need this
- **PRIVATE**: Only the target needs this (implementation detail)
- **INTERFACE**: Only consumers need this (header-only libraries)

## Project Structure

```
modern-cpp-modular-project/
├── CMakeLists.txt           # Top-level project configuration
├── cmake/                   # Custom CMake modules
│   └── MathEngineConfig.cmake.in
├── libs/                    # Sub-packages (internal libraries)
│   ├── logger/              # Header-only INTERFACE library
│   │   ├── CMakeLists.txt
│   │   └── include/logger/
│   └── math_engine/         # Compiled STATIC library
│       ├── CMakeLists.txt
│       ├── include/math/
│       └── src/
├── apps/                    # Executables
│   └── main_app/
│       ├── CMakeLists.txt
│       └── main.cpp
├── tests/                   # Unit tests
│   ├── CMakeLists.txt
│   └── test_math.cpp
└── examples/                # Consumer examples
    └── consumer_app/
        ├── CMakeLists.txt
        └── main.cpp
```

## Building

### Prerequisites

- CMake 3.20 or later
- C++20 compatible compiler:
  - GCC 11+
  - Clang 13+
  - MSVC 2022 (v19.30+)

### Quick Start

```bash
# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run tests
cd build
ctest --output-on-failure

# Run the main application
./apps/main_app/main_app
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTING` | ON | Build the test suite |
| `BUILD_EXAMPLES` | ON | Build example applications |
| `ENABLE_INSTALL` | ON | Enable install targets |

```bash
cmake -B build -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON
```

## Testing `find_package()` Support

After building, install the library and test the consumer app:

```bash
# Install to a temporary location
cmake --install build --prefix /tmp/mathengine-install

# Build the consumer app against the installed package
cmake -B build/consumer -S examples/consumer_app \
    -DCMAKE_PREFIX_PATH=/tmp/mathengine-install
cmake --build build/consumer
./build/consumer/consumer_app
```

## Learning Guide

### 1. Target-Based Design

All CMake operations are done on **targets** (created by `add_library()` or `add_executable()`):

```cmake
# Create a library target
add_library(mylib STATIC src.cpp)

# Set properties on that target
target_include_directories(mylib PUBLIC include/)
target_link_libraries(mylib PUBLIC dependency)
target_compile_features(mylib PUBLIC cxx_std_20)
```

### 2. Transitive Dependencies

When `math_engine` links to `logger` with **PUBLIC** visibility, consumers of `math_engine` automatically get `logger`:

```cmake
# In math_engine's CMakeLists.txt
target_link_libraries(math_engine PUBLIC MathEngine::logger)

# In main_app - we don't need to mention logger!
target_link_libraries(main_app PRIVATE MathEngine::math_engine)
```

### 3. Generator Expressions

Generator expressions enable different behavior during build vs install:

```cmake
target_include_directories(mylib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
```

### 4. External Dependencies

Use `FetchContent` for automatic dependency downloading:

```cmake
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
```

## Installing OpenSSL (Example)

CMake 3.15+ includes built-in support for OpenSSL:

```cmake
# In your CMakeLists.txt
find_package(OpenSSL REQUIRED)

target_link_libraries(myapp PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

For libraries without find modules, use `FetchContent`:

```cmake
FetchContent_Declare(
    some_lib
    GIT_REPOSITORY https://github.com/user/some_lib.git
    GIT_TAG v1.0.0
)
FetchContent_MakeAvailable(some_lib)

target_link_libraries(myapp PRIVATE some_lib::some_lib)
```

## Further Reading

- [Modern CMake by Example](https://cliutils.gitlab.io/modern-cmake/)
- [CMake 3.20+ Documentation](https://cmake.org/cmake/help/v3.20/)
- [Effective Modern CMake](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1)

## License

This is a learning project. Feel free to use as a template for your own projects.
