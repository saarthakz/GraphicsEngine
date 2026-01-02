#include "matrix.h"

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

  matrix.Set(0, 0, fAspectRatio * fFovTanInv);
  matrix.Set(1, 1, fFovTanInv);
  matrix.Set(2, 2, fFar / (fFar - fNear));
  matrix.Set(3, 2, (-fFar * fNear) / (fFar - fNear));
  matrix.Set(2, 3, 1.0f);
  matrix.Set(3, 3, 0.0f);
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
