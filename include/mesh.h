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
  private:
  std::vector<Triangle> m_tris;

  // Internal helper to apply a transformation matrix to all triangles in the mesh.
  void ApplyMatrix(Matrix& mat);

  public:
  bool LoadFromObjectFile(std::string sFilename);

  const std::vector<Triangle>& GetTriangles() const { return m_tris; }
  void AddTriangle(const Triangle& tri) { m_tris.push_back(tri); }
  void Clear() { m_tris.clear(); }

  // Rotates the mesh around the X axis. fAngle is in degrees.
  void RotateX(float fAngle);

  // Rotates the mesh around the Y axis. fAngle is in degrees.
  void RotateY(float fAngle);

  // Rotates the mesh around the Z axis. fAngle is in degrees.
  void RotateZ(float fAngle);

  // Translates the mesh by the given offsets.
  void Translate(float x, float y, float z);
};
