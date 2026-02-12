# Modern CMake Learning Plan

> A structured guide to master Modern CMake (3.20+) through hands-on practice

## Overview

This learning plan takes you from basic CMake knowledge to advanced package management. Each section includes:
- **Concept**: What you're learning
- **Why it matters**: The problem it solves
- **Practice**: Hands-on exercises in this project
- **Verification**: How to confirm you understand it

---

## Phase 1: Target-Based Thinking (Foundations)

### 1.1 Understanding Targets

**Concept**: In Modern CMake, everything is a **target** - created by `add_library()` or `add_executable()`. Targets have properties that describe how they're built and what they depend on.

**Old Way (Avoid)**:
```cmake
include_directories(include)
link_libraries(mylib)
add_definitions(-DSOMETHING)
```

**Modern Way**:
```cmake
add_library(mylib src.cpp)
target_include_directories(mylib PUBLIC include/)
target_link_libraries(mylib PUBLIC dependency)
target_compile_definitions(mylib PUBLIC SOMETHING)
```

**Practice**: Open `libs/math_engine/CMakeLists.txt` and find all `target_*` commands.

---

### 1.2 Alias Targets

**Concept**: Create namespaced targets for consistency: `MyProject::mylib`

**Why**: Avoids naming conflicts and clearly indicates project ownership

**Example**:
```cmake
add_library(logger INTERFACE)
add_library(MathEngine::logger ALIAS logger)
```

**Practice**:
1. Search for `add_library` in all CMakeLists.txt files
2. Notice how `main_app` links to `MathEngine::math_engine` (namespaced)
3. Try changing `main_app` to link to just `math_engine` (still works, but less clear)

---

## Phase 2: Visibility Keywords (PUBLIC, PRIVATE, INTERFACE)

This is the **most important** Modern CMake concept.

### 2.1 PUBLIC - "I need it, and my users need it"

**Concept**: Both the target AND its consumers require this dependency.

**Use case**: When your public headers include the dependency's headers.

```cmake
# In math_engine's CMakeLists.txt
target_link_libraries(math_engine PUBLIC MathEngine::logger)
```

**Why**: `calculator.hpp` includes `logger/logger.hpp`, so anyone using `math_engine` also needs `logger`.

**Practice**:
1. Open `libs/math_engine/include/math/calculator.hpp` - see it includes logger
2. Open `libs/math_engine/CMakeLists.txt` line 55-58
3. Open `apps/main_app/CMakeLists.txt` - notice it only links to `math_engine`
4. Build the project - it works! Logger comes automatically via PUBLIC.

---

### 2.2 PRIVATE - "Only I need it"

**Concept**: Only the target needs this; consumers don't know about it.

**Use case**: When a dependency is only used in `.cpp` files, not in headers.

```cmake
target_link_libraries(mylib PRIVATE json_parser)
```

**Practice Exercise**:
1. Edit `libs/math_engine/CMakeLists.txt`
2. Change `PUBLIC` to `PRIVATE` for the logger dependency
3. Rebuild - you'll get errors in `main_app` because it can't find logger headers
4. Change back to `PUBLIC` to fix it

---

### 2.3 INTERFACE - "Only my users need it"

**Concept**: The target itself doesn't use it, but consumers do.

**Use case**: Header-only libraries where the target has no compiled files.

```cmake
# logger is INTERFACE - no compilation needed
add_library(logger INTERFACE)
target_include_directories(logger INTERFACE include/)
```

**Practice**:
1. Open `libs/logger/CMakeLists.txt`
2. Notice it's an INTERFACE library (no source files)
3. Notice everything uses `INTERFACE` visibility

---

## Phase 3: Generator Expressions

### 3.1 Build vs Install Interface

**Concept**: Different behavior when building vs when installed.

**The Problem**:
- During build: headers are in `libs/logger/include/`
- After install: headers are in `/usr/local/include/` (or install prefix)

**The Solution**:
```cmake
target_include_directories(logger INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
```

**Practice**:
1. Find this pattern in `libs/logger/CMakeLists.txt` lines 23-28
2. Notice `$<BUILD_INTERFACE:...>` and `$<INSTALL_INTERFACE:...>`
3. Build the project - uses BUILD_INTERFACE
4. Install the project - creates install for consumers

---

## Phase 4: External Dependencies

### 4.1 Using Find Package (System Dependencies)

**Concept**: Find libraries already installed on the system.

**Example - OpenSSL**:
```cmake
find_package(OpenSSL REQUIRED)
target_link_libraries(myapp PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

**Practice**:
1. Create a test file: `test_opengl/CMakeLists.txt`
2. Add: `find_package(OpenGL REQUIRED)`
3. Add: `target_link_libraries(testgl PRIVATE OpenGL::GL)`
4. Try finding other packages: `ZLIB`, `CURL`, `Threads`

**Common Find Package Targets**:
| Package | Imported Target |
|---------|-----------------|
| OpenSSL | `OpenSSL::SSL`, `OpenSSL::Crypto` |
| OpenGL | `OpenGL::GL` |
| ZLIB | `ZLIB::ZLIB` |
| Threads | `Threads::Threads` |
| CURL | `CURL::libcurl` |

---

### 4.2 Using FetchContent (Automatic Download)

**Concept**: Download and configure dependencies automatically during CMake configure.

**Why**: No need for users to manually install dependencies

**Example - Catch2** (already in this project):
```cmake
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.0
)
FetchContent_MakeAvailable(Catch2)
```

**Practice**:
1. Open `tests/CMakeLists.txt` - see Catch2 via FetchContent
2. Open `CMakeLists.txt` (root) - see fmt via FetchContent
3. Try adding a new FetchContent dependency (e.g., spdlog)

---

### 4.3 FetchContent vs Find Package - When to Use What

| Scenario | Use |
|----------|-----|
| Large system library (OpenSSL, OpenGL) | `find_package()` |
| Header-only library | `FetchContent` |
| Small dependency without find module | `FetchContent` |
| User might have custom version | `find_package()` with `CONFIG` mode |

---

## Phase 5: Making Your Library Findable

### 5.1 Installing Targets

**Concept**: Export your library so others can use it.

```cmake
install(TARGETS math_engine
    EXPORT MathEngineTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
```

**Practice**:
1. Look at `libs/math_engine/CMakeLists.txt` lines 97-105
2. Notice `EXPORT MathEngineTargets` - creates the export set
3. Run: `cmake --install build --prefix /tmp/test-install`

---

### 5.2 Creating Package Config Files

**Concept**: Generate `MyProjectConfig.cmake` for `find_package()` support.

**Files Created**:
- `MathEngineConfig.cmake` - Main config (finds dependencies, includes targets)
- `MathEngineConfigVersion.cmake` - Version compatibility check
- `MathEngineTargets.cmake` - Imported targets

**Practice**:
1. Look at root `CMakeLists.txt` lines 71-95
2. See `write_basic_package_version_file()` - creates version file
3. See `configure_package_config_file()` - creates main config
4. After installing, check the install directory for `.cmake` files

---

### 5.3 Testing Your Package

**Practice**:
1. Build and install MathEngine:
   ```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   cmake --install build --prefix C:/temp/mathengine
   ```

2. Build consumer app against installed package:
   ```bash
   cmake -B build/consumer -S examples/consumer_app \
       -DCMAKE_PREFIX_PATH=C:/temp/mathengine
   cmake --build build/consumer
   ```

3. Run it: `./build/consumer/consumer_app.exe`

---

## Phase 6: Advanced Patterns

### 6.1 Interface Libraries for Common Settings

**Pattern**: Create an INTERFACE library for common compiler flags.

```cmake
add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)

# Then use it:
target_link_libraries(myapp PRIVATE project_warnings)
```

---

### 6.2 Object Libraries (Partial Compilation)

**Concept**: Compile source but don't create a library - allows reuse.

```cmake
add_library(common_objs OBJECT common.cpp)
add_executable(app1 app1.cpp $<TARGET_OBJECTS:common_objs>)
add_executable(app2 app2.cpp $<TARGET_OBJECTS:common_objs>)
```

---

### 6.3 Component-Based Installation

**Pattern**: Allow users to select components.

```cmake
# In your config file
include(CMakeFindDependencyMacro)
find_dependency(Threads)

# User can do:
find_package(MyProject REQUIRED COMPONENTS Core Network)
```

---

## Practice Exercises

### Exercise 1: Add a New Sub-Package

**Task**: Create a `string_utils` library

1. Create `libs/string_utils/CMakeLists.txt`
2. Create INTERFACE library with string utility functions
3. Update root CMakeLists.txt to add subdirectory
4. Make `math_engine` depend on `string_utils` (PRIVATE)

---

### Exercise 2: Convert to Shared Library

**Task**: Convert `math_engine` from STATIC to SHARED

1. Edit `libs/math_engine/CMakeLists.txt`
2. Change `STATIC` to `SHARED`
3. Rebuild and notice `.dll`/`.so` created
4. Run main_app - it should still work

---

### Exercise 3: Add a New FetchContent Dependency

**Task**: Add [spdlog](https://github.com/gabime/spdlog) as a dependency

1. Add FetchContent for spdlog in root CMakeLists.txt
2. Link main_app to spdlog
3. Use spdlog in main.cpp instead of our logger

---

### Exercise 4: Create Package with Components

**Task**: Make MathEngine support components

1. Modify config to support: `find_package(MathEngine REQUIRED COMPONENTS Logger Math)`
2. Allow user to install only what they need

---

## Verification Checklist

Use this checklist to verify your understanding:

- [ ] Can explain PUBLIC vs PRIVATE vs INTERFACE
- [ ] Can create an ALIAS target
- [ ] Understands generator expressions
- [ ] Can use FetchContent to add a dependency
- [ ] Can use find_package for system libraries
- [ ] Can install and export a library
- [ ] Can use find_package() with their own library
- [ ] Understands transitive dependencies
- [ ] Can create INTERFACE libraries
- [ ] Knows when to use STATIC vs SHARED

---

## Learning Resources

### Essential Reading
1. [Modern CMake by Example](https://cliutils.gitlab.io/modern-cmake/)
2. [Effective Modern CMake](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1)
3. [CMake Tutorial (Official)](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

### Video Resources
1. "CMake Tutorial for Beginners" - by likecs
2. "Effective CMake" - Daniele Di Sarli
3. C++Now / CppCon talks on CMake

### Reference
- [CMake 3.20 Documentation](https://cmake.org/cmake/help/v3.20/)
- [CMake Commands Reference](https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html)

---

## Common Pitfalls to Avoid

### Pitfall 1: Using Global Commands
```cmake
# BAD
include_directories(include)
link_libraries(mylib)

# GOOD
target_include_directories(myapp PRIVATE include)
target_link_libraries(myapp PRIVATE mylib)
```

### Pitfall 2: Not Using Generator Expressions
```cmake
# BAD - only works during build
target_include_directories(mylib PUBLIC include/)

# GOOD - works for build AND install
target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
```

### Pitfall 3: Mixing Visibility
```cmake
# Confusing - why PUBLIC compile options but PRIVATE link?
target_compile_options(mylib PUBLIC -Wall)
target_link_libraries(mylib PRIVATE dependency)

# Be consistent and document why
```

---

## Quick Reference Card

### Common Commands

| Task | Command |
|------|---------|
| Create library | `add_library(name STATIC/SHARED/INTERFACE sources)` |
| Create executable | `add_executable(name sources)` |
| Link libraries | `target_link_libraries(target visibility dep)` |
| Include dirs | `target_include_directories(target visibility dirs)` |
| Compile defs | `target_compile_definitions(target visibility DEFS)` |
| Compile options | `target_compile_options(target visibility opts)` |
| Alias target | `add_library(Namespace::name ALIAS name)` |
| Find package | `find_package(Name REQUIRED)` |
| Fetch content | `FetchContent_MakeAvailable(name)` |

### Visibility Quick Reference

| Keyword | Target Needs | Consumers Need |
|---------|--------------|----------------|
| PUBLIC | Yes | Yes |
| PRIVATE | Yes | No |
| INTERFACE | No | Yes |

---

## Next Steps

1. **Practice**: Create your own library using these patterns
2. **Contribute**: Look at open-source CMakeLists.txt files
3. **Teach**: Explain these concepts to someone else
4. **Iterate**: Improve existing CMake code with modern patterns

Remember: **Everything is a target with properties.** This is the core of Modern CMake.
