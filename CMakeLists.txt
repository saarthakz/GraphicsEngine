cmake_minimum_required(VERSION 3.10)
project(GraphicsEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# Find dependencies
find_package(OpenGL REQUIRED)

# Try to find GLFW3
find_package(glfw3 CONFIG QUIET)
if(glfw3_FOUND)
    set(GLFW_LIBRARIES glfw)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    set(GLFW_LIBRARIES ${GLFW_LIBRARIES})
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
set(ALL_SOURCES main.cpp ${SRC_FILES})

# Executable
add_executable(${PROJECT_NAME} ${ALL_SOURCES})

# Target Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL ${GLFW_LIBRARIES})
