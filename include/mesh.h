#pragma once
#include "matrix.h"

#include <string>
#include <vector>

struct VecThree {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
};

struct Triangle {
  VecThree points[3];
};

class Mesh {
  public:
  std::vector<Triangle> tris;

  public:
  bool LoadFromObjectFile(std::string sFilename);

  // Rotates the mesh around the X axis. fAngle is in degrees.
  void RotateX(float fAngle);

  // Rotates the mesh around the Y axis. fAngle is in degrees.
  void RotateY(float fAngle);

  // Rotates the mesh around the Z axis. fAngle is in degrees.
  void RotateZ(float fAngle);

  // Translates the mesh by the given offsets.
  void Translate(float x, float y, float z);

  private:
  // Internal helper to apply a transformation matrix to all triangles in the mesh.
  void ApplyMatrix(Matrix& mat);
};
