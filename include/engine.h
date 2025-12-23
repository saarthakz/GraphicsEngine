#pragma once

//  Cross-platform GLFW-based Game Engine (Header Only)
//  Adapted from OneLoneCoder Console Game Engine
//
//  - Audio removed
//  - Windows console removed
//  - GLFW used only for window + input + timing
//  - Software rendering preserved
//  - Original drawing algorithms preserved

#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

// GLFW thin wrapper (platform abstraction)

namespace platform {
  inline bool Init() { return glfwInit() == GLFW_TRUE; }

  inline void Shutdown() { glfwTerminate(); }

  inline void PollEvents() { glfwPollEvents(); }

  inline bool WindowShouldClose(GLFWwindow *w) { return glfwWindowShouldClose(w); }

  inline double Time() {
    // Seconds, monotonic
    return glfwGetTime();
  }

  inline int GetKey(GLFWwindow *w, int key) { return glfwGetKey(w, key); }

  inline void SwapBuffers(GLFWwindow *w) { glfwSwapBuffers(w); }
} // namespace platform

// Engine

class Engine {
  public:
  // Construction / Destruction

  Engine() { m_sAppName = "Engine"; }

  virtual ~Engine() {
    if (m_window)
      glfwDestroyWindow(m_window);

    platform::Shutdown();
  }

  // Engine Setup

  bool Initialize(int width, int height) {
    m_nScreenWidth = width;
    m_nScreenHeight = height;

    if (!platform::Init())
      return false;

    m_window =
      glfwCreateWindow(m_nScreenWidth, m_nScreenHeight, m_sAppName.c_str(), nullptr, nullptr);

    if (!m_window)
      return false;

    glfwMakeContextCurrent(m_window);

    // Optional: VSync (does NOT replace dt logic)
    glfwSwapInterval(1);

    m_vFramebuffer.resize(m_nScreenWidth * m_nScreenHeight);
    Clear();

    return true;
  }

  // Engine Start

  void Run() {
    /*
    Threading Model:
    - Single-threaded engine loop
    - Input polling, simulation update, and rendering
    - GLFW internal OS threads are separate
    */

    if (!OnCreate())
      return;

    double lastTime = platform::Time();

    while (!platform::WindowShouldClose(m_window)) {
      double now = platform::Time();
      float deltaT = static_cast<float>(now - lastTime);
      lastTime = now;

      platform::PollEvents();
      UpdateInputState();

      if (!OnUpdate(deltaT))
        break;

      Present();
    }
  }

  protected:
  // Rendering (Software Framebuffer)

  struct Pixel {
    uint8_t r, g, b, a;
  };

  void Clear() {
    for (auto &p : m_vFramebuffer)
      p = {0, 0, 0, 255};
  }

  void Draw(int x, int y, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) {
    if (x < 0 || y < 0 || x >= m_nScreenWidth || y >= m_nScreenHeight)
      return;

    m_vFramebuffer[y * m_nScreenWidth + x] = {r, g, b, 255};
  }

  void Present() {
    glDrawPixels(m_nScreenWidth, m_nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_vFramebuffer.data());

    platform::SwapBuffers(m_window);
  }

  // Drawing Primitives (Copied from Original Engine)

  void DrawLine(int x1, int y1, int x2, int y2, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
      Draw(x1, y1, r, g, b);
      if (x1 == x2 && y1 == y2)
        break;
      int e2 = err << 1;
      if (e2 > -dy) {
        err -= dy;
        x1 += sx;
      }
      if (e2 < dx) {
        err += dx;
        y1 += sy;
      }
    }
  }

  void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint8_t r = 255,
                    uint8_t g = 255, uint8_t b = 255) {
    DrawLine(x1, y1, x2, y2, r, g, b);
    DrawLine(x2, y2, x3, y3, r, g, b);
    DrawLine(x3, y3, x1, y1, r, g, b);
  }

  void DrawCircle(int xc, int yc, int radius, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) {
    int x = 0;
    int y = radius;
    int p = 3 - 2 * radius;

    if (radius == 0)
      return;

    while (y >= x) {
      Draw(xc - x, yc - y, r, g, b);
      Draw(xc - y, yc - x, r, g, b);
      Draw(xc + y, yc - x, r, g, b);
      Draw(xc + x, yc - y, r, g, b);
      Draw(xc - x, yc + y, r, g, b);
      Draw(xc - y, yc + x, r, g, b);
      Draw(xc + y, yc + x, r, g, b);
      Draw(xc + x, yc + y, r, g, b);

      if (p < 0)
        p += 4 * x++ + 6;
      else
        p += 4 * (x++ - y--) + 10;
    }
  }

  // Input Handling (Pressed / Held / Released)

  struct sKeyState {
    bool bPressed = false;
    bool bHeld = false;
    bool bReleased = false;
  };

  sKeyState m_keyStates[512];

  void UpdateInputState() {
    for (int k = 0; k < 512; ++k) {
      bool isDown = platform::GetKey(m_window, k) == GLFW_PRESS;

      m_keyStates[k].bPressed = false;
      m_keyStates[k].bReleased = false;

      if (isDown && !m_keyStates[k].bHeld) {
        m_keyStates[k].bPressed = true;
        m_keyStates[k].bHeld = true;
      } else if (!isDown && m_keyStates[k].bHeld) {
        m_keyStates[k].bReleased = true;
        m_keyStates[k].bHeld = false;
      }
    }
  }

  public:
  sKeyState GetKey(int key) const { return m_keyStates[key]; }

  protected:
  // User Interface (unchanged philosophy)

  virtual bool OnCreate() = 0;

  /*
  deltaT:
  - Time in seconds since previous frame
  - Used for FPS-independent motion
  */
  virtual bool OnUpdate(float deltaT) = 0;

  // Engine State

  GLFWwindow *m_window = nullptr;

  int m_nScreenWidth = 0;
  int m_nScreenHeight = 0;

  std::string m_sAppName;

  std::vector<Pixel> m_vFramebuffer;
};
