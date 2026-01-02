#pragma once
#include "mathematics.h"
#include "matrix.h"
#include "mesh.h"

struct Object {
  Mesh* meshAsset;
  VecThree position;
  VecThree rotation; // in degrees
  float scale = 1.0f;

  Matrix GetWorldMatrix() const {
    // 1. Create Scale matrix
    Matrix matScale = Matrix::MakeIdentity();
    matScale.Set(0, 0, scale);
    matScale.Set(1, 1, scale);
    matScale.Set(2, 2, scale);

    // 2. Create Rotation matrices
    Matrix matRotX = Matrix::MakeRotationX(mathematics::DegToRad(rotation.x));
    Matrix matRotY = Matrix::MakeRotationY(mathematics::DegToRad(rotation.y));
    Matrix matRotZ = Matrix::MakeRotationZ(mathematics::DegToRad(rotation.z));

    // 3. Create Translation matrix
    Matrix matTrans = Matrix::MakeTranslation(position.x, position.y, position.z);

    // 4. Combine them: Scale -> Rotate -> Translate
    Matrix world = Matrix::Multiply(matScale, matRotX);
    world = Matrix::Multiply(world, matRotY);
    world = Matrix::Multiply(world, matRotZ);
    world = Matrix::Multiply(world, matTrans);

    return world;
  }
};
