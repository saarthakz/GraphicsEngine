#include "engine.h"

#include <GL/gl.h>
#include <cmath>

// Static Color definitions
const Color Color::White = {255, 255, 255, 255};
const Color Color::Red = {255, 0, 0, 255};
const Color Color::Green = {0, 255, 0, 255};
const Color Color::Blue = {0, 0, 255, 255};
const Color Color::Black = {0, 0, 0, 255};

namespace platform {
  bool Init() { return glfwInit() == GLFW_TRUE; }
  void Shutdown() { glfwTerminate(); }
  void PollEvents() { glfwPollEvents(); }
  bool WindowShouldClose(GLFWwindow* w) { return glfwWindowShouldClose(w); }
  double Time() { return glfwGetTime(); }
  int GetKey(GLFWwindow* w, int key) { return glfwGetKey(w, key); }
  void SwapBuffers(GLFWwindow* w) { glfwSwapBuffers(w); }
} // namespace platform

Engine::Engine() {}

Engine::~Engine() {
  if (m_window)
    glfwDestroyWindow(m_window);
  platform::Shutdown();
}

bool Engine::Initialize(int width, int height, std::string appName) {
  m_nScreenWidth = width;
  m_nScreenHeight = height;
  m_sAppName = appName;

  if (!platform::Init())
    return false;

  m_window =
    glfwCreateWindow(m_nScreenWidth, m_nScreenHeight, m_sAppName.c_str(), nullptr, nullptr);

  if (!m_window)
    return false;

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  m_vFramebuffer.resize(m_nScreenWidth * m_nScreenHeight);
  Clear();

  return true;
}

void Engine::Run() {
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

void Engine::Clear(Color color) {
  for (auto& p : m_vFramebuffer)
    p = color;
}

void Engine::Draw(int x, int y, Color color) {
  if (x < 0 || y < 0 || x >= m_nScreenWidth || y >= m_nScreenHeight)
    return;
  m_vFramebuffer[y * m_nScreenWidth + x] = color;
}

void Engine::Present() {
  glDrawPixels(m_nScreenWidth, m_nScreenHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_vFramebuffer.data());
  platform::SwapBuffers(m_window);
}

void Engine::DrawLine(int x1, int y1, int x2, int y2, Color color) {
  int dx = std::abs(x2 - x1);
  int dy = std::abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  while (true) {
    Draw(x1, y1, color);
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

void Engine::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
  DrawLine(x1, y1, x2, y2, color);
  DrawLine(x2, y2, x3, y3, color);
  DrawLine(x3, y3, x1, y1, color);
}

void Engine::DrawTriangle(const Triangle& tri, Color color) {
  DrawTriangle((int)tri.points[0].x, (int)tri.points[0].y, (int)tri.points[1].x,
               (int)tri.points[1].y, (int)tri.points[2].x, (int)tri.points[2].y, color);
}

void Engine::DrawCircle(int xc, int yc, int radius, Color color) {
  int x = 0;
  int y = radius;
  int p = 3 - 2 * radius;

  if (radius == 0)
    return;

  while (y >= x) {
    Draw(xc - x, yc - y, color);
    Draw(xc - y, yc - x, color);
    Draw(xc + y, yc - x, color);
    Draw(xc + x, yc - y, color);
    Draw(xc - x, yc + y, color);
    Draw(xc - y, yc + x, color);
    Draw(xc + y, yc + x, color);
    Draw(xc + x, yc + y, color);

    if (p < 0)
      p += 4 * x++ + 6;
    else
      p += 4 * (x++ - y--) + 10;
  }
}

void Engine::UpdateInputState() {
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

Engine::sKeyState Engine::GetKey(int key) const { return m_keyStates[key]; }
