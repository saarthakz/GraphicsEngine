#include "engine.h"
#include "mathematics.h"
#include "matrix.h"
#include "mesh.h"
#include "object.h"
#include "primitives.h"

#include <cassert>
#include <iostream>
#include <vector>

class ThreeEngine : public Engine {
  private:
  Mesh m_cubeAsset;
  std::vector<Object> m_sceneObjects;
  Matrix m_projectionMatrix{4, 4};

  public:
  ThreeEngine() = default;

  protected:
  bool OnCreate() override {
    // 1. Create the base asset once (Centered)
    primitives::AddCubeToMesh(m_cubeAsset, -0.5f, -0.5f, -0.5f);

    // 2. Create actual game objects that USE that asset
    Object cube1;
    cube1.meshAsset = &m_cubeAsset;
    cube1.position = {0.0f, 0.0f, 8.0f}; // Pushed back from 3.0
    cube1.rotation = {0.0f, 0.0f, 0.0f};
    m_sceneObjects.push_back(cube1);

    // 3. Setup Projection
    float fAspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
    m_projectionMatrix = Matrix::MakeProjection(90.0f, fAspectRatio, 0.1f, 100.0f);

    return true;
  }

  bool OnUpdate(float deltaT) override {
    Clear();

    for (auto& obj : m_sceneObjects) {
      // 1. Update State
      obj.rotation.y += 60.0f * deltaT;
      obj.rotation.x += 30.0f * deltaT;

      // 2. Pipeline: Final Matrix = World * Projection
      Matrix matFinal = Matrix::Multiply(obj.GetWorldMatrix(), m_projectionMatrix);

      // 3. Render
      for (const auto& tri : obj.meshAsset->tris) {
        Triangle triProjected;
        for (int i = 0; i < 3; i++) {
          mathematics::ProjectToScreen(tri.points[i], triProjected.points[i], matFinal,
                                       GetScreenWidth(), GetScreenHeight());
        }

        // Now we can just draw the triangle directly
        DrawTriangle(triProjected, Color::White);
      }
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
