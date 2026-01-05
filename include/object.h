#pragma once
#include "mathematics.h"
#include "matrix.h"
#include "mesh.h"

class Object {
  private:
  Mesh* m_pMeshAsset = nullptr;
  VecThree m_vPosition = {0.0f, 0.0f, 0.0f};
  VecThree m_vRotation = {0.0f, 0.0f, 0.0f}; // Euler angles in degrees
  float m_fScale = 1.0f;
  Object* m_pParent = nullptr;

  // Cache and Dirty Flag
  mutable bool m_bDirty = true;
  mutable Matrix m_cachedLocalMatrix{4, 4};

  void UpdateLocalMatrix() const {
    if (!m_bDirty)
      return;

    Matrix matScale = Matrix::MakeIdentity();
    matScale.Set(0, 0, m_fScale);
    matScale.Set(1, 1, m_fScale);
    matScale.Set(2, 2, m_fScale);

    Matrix matRotX = Matrix::MakeRotationX(mathematics::DegToRad(m_vRotation.x));
    Matrix matRotY = Matrix::MakeRotationY(mathematics::DegToRad(m_vRotation.y));
    Matrix matRotZ = Matrix::MakeRotationZ(mathematics::DegToRad(m_vRotation.z));
    Matrix matTrans = Matrix::MakeTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);

    m_cachedLocalMatrix = Matrix::Multiply(matScale, matRotX);
    m_cachedLocalMatrix = Matrix::Multiply(m_cachedLocalMatrix, matRotY);
    m_cachedLocalMatrix = Matrix::Multiply(m_cachedLocalMatrix, matRotZ);
    m_cachedLocalMatrix = Matrix::Multiply(m_cachedLocalMatrix, matTrans);

    m_bDirty = false;
  }

  public:
  Object() = default;

  // Setters
  void SetMesh(Mesh* mesh) { m_pMeshAsset = mesh; }
  void SetPosition(VecThree pos) {
    m_vPosition = pos;
    m_bDirty = true;
  }
  void SetRotation(VecThree rot) {
    m_vRotation = rot;
    m_bDirty = true;
  }
  void SetScale(float s) {
    m_fScale = s;
    m_bDirty = true;
  }
  void SetParent(Object* p) { m_pParent = p; }

  // Getters
  Mesh* GetMesh() const { return m_pMeshAsset; }
  VecThree GetPosition() const { return m_vPosition; }
  VecThree GetRotation() const { return m_vRotation; }
  float GetScale() const { return m_fScale; }
  Object* GetParent() const { return m_pParent; }

  Matrix GetWorldMatrix() const {
    UpdateLocalMatrix();
    if (m_pParent) {
      return Matrix::Multiply(m_cachedLocalMatrix, m_pParent->GetWorldMatrix());
    }
    return m_cachedLocalMatrix;
  }
};
