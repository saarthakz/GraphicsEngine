#include "three.h"

#include "mathematics.h"

#include <algorithm>
#include <cmath>

void ThreeEngine::HandleCameraRotation(float deltaT) {
  float fRotSpeed = 2.0f * deltaT;
  if (GetKey(GLFW_KEY_LEFT).bHeld)
    m_fYaw -= fRotSpeed;
  if (GetKey(GLFW_KEY_RIGHT).bHeld)
    m_fYaw += fRotSpeed;
  if (GetKey(GLFW_KEY_UP).bHeld)
    m_fPitch += fRotSpeed;
  if (GetKey(GLFW_KEY_DOWN).bHeld)
    m_fPitch -= fRotSpeed;

  if (m_fPitch > 1.56f)
    m_fPitch = 1.56f;
  if (m_fPitch < -1.56f)
    m_fPitch = -1.56f;
}

void ThreeEngine::UpdateSight() {
  m_vLookDir.x = cosf(m_fPitch) * sinf(m_fYaw);
  m_vLookDir.y = sinf(m_fPitch);
  m_vLookDir.z = cosf(m_fPitch) * cosf(m_fYaw);
}

void ThreeEngine::HandleCameraMovement(float deltaT) {
  float fSpeed = 5.0f * deltaT;
  VecThree vForward = {sinf(m_fYaw), 0.0f, cosf(m_fYaw)};
  VecThree vRight = {cosf(m_fYaw), 0.0f, -sinf(m_fYaw)};

  VecThree camPos = GetCameraPosition();
  if (GetKey(GLFW_KEY_W).bHeld) {
    camPos.x += vForward.x * fSpeed;
    camPos.z += vForward.z * fSpeed;
  }
  if (GetKey(GLFW_KEY_S).bHeld) {
    camPos.x -= vForward.x * fSpeed;
    camPos.z -= vForward.z * fSpeed;
  }
  if (GetKey(GLFW_KEY_A).bHeld) {
    camPos.x -= vRight.x * fSpeed;
    camPos.z -= vRight.z * fSpeed;
  }
  if (GetKey(GLFW_KEY_D).bHeld) {
    camPos.x += vRight.x * fSpeed;
    camPos.z += vRight.z * fSpeed;
  }

  if (GetKey(GLFW_KEY_Q).bHeld)
    camPos.y += fSpeed;
  if (GetKey(GLFW_KEY_E).bHeld)
    camPos.y -= fSpeed;

  SetCameraPosition(camPos);
}

void ThreeEngine::UpdateCamera(float deltaT) {
  HandleCameraRotation(deltaT);
  UpdateSight();
  HandleCameraMovement(deltaT);
}

void ThreeEngine::DrawMesh(const Mesh& mesh, const Matrix& matWorld, const Matrix& matView,
                           const Matrix& matProj, Color color) {
  for (const auto& tri : mesh.GetTriangles()) {
    Triangle triTransformed;
    for (int i = 0; i < 3; i++) {
      float w = 1.0f;
      mathematics::MultiplyMatrixVector(tri.points[i], triTransformed.points[i], w, matWorld);
    }

    VecThree normal = mathematics::CalculateNormal(triTransformed);
    VecThree vRay = mathematics::vector::Sub(triTransformed.points[0], m_vCameraPos);

    if (mathematics::vector::DotProduct(normal, vRay) < 0.0f) {
      float dp = mathematics::vector::DotProduct(normal, m_vLightDirection);
      Color correctedColor = Color::ApplyIntensity(color, std::max(0.1f, dp));

      Triangle triView;
      for (int i = 0; i < 3; i++) {
        float w = 1.0f;
        mathematics::MultiplyMatrixVector(triTransformed.points[i], triView.points[i], w, matView);
      }

      Triangle clipped[2];
      int nClippedTriangles = mathematics::TriangleClipAgainstPlane(
        {0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f}, triView, clipped[0], clipped[1]);

      for (int n = 0; n < nClippedTriangles; n++) {
        Triangle triProjected;
        for (int i = 0; i < 3; i++) {
          mathematics::ProjectToScreen(clipped[n].points[i], triProjected.points[i], matProj,
                                       GetScreenWidth(), GetScreenHeight());
        }
        FillTriangle(triProjected, correctedColor);
      }
    }
  }
}

void ThreeEngine::DrawObject(const Object& obj, const Matrix& matView, const Matrix& matProj,
                             Color color) {
  if (!obj.GetMesh())
    return;
  DrawMesh(*obj.GetMesh(), obj.GetWorldMatrix(), matView, matProj, color);
}

void ThreeEngine::SetLightDirection(VecThree dir) {
  m_vLightDirection = mathematics::vector::Normalise(dir);
}

bool ThreeEngine::OnCreate() {
  if (!m_meshPath.empty()) {
    m_mesh.LoadFromObjectFile(m_meshPath);
  } else {
    m_mesh.LoadFromObjectFile("resources/axes.obj");
  }
  SetLightDirection({0.2f, 1.0f, -0.5f});

  m_object.SetMesh(&m_mesh);
  m_object.SetPosition({0.0f, 0.0f, 10.0f});

  float fAspectRatio = (float)GetScreenWidth() / (float)GetScreenHeight();
  m_projectionMatrix = Matrix::MakeProjection(90.0f, fAspectRatio, 0.1f, 100.0f);
  return true;
}

bool ThreeEngine::OnUpdate(float deltaT) {
  UpdateCamera(deltaT);
  VecThree camPos = GetCameraPosition();
  VecThree vTarget = {camPos.x + m_vLookDir.x, camPos.y + m_vLookDir.y, camPos.z + m_vLookDir.z};

  Matrix matCamera = Matrix::PointAt(camPos, vTarget, GetWorldUp());
  Matrix matView = Matrix::QuickPointAtInverse(matCamera);

  Clear();
  DrawObject(m_object, matView, m_projectionMatrix, Color::White);
  return true;
}
