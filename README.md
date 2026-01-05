# GraphicsEngine

A lightweight C++ graphics engine framework built on top of **OpenGL** and **GLFW**. This project provides a simple abstraction for creating graphical applications, allowing developers to focus on logic and rendering rather than boilerplate windowing and context setup.

## Features

- **Simplified API**: Easy-to-use `Engine` base class with `OnCreate` and `OnUpdate` callbacks.
- **Rendering Primitives**: Built-in support for software-based rasterization (triangles, lines, circles).
- **3D Model Loading**: Support for loading and rendering `.obj` files.
- **Interactive Camera**: Real-time 3D camera system with movement controls.
- **Scene Graph**: Transform-based object management with parent-child relationships.
- **Cross-Platform Build System**: Managed with **CMake** for easy configuration across different environments.
- **Automated Scripts**: Includes helper scripts for configuration, building, and running.

## Prerequisites

Before setting up the project, ensure you have the following installed on your system:

- **C++ Compiler**: Supporting C++17 (e.g., GCC, Clang).
- **CMake**: Version 3.10 or higher.
- **OpenGL**: Development libraries.
- **GLFW3**: Windowing and input library.

### Installing Dependencies

We provide a script to handle dependency installation for macOS and Ubuntu/Debian:

```bash
./scripts/setup.sh
```

## Setup and Building

The project provides several scripts in the `scripts/` directory to streamline the build process.

### 1. Configure the Project
Generate the build files and compile commands:
```bash
./scripts/configure.sh
```

### 2. Build the Project
Compile the source code:
```bash
./scripts/build.sh
```

### 3. Run the Application
Build (if necessary) and execute the engine:
```bash
./scripts/run.sh
```

### 4. Clean the Project
Remove build artifacts:
```bash
./scripts/clean.sh
```

## Project Structure

- `include/`: Header files for the core engine.
- `src/`: Implementation of the engine logic.
- `resources/`: 3D models and assets (e.g., `.obj` files).
- `main.cpp`: Entry point and example application logic.
- `scripts/`: Helper shells scripts for automation.
- `build/`: Target directory for compiled binaries (created during configuration).

## Camera Controls

The engine includes a full first-person camera system with the following controls:

- **Movement**: 
  - `W` / `S`: Move Forward / Backward
  - `A` / `D`: Strafe Left / Right
  - `Q` / `Space`: Move Up (Ascend)
  - `E` / `Left Ctrl`: Move Down (Descend)
- **Rotation**:
  - `Left` / `Right` Arrow: Pan (Yaw)
  - `Up` / `Down` Arrow: Tilt (Pitch)

## Getting Started

To create a 3D application, use the `ThreeEngine` class. You can specify which 3D model to load directly in the entry point:

```cpp
#include "three.h"
#include <iostream>

int main() {
    ThreeEngine app;
    
    // Initialize window with Width, Height, and Title
    if (app.Initialize(800, 600, "ThreeEngine")) {
        // Load an .obj asset from the resources directory
        app.LoadMeshAsset("resources/axes.obj");
        
        // Start the engine loop
        app.Run();
    } else {
        std::cerr << "Failed to initialize engine" << std::endl;
    }
    return 0;
}
```

### Adding New Assets
Place your `.obj` files in the `resources/` directory and load them using `app.LoadMeshAsset("resources/your_model.obj")`.

## License
This project is open-source and available under the MIT License.
