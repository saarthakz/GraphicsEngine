#include "engine.h"
#include "matrix.h"
#include "mesh.h"
#include "object.h"

#include <cassert>
#include <iostream>
#include <vector>

class ThreeEngine : public Engine {
  private:
  Mesh m_objAsset;
  std::vector<Object> m_sceneObjects;
  Matrix m_projectionMatrix{4, 4};

  // Helper method to get the updated camera position
  VecThree UpdateCamera(float deltaT) {
    VecThree camPos = GetCameraPosition();
    float fSpeed = 5.0f * deltaT;

    if (GetKey(GLFW_KEY_W).bHeld)
      camPos.z += fSpeed;
    if (GetKey(GLFW_KEY_S).bHeld)
      camPos.z -= fSpeed;
    if (GetKey(GLFW_KEY_A).bHeld)
      camPos.x -= fSpeed;
    if (GetKey(GLFW_KEY_D).bHeld)
      camPos.x += fSpeed;
    if (GetKey(GLFW_KEY_UP).bHeld)
      camPos.y += fSpeed;
    if (GetKey(GLFW_KEY_DOWN).bHeld)
      camPos.y -= fSpeed;

    SetCameraPosition(camPos);
    return camPos;
  }

  public:
  ThreeEngine() = default;

  protected:
  bool OnCreate() override {
    // SetLightDirection({0.5f, 1.0f, -0.5f}); // Light from top-right-front

    // 2. Load Assets
    m_objAsset.LoadFromObjectFile("resources/axes.obj");

    // Enable Light Source for the shiny new ship
    SetLightDirection({0.2f, 1.0f, -0.5f});

    // 2. Setup Scene
    m_sceneObjects.reserve(10);

    // Object 0: The Obj Asset
    Object obj;
    obj.SetMesh(&m_objAsset);
    obj.SetPosition({0.0f, 0.0f, 10.0f});
    m_sceneObjects.push_back(obj);

    // 3. Setup Projection
    float fAspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
    m_projectionMatrix = Matrix::MakeProjection(90.0f, fAspectRatio, 0.1f, 100.0f);

    return true;
  }

  bool OnUpdate(float deltaT) override {
    // 1. Handle Camera Movement
    VecThree camPos = UpdateCamera(deltaT);

    // 2. Setup View Matrix (Translation only)
    Matrix matView = Matrix::MakeTranslation(-camPos.x, -camPos.y, -camPos.z);

    // 2. Render
    Clear();

    // All objects (including the pivot) are passed to the renderer.
    for (const auto& obj : m_sceneObjects) {
      DrawObject(obj, matView, m_projectionMatrix, Color::White);
    }

    return true;
  }
};

int main() {
  ThreeEngine app;
  // Use our new flexible Initialize method
  if (app.Initialize(800, 600, "ThreeEngine")) {
    app.Run();
  } else {
    std::cerr << "Failed to initialize engine" << std::endl;
  }
  return 0;
}
