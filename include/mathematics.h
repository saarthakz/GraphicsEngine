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

  // Combined helper that handles Transformation and Projection.
  void ProjectToScreen(const VecThree& inVec, VecThree& outVec, const Matrix& matrix);

  // Overload that also handles Viewport Scaling (NDC to Screen).
  void ProjectToScreen(const VecThree& inVec, VecThree& outVec, const Matrix& matrix, int width,
                       int height);
} // namespace mathematics
