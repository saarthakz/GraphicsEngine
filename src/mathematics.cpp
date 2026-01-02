#include "mathematics.h"

#include "matrix.h"
#include "mesh.h"

#include <cassert>
#include <cmath>

namespace mathematics {

  float DegToRad(float fDegrees) { return fDegrees * (PI / 180.0f); }

  float RadToDeg(float fRadians) { return fRadians * (180.0f / PI); }

  void MultiplyMatrixVector(const VecThree& inVec, VecThree& outVec, float& w,
                            const Matrix& matrix) {
    assert(matrix.GetRows() == 4 && matrix.GetCols() == 4);

    outVec.x = inVec.x * matrix.Get(0, 0) + inVec.y * matrix.Get(1, 0) +
               inVec.z * matrix.Get(2, 0) + matrix.Get(3, 0);
    outVec.y = inVec.x * matrix.Get(0, 1) + inVec.y * matrix.Get(1, 1) +
               inVec.z * matrix.Get(2, 1) + matrix.Get(3, 1);
    outVec.z = inVec.x * matrix.Get(0, 2) + inVec.y * matrix.Get(1, 2) +
               inVec.z * matrix.Get(2, 2) + matrix.Get(3, 2);

    w = inVec.x * matrix.Get(0, 3) + inVec.y * matrix.Get(1, 3) + inVec.z * matrix.Get(2, 3) +
        matrix.Get(3, 3);
  }

  void PerspectiveDivide(VecThree& vec, float w) {
    if (w != 0.0f) {
      vec.x /= w;
      vec.y /= w;
      vec.z /= w;
    }
  }

  void NDCToScreen(VecThree& vec, int width, int height) {
    vec.x = (vec.x + 1.0f) * 0.5f * (float)width;
    vec.y = (vec.y + 1.0f) * 0.5f * (float)height;
  }

  void ProjectToScreen(const VecThree& inVec, VecThree& outVec, const Matrix& matrix) {
    float w = 1.0f;
    MultiplyMatrixVector(inVec, outVec, w, matrix);
    PerspectiveDivide(outVec, w);
  }

  void ProjectToScreen(const VecThree& inVec, VecThree& outVec, const Matrix& matrix, int width,
                       int height) {
    ProjectToScreen(inVec, outVec, matrix);
    NDCToScreen(outVec, width, height);
  }

} // namespace mathematics
