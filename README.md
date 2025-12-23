# GraphicsEngine

A lightweight C++ graphics engine framework built on top of **OpenGL** and **GLFW**. This project provides a simple abstraction for creating graphical applications, allowing developers to focus on logic and rendering rather than boilerplate windowing and context setup.

## Features

- **Simplified API**: Easy-to-use `Engine` base class with `OnCreate` and `OnUpdate` callbacks.
- **Rendering Primitives**: Built-in support for clearing the screen and drawing basic shapes like triangles.
- **Cross-Platform Build System**: Managed with **CMake** for easy configuration across different environments.
- **Automated Scripts**: Includes helper scripts for configuration, building, and running.

## Prerequisites

Before setting up the project, ensure you have the following installed on your system:

- **C++ Compiler**: Supporting C++17 (e.g., GCC, Clang).
- **CMake**: Version 3.10 or higher.
- **OpenGL**: Development libraries.
- **GLFW3**: Windowing and input library.

### Installing Dependencies (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libgl1-mesa-dev libglfw3-dev
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

## Project Structure

- `include/`: Header files for the core engine.
- `src/`: Implementation of the engine logic.
- `main.cpp`: Entry point and example application logic.
- `scripts/`: Helper shells scripts for automation.
- `build/`: Target directory for compiled binaries (created during configuration).

## Getting Started

To create your own application, simply inherit from the `Engine` class and override the lifecycle methods:

```cpp
class MyGame : public Engine {
public:
    bool OnCreate() override {
        // Initialization code here
        return true;
    }

    bool OnUpdate(float deltaT) override {
        Clear(); // Clear screen
        // Rendering logic here
        return true;
    }
};

int main() {
    MyGame game;
    if (game.Initialize(800, 600)) {
        game.Run();
    }
    return 0;
}
```

## License
This project is open-source and available under the MIT License.
