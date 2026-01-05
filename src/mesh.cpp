#include "mesh.h"

#include "mathematics.h"

#include <cmath>
#include <fstream>
#include <sstream>

bool Mesh::LoadFromObjectFile(std::string sFilename) {
  std::ifstream f(sFilename);
  if (!f.is_open())
    return false;

  // Local cache of vertices to store 'v' entries before they are combined into faces
  std::vector<VecThree> verts;
  m_tris.clear();

  while (!f.eof()) {
    char line[128];
    f.getline(line, 128);

    std::stringstream s;
    s << line;

    char junk;

    // 'vertex' lines define a vertex coordinate (x, y, z)
    if (line[0] == 'v') {
      VecThree vertex;
      // Read the 'v' character into 'junk' and then the three floats
      s >> junk >> vertex.x >> vertex.y >> vertex.z;
      verts.push_back(vertex);
    }

    // 'face' lines define a face (triangle in our case) using vertex indices
    // Example: f 1 2 3 means a triangle made of the 1st, 2nd, and 3rd vertices defined in the file
    if (line[0] == 'f') {
      int faceIndices[3];
      // Read the 'f' character into 'junk' and then the three integer indices
      s >> junk >> faceIndices[0] >> faceIndices[1] >> faceIndices[2];

      // OBJ indices are 1-based, so we subtract 1 to convert to 0-based C++ vector indexing
      Triangle tri;
      tri.points[0] = verts[faceIndices[0] - 1];
      tri.points[1] = verts[faceIndices[1] - 1];
      tri.points[2] = verts[faceIndices[2] - 1];

      m_tris.push_back(tri);
    }
  }

  return true;
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
  for (auto& tri : m_tris) {
    for (int i = 0; i < 3; i++) {
      float w = 1.0f;
      VecThree in = tri.points[i];
      mathematics::MultiplyMatrixVector(in, tri.points[i], w, mat);
    }
  }
}
