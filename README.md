# OpenGL Basic C++ Apps

[![Release on Linux](https://github.com/Andrew2a1/opengl_template/actions/workflows/release_linux.yml/badge.svg)](https://github.com/Andrew2a1/opengl_template/actions/workflows/release_linux.yml)

This is basic template for multiplatform C++ OpenGL Application.

## Dependencies

This project uses CMake for compilation.

This template uses `freeglut`, `glew` and `glm` as it's dependencies.
Dependencies are managed by Conan package manager.

1. Download and install [CMake](https://cmake.org/download/) (3.22 minimum version)
2. Install Conan

```bash
python3 -m pip install --upgrade conan
```

3. Install system dependencies (Linux)

On linux installation of additional system dependencies might be required.
Conan will raise an error if some system package is missing.

## Setup

This project uses CMakePresets.

To build application on Windows use:

```bash
cmake --preset vs_release
cmake --build --preset vs_release
```

To build application on Linux use:

```bash
cmake --preset gcc_release
cmake --build --preset gcc_release
```

