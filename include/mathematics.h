#pragma once
#include "matrix.h"
#include "mesh.h"

#include <cassert>
#include <cmath>

namespace mathematics {
  const float PI = 3.14159265f;

  float DegToRad(float fDegrees);
  float RadToDeg(float fRadians);

  // Multiplies a VecThree by a 4x4 Matrix.
  void MultiplyMatrixVector(const VecThree& inVec, VecThree& outVec, float& w,
                            const Matrix& matrix);

  // Performs the perspective divide (Division by W) on the vector.
  void PerspectiveDivide(VecThree& vec, float w);

  // Maps Normalized Device Coordinates (-1 to 1) to screen pixel coordinates.
  void NDCToScreen(VecThree& vec, int screenWidth, int screenHeight);

  // Vector mathematics
  namespace vector {
    VecThree Add(const VecThree& v1, const VecThree& v2);
    VecThree Sub(const VecThree& v1, const VecThree& v2);
    VecThree Mul(const VecThree& v1, float k);
    VecThree Div(const VecThree& v1, float k);
    float DotProduct(const VecThree& v1, const VecThree& v2);
    float Length(const VecThree& v);
    VecThree Normalise(const VecThree& v);
    VecThree CrossProduct(const VecThree& v1, const VecThree& v2);
    VecThree IntersectPlane(const VecThree& plane_p, const VecThree& plane_n,
                            const VecThree& lineStart, const VecThree& lineEnd);
  } // namespace vector

  // High-level 3D Helper
  VecThree CalculateNormal(const Triangle& tri);
  bool IsBackface(const Triangle& tri, const VecThree& vCameraRay);

  // Clipping
  int TriangleClipAgainstPlane(VecThree plane_p, VecThree plane_n, const Triangle& triIn,
                               Triangle& triOut1, Triangle& triOut2);

  // Combined helper that handles Transformation and Projection.
  void ProjectToScreen(const VecThree& inVec, VecThree& outVec, const Matrix& matrix);

  // Overload that also handles Viewport Scaling (NDC to Screen).
  void ProjectToScreen(const VecThree& inVec, VecThree& outVec, const Matrix& matrix, int width,
                       int height);
} // namespace mathematics
