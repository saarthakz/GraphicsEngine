#pragma once

#include "engine.h"
#include "matrix.h"
#include "mesh.h"
#include "object.h"

class ThreeEngine : public Engine {
  private:
  Mesh m_mesh;
  Object m_object;
  Matrix m_projectionMatrix{4, 4};
  std::string m_meshPath;

  // 3D Engine State
  VecThree m_vCameraPos = {0.0f, 0.0f, 0.0f};
  VecThree m_vLightDirection = {0.0f, 0.0f, -1.0f};
  const VecThree m_vWorldUp = {0.0f, 1.0f, 0.0f};

  // Camera state (Angles)
  float m_fYaw = 0.0f;
  float m_fPitch = 0.0f;
  VecThree m_vLookDir = {0.0f, 0.0f, 1.0f};

  void HandleCameraRotation(float deltaT);
  void UpdateSight();
  void HandleCameraMovement(float deltaT);
  void UpdateCamera(float deltaT);

  // High-level 3D Rendering (The Pipeline)
  void DrawMesh(const Mesh& mesh, const Matrix& matWorld, const Matrix& matView,
                const Matrix& matProj, Color color = Color::White);
  void DrawObject(const Object& obj, const Matrix& matView, const Matrix& matProj,
                  Color color = Color::White);

  public:
  ThreeEngine() = default;

  void LoadMeshAsset(const std::string& path) { m_meshPath = path; }

  void SetCameraPosition(VecThree pos) { m_vCameraPos = pos; }
  VecThree GetCameraPosition() const { return m_vCameraPos; }
  void SetLightDirection(VecThree dir);
  VecThree GetWorldUp() const { return m_vWorldUp; }

  protected:
  bool OnCreate() override;
  bool OnUpdate(float deltaT) override;
};
