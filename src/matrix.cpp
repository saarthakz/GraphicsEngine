#include "matrix.h"

#include "mathematics.h"
#include "mesh.h"

#include <cmath>
#include <cstring>

Matrix::Matrix(int rows, int cols) : m_iRows(rows), m_iCols(cols) {
  m_data = new float[rows * cols]{};
}

Matrix::~Matrix() { delete[] m_data; }

Matrix::Matrix(const Matrix& other) : m_iRows(other.m_iRows), m_iCols(other.m_iCols) {
  m_data = new float[m_iRows * m_iCols];
  std::memcpy(m_data, other.m_data, m_iRows * m_iCols * sizeof(float));
}

Matrix::Matrix(Matrix&& other) noexcept
  : m_iRows(other.m_iRows), m_iCols(other.m_iCols), m_data(other.m_data) {
  other.m_data = nullptr;
  other.m_iRows = 0;
  other.m_iCols = 0;
}

Matrix& Matrix::operator=(const Matrix& other) {
  if (this != &other) {
    float* newData = new float[other.m_iRows * other.m_iCols];
    std::memcpy(newData, other.m_data, other.m_iRows * other.m_iCols * sizeof(float));
    delete[] m_data;
    m_data = newData;
    m_iRows = other.m_iRows;
    m_iCols = other.m_iCols;
  }
  return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
  if (this != &other) {
    delete[] m_data;
    m_data = other.m_data;
    m_iRows = other.m_iRows;
    m_iCols = other.m_iCols;
    other.m_data = nullptr;
    other.m_iRows = 0;
    other.m_iCols = 0;
  }
  return *this;
}

void Matrix::Set(int row, int col, float value) { m_data[row * m_iCols + col] = value; }

float Matrix::Get(int row, int col) const { return m_data[row * m_iCols + col]; }

int Matrix::GetRows() const { return m_iRows; }

int Matrix::GetCols() const { return m_iCols; }

Matrix Matrix::MakeIdentity() {
  Matrix matrix(4, 4);
  matrix.Set(0, 0, 1.0f);
  matrix.Set(1, 1, 1.0f);
  matrix.Set(2, 2, 1.0f);
  matrix.Set(3, 3, 1.0f);
  return matrix;
}

Matrix Matrix::MakeRotationX(float fAngleRad) {
  Matrix matrix(4, 4);
  matrix.Set(0, 0, 1.0f);
  matrix.Set(1, 1, cosf(fAngleRad));
  matrix.Set(1, 2, sinf(fAngleRad));
  matrix.Set(2, 1, -sinf(fAngleRad));
  matrix.Set(2, 2, cosf(fAngleRad));
  matrix.Set(3, 3, 1.0f);
  return matrix;
}

Matrix Matrix::MakeRotationY(float fAngleRad) {
  Matrix matrix(4, 4);
  matrix.Set(0, 0, cosf(fAngleRad));
  matrix.Set(0, 2, sinf(fAngleRad));
  matrix.Set(1, 1, 1.0f);
  matrix.Set(2, 0, -sinf(fAngleRad));
  matrix.Set(2, 2, cosf(fAngleRad));
  matrix.Set(3, 3, 1.0f);
  return matrix;
}

Matrix Matrix::MakeRotationZ(float fAngleRad) {
  Matrix matrix(4, 4);
  matrix.Set(0, 0, cosf(fAngleRad));
  matrix.Set(0, 1, sinf(fAngleRad));
  matrix.Set(1, 0, -sinf(fAngleRad));
  matrix.Set(1, 1, cosf(fAngleRad));
  matrix.Set(2, 2, 1.0f);
  matrix.Set(3, 3, 1.0f);
  return matrix;
}

Matrix Matrix::MakeTranslation(float x, float y, float z) {
  Matrix matrix = MakeIdentity();
  matrix.Set(3, 0, x);
  matrix.Set(3, 1, y);
  matrix.Set(3, 2, z);
  return matrix;
}

Matrix Matrix::MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar) {
  Matrix matrix(4, 4);
  float fFovRad = fFovDegrees * (3.14159265f / 180.0f);
  float fFovTanInv = 1.0f / tanf(fFovRad / 2.0f);
  /*
   * Projection Matrix Layout:
   * [ k     0      0                          0 ]
   * [ 0     a*k    0                          0 ]
   * [ 0     0      fFar/(fFar-fNear)          1 ]
   * [ 0     0      -fFar*fNear/(fFar-fNear)   0 ]
   *
   * a = Aspect ratio - Compensates for screen width/height difference to avoid stretching
   * k = FOV factor - 1 / tan(fov / 2) - Horizontal FOV (XZ plane); higher k compresses. Used for
   * normalizing the X coordinate. fNear = Near plane - the closest visible distance; maps to 0 in
   * normalized depth fFar = Far plane - the furthest visible distance; maps to 1 in normalized
   * depth
   *
   * Transformation of [x, y, z, 1]:
   * x' = x * k
   * y' = y * a * k
   * z' = (fFar * (z - fNear)) / (fFar - fNear)
   * w' = z
   *
   * After w-division (w' = z):
   * x_out = (x * k) / z
   * y_out = (y * a * k) / z
   * z_out = (fFar * (z - fNear)) / (z * (fFar - fNear))
   */

  matrix.Set(0, 0, fFovTanInv);
  matrix.Set(1, 1, fAspectRatio * fFovTanInv);
  matrix.Set(2, 2, fFar / (fFar - fNear));
  matrix.Set(3, 2, (-fFar * fNear) / (fFar - fNear));
  matrix.Set(2, 3, 1.0f);
  matrix.Set(3, 3, 0.0f);
  return matrix;
}

/**
 * Computes a matrix that "points at" a target from a position.
 * Used primarily for camera orientation where:
 * Forward = Target - Pos
 * Right   = Up (World) x Forward
 * Up      = Forward x Right
 *
 * Matrix Layout (Column-Major concept, stored as Row-Major):
 * [ Rx Ry Rz 0 ] - Right Vector
 * [ Ux Uy Uz 0 ] - Up Vector
 * [ Fx Fy Fz 0 ] - Forward Vector
 * [ Px Py Pz 1 ] - Position
 */
Matrix Matrix::PointAt(const VecThree& vecCameraPos, const VecThree& vecCameraTgt,
                       const VecThree& vecWorldUp) {

  // Calculate new forward direction
  VecThree vecDirection = mathematics::vector::Sub(vecCameraTgt, vecCameraPos);
  vecDirection = mathematics::vector::Normalise(vecDirection);

  // Calculate new Up direction
  VecThree vecScaledDirection = mathematics::vector::Mul(
    vecDirection, mathematics::vector::DotProduct(vecWorldUp, vecDirection));
  VecThree vecNewUp = mathematics::vector::Sub(vecWorldUp, vecScaledDirection);
  vecNewUp = mathematics::vector::Normalise(vecNewUp);

  // New Right direction is just cross product
  VecThree vecNewRight = mathematics::vector::CrossProduct(vecNewUp, vecDirection);

  // Construct Dimensioning and Translation Matrix
  Matrix matrix(4, 4);
  matrix.Set(0, 0, vecNewRight.x);
  matrix.Set(0, 1, vecNewRight.y);
  matrix.Set(0, 2, vecNewRight.z);
  matrix.Set(0, 3, 0.0f);
  matrix.Set(1, 0, vecNewUp.x);
  matrix.Set(1, 1, vecNewUp.y);
  matrix.Set(1, 2, vecNewUp.z);
  matrix.Set(1, 3, 0.0f);
  matrix.Set(2, 0, vecDirection.x);
  matrix.Set(2, 1, vecDirection.y);
  matrix.Set(2, 2, vecDirection.z);
  matrix.Set(2, 3, 0.0f);
  matrix.Set(3, 0, vecCameraPos.x);
  matrix.Set(3, 1, vecCameraPos.y);
  matrix.Set(3, 2, vecCameraPos.z);
  matrix.Set(3, 3, 1.0f);
  return matrix;
}

/**
 * Specialized fast inverse for Orthonormal matrices (Rotation + Translation).
 * This is perfect for the View Matrix (inverse of Camera Matrix).
 *
 * Given a PointAt Matrix M:
 * [ R00 R01 R02  0 ]  (Right Vector)
 * [ R10 R11 R12  0 ]  (Up Vector)
 * [ R20 R21 R22  0 ]  (Forward Vector)
 * [ Tx  Ty  Tz   1 ]  (Position)
 *
 * The Inverse M^-1 is:
 * [ R00 R10 R20  0 ]  (Transpose of 3x3)
 * [ R01 R11 R21  0 ]
 * [ R02 R12 R22  0 ]
 * [ -T·R -T·U -T·F 1 ] (Negative Dot Product of Pos and Axis)
 */
Matrix Matrix::QuickPointAtInverse(const Matrix& m) {
  Matrix matrix(4, 4);
  matrix.Set(0, 0, m.Get(0, 0));
  matrix.Set(0, 1, m.Get(1, 0));
  matrix.Set(0, 2, m.Get(2, 0));
  matrix.Set(0, 3, 0.0f);
  matrix.Set(1, 0, m.Get(0, 1));
  matrix.Set(1, 1, m.Get(1, 1));
  matrix.Set(1, 2, m.Get(2, 1));
  matrix.Set(1, 3, 0.0f);
  matrix.Set(2, 0, m.Get(0, 2));
  matrix.Set(2, 1, m.Get(1, 2));
  matrix.Set(2, 2, m.Get(2, 2));
  matrix.Set(2, 3, 0.0f);
  matrix.Set(3, 0,
             -(m.Get(3, 0) * matrix.Get(0, 0) + m.Get(3, 1) * matrix.Get(1, 0) +
               m.Get(3, 2) * matrix.Get(2, 0)));
  matrix.Set(3, 1,
             -(m.Get(3, 0) * matrix.Get(0, 1) + m.Get(3, 1) * matrix.Get(1, 1) +
               m.Get(3, 2) * matrix.Get(2, 1)));
  matrix.Set(3, 2,
             -(m.Get(3, 0) * matrix.Get(0, 2) + m.Get(3, 1) * matrix.Get(1, 2) +
               m.Get(3, 2) * matrix.Get(2, 2)));
  matrix.Set(3, 3, 1.0f);
  return matrix;
}

Matrix Matrix::Multiply(const Matrix& matOne, const Matrix& matTwo) {
  Matrix matrix(4, 4);
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      matrix.Set(row, col,
                 matOne.Get(row, 0) * matTwo.Get(0, col) + matOne.Get(row, 1) * matTwo.Get(1, col) +
                   matOne.Get(row, 2) * matTwo.Get(2, col) +
                   matOne.Get(row, 3) * matTwo.Get(3, col));
    }
  }
  return matrix;
}
