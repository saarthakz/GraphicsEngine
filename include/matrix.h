#pragma once

class Matrix {
  int m_iRows;
  int m_iCols;
  float* m_data;

  public:
  Matrix(int rows, int cols);
  ~Matrix();

  // Disable copy constructor and assignment for now to prevent double-delete of m_data
  // unless we implement deep copying.
  Matrix(const Matrix& other);
  Matrix(Matrix&& other) noexcept;
  Matrix& operator=(const Matrix& other);
  Matrix& operator=(Matrix&& other) noexcept;

  // Move constructor/assignment could be added later for performance.

  void Set(int row, int col, float value);
  float Get(int row, int col) const;
  int GetRows() const;
  int GetCols() const;

  static Matrix MakeIdentity();
  static Matrix MakeRotationX(float fAngleRad);
  static Matrix MakeRotationY(float fAngleRad);
  static Matrix MakeRotationZ(float fAngleRad);
  static Matrix MakeTranslation(float x, float y, float z);
  static Matrix MakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
  static Matrix Multiply(const Matrix& matOne, const Matrix& matTwo);
};
