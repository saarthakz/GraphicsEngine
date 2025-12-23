#include "engine.h"

#include <iostream>

class ExampleApp : public Engine {
  public:
  ExampleApp() { m_sAppName = "Triangle Demo"; }

  protected:
  bool OnCreate() override {
    // Called once on start
    return true;
  }

  bool OnUpdate(float deltaT) override {
    // Clear the screen to black
    Clear();

    // Draw a white triangle in the middle of the screen
    // Using simple coordinates, assuming screen size is reasonably large
    // Vertex 1: Top (center)
    // Vertex 2: Bottom-left
    // Vertex 3: Bottom-right
    int x1 = m_nScreenWidth / 2;
    int y1 = m_nScreenHeight / 4;

    int x2 = m_nScreenWidth / 4;
    int y2 = 3 * m_nScreenHeight / 4;

    int x3 = 3 * m_nScreenWidth / 4;
    int y3 = 3 * m_nScreenHeight / 4;

    DrawTriangle(x1, y1, x2, y2, x3, y3, 255, 255, 255);

    return true;
  }
};

int main() {
  ExampleApp app;
  if (app.Initialize(800, 600)) {
    app.Run();
  } else {
    std::cerr << "Failed to initialize engine" << std::endl;
  }
  return 0;
}
