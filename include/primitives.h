#pragma once
#include "mesh.h"

namespace primitives {
  // Adds a unit cube to an existing mesh at the specified location
  inline void AddCubeToMesh(Mesh &mesh, float x, float y, float z, float fScale = 1.0f) {
    float s = fScale;

    // Define the 12 triangles of a cube relative to (x, y, z)
    std::vector<Triangle> cubeTris = {// SOUTH
                                      {x, y, z, x, y + s, z, x + s, y + s, z},
                                      {x, y, z, x + s, y + s, z, x + s, y, z},
                                      // EAST
                                      {x + s, y, z, x + s, y + s, z, x + s, y + s, z + s},
                                      {x + s, y, z, x + s, y + s, z + s, x + s, y, z + s},
                                      // NORTH
                                      {x + s, y, z + s, x + s, y + s, z + s, x, y + s, z + s},
                                      {x + s, y, z + s, x, y + s, z + s, x, y, z + s},
                                      // WEST
                                      {x, y, z + s, x, y + s, z + s, x, y + s, z},
                                      {x, y, z + s, x, y + s, z, x, y, z},
                                      // TOP
                                      {x, y + s, z, x, y + s, z + s, x + s, y + s, z + s},
                                      {x, y + s, z, x + s, y + s, z + s, x + s, y + s, z},
                                      // BOTTOM
                                      {x + s, y, z + s, x, y, z + s, x, y, z},
                                      {x + s, y, z + s, x, y, z, x + s, y, z}};

    for (auto &t : cubeTris) {
      mesh.tris.push_back(t);
    }
  }

  // Adds a horizontal floor of cubes
  inline void AddFloor(Mesh &mesh, float x, float y, float z, int w, int d, float fScale = 1.0f) {
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < d; j++) {
        AddCubeToMesh(mesh, x + (i * fScale), y, z + (j * fScale), fScale);
      }
    }
  }

  // Adds a vertical wall of cubes (along the X axis)
  inline void AddWallX(Mesh &mesh, float x, float y, float z, int w, int h, float fScale = 1.0f) {
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < h; j++) {
        AddCubeToMesh(mesh, x + (i * fScale), y + (j * fScale), z, fScale);
      }
    }
  }

  // Adds a vertical wall of cubes (along the Z axis)
  inline void AddWallZ(Mesh &mesh, float x, float y, float z, int d, int h, float fScale = 1.0f) {
    for (int i = 0; i < d; i++) {
      for (int j = 0; j < h; j++) {
        AddCubeToMesh(mesh, x, y + (j * fScale), z + (i * fScale), fScale);
      }
    }
  }
} // namespace primitives
