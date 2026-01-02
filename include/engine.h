#pragma once

#include "mesh.h"

#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>
#include <vector>

// Forward declaration for GLFW window
struct GLFWwindow;

// Color structure to handle RGB assets
struct Color {
  uint8_t r = 255;
  uint8_t g = 255;
  uint8_t b = 255;
  uint8_t a = 255;

  static const Color White;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Black;
};

// GLFW thin wrapper (platform abstraction)
namespace platform {
  bool Init();
  void Shutdown();
  void PollEvents();
  bool WindowShouldClose(GLFWwindow* w);
  double Time();
  int GetKey(GLFWwindow* w, int key);
  void SwapBuffers(GLFWwindow* w);
} // namespace platform

// Engine
class Engine {
  private:
  // Engine State
  GLFWwindow* m_window = nullptr;
  int m_nScreenWidth = 0;
  int m_nScreenHeight = 0;
  std::string m_sAppName;
  std::vector<Color> m_vFramebuffer;

  protected:
  // Input State
  struct sKeyState {
    bool bPressed = false;
    bool bHeld = false;
    bool bReleased = false;
  };
  sKeyState m_keyStates[512];
  void UpdateInputState();

  // Internal Rendering and Primitive Methods
  void Clear(Color color = Color::Black);
  void Draw(int x, int y, Color color = Color::White);
  void Present();

  void DrawLine(int x1, int y1, int x2, int y2, Color color = Color::White);
  void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color color = Color::White);
  void DrawTriangle(const Triangle& tri, Color color = Color::White);
  void DrawCircle(int xc, int yc, int radius, Color color = Color::White);

  // User Interface Hooks (Overridden by the derived class)
  virtual bool OnCreate() = 0;
  virtual bool OnUpdate(float deltaT) = 0;

  public:
  // Life Cycle and Initialization
  Engine();
  virtual ~Engine();

  bool Initialize(int width = 800, int height = 600, std::string appName = "Engine");
  void Run();

  // Public Accessors
  sKeyState GetKey(int key) const;
  int GetScreenWidth() const { return m_nScreenWidth; }
  int GetScreenHeight() const { return m_nScreenHeight; }
};
