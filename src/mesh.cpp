#include "mesh.h"

#include "mathematics.h"

#include <cmath>

bool Mesh::LoadFromObjectFile(std::string sFilename) {
  // Placeholder for future OBJ loading
  return false;
}

void Mesh::RotateX(float fAngle) {
  Matrix mat = Matrix::MakeRotationX(mathematics::DegToRad(fAngle));
  ApplyMatrix(mat);
}

void Mesh::RotateY(float fAngle) {
  Matrix mat = Matrix::MakeRotationY(mathematics::DegToRad(fAngle));
  ApplyMatrix(mat);
}

void Mesh::RotateZ(float fAngle) {
  Matrix mat = Matrix::MakeRotationZ(mathematics::DegToRad(fAngle));
  ApplyMatrix(mat);
}

void Mesh::Translate(float x, float y, float z) {
  Matrix mat = Matrix::MakeTranslation(x, y, z);
  ApplyMatrix(mat);
}

void Mesh::ApplyMatrix(Matrix& mat) {
  for (auto& tri : tris) {
    for (int i = 0; i < 3; i++) {
      float w = 1.0f;
      VecThree in = tri.points[i];
      mathematics::MultiplyMatrixVector(in, tri.points[i], w, mat);
    }
  }
}
