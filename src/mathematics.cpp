#include "mathematics.h"

#include "matrix.h"
#include "mesh.h"

#include <cassert>
#include <cmath>

namespace mathematics {

  float DegToRad(float fDegrees) { return fDegrees * (PI / 180.0f); }

  float RadToDeg(float fRadians) { return fRadians * (180.0f / PI); }

  void MultiplyMatrixVector(const VecThree& vecIn, VecThree& vecOut, float& w,
                            const Matrix& matrix) {
    assert(matrix.GetRows() == 4 && matrix.GetCols() == 4);

    vecOut.x = vecIn.x * matrix.Get(0, 0) + vecIn.y * matrix.Get(1, 0) +
               vecIn.z * matrix.Get(2, 0) + matrix.Get(3, 0);
    vecOut.y = vecIn.x * matrix.Get(0, 1) + vecIn.y * matrix.Get(1, 1) +
               vecIn.z * matrix.Get(2, 1) + matrix.Get(3, 1);
    vecOut.z = vecIn.x * matrix.Get(0, 2) + vecIn.y * matrix.Get(1, 2) +
               vecIn.z * matrix.Get(2, 2) + matrix.Get(3, 2);

    w = vecIn.x * matrix.Get(0, 3) + vecIn.y * matrix.Get(1, 3) + vecIn.z * matrix.Get(2, 3) +
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

  namespace vector {
    VecThree Add(const VecThree& v1, const VecThree& v2) {
      return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    }

    VecThree Sub(const VecThree& v1, const VecThree& v2) {
      return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    }

    VecThree Mul(const VecThree& v1, float k) { return {v1.x * k, v1.y * k, v1.z * k}; }

    VecThree Div(const VecThree& v1, float k) { return {v1.x / k, v1.y / k, v1.z / k}; }

    float DotProduct(const VecThree& v1, const VecThree& v2) {
      return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    float Length(const VecThree& v) { return sqrtf(DotProduct(v, v)); }

    VecThree Normalise(const VecThree& v) {
      float l = Length(v);
      return {v.x / l, v.y / l, v.z / l};
    }

    VecThree CrossProduct(const VecThree& v1, const VecThree& v2) {
      VecThree v;
      v.x = v1.y * v2.z - v1.z * v2.y;
      v.y = v1.z * v2.x - v1.x * v2.z;
      v.z = v1.x * v2.y - v1.y * v2.x;
      return v;
    }

    VecThree IntersectPlane(const VecThree& plane_p, const VecThree& plane_n,
                            const VecThree& lineStart, const VecThree& lineEnd) {
      VecThree n = Normalise(plane_n);
      float plane_d = -DotProduct(n, plane_p);
      float ad = DotProduct(lineStart, n);
      float bd = DotProduct(lineEnd, n);
      float t = (-plane_d - ad) / (bd - ad);
      VecThree lineStartToEnd = Sub(lineEnd, lineStart);
      VecThree lineToIntersect = Mul(lineStartToEnd, t);
      return Add(lineStart, lineToIntersect);
    }
  } // namespace vector

  VecThree CalculateNormal(const Triangle& tri) {
    VecThree line1 = vector::Sub(tri.points[1], tri.points[0]);
    VecThree line2 = vector::Sub(tri.points[2], tri.points[0]);
    return vector::Normalise(vector::CrossProduct(line1, line2));
  }

  bool IsBackface(const Triangle& tri, const VecThree& vCameraPos) {
    VecThree normal = CalculateNormal(tri);
    VecThree vRay = vector::Sub(tri.points[0], vCameraPos);
    return vector::DotProduct(normal, vRay) >= 0.0f;
  }

  int TriangleClipAgainstPlane(VecThree plane_p, VecThree plane_n, const Triangle& triIn,
                               Triangle& triOut1, Triangle& triOut2) {
    // Make sure plane normal is indeed normal
    plane_n = vector::Normalise(plane_n);

    // Return signed shortest distance from point to plane, plane normal must be normalised
    auto dist = [&](const VecThree& p) {
      return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z -
              vector::DotProduct(plane_n, plane_p));
    };

    // Create two temporary storage arrays to classify points either side of plane
    // If distance test is greater than or equal to zero, point is 'inside'
    const VecThree* inside_points[3];
    int nInsidePointCount = 0;
    const VecThree* outside_points[3];
    int nOutsidePointCount = 0;

    // Get signed distance of each point in triangle to plane and classify
    for (int i = 0; i < 3; i++) {
      if (dist(triIn.points[i]) >= 0)
        inside_points[nInsidePointCount++] = &triIn.points[i];
      else
        outside_points[nOutsidePointCount++] = &triIn.points[i];
    }

    // Now classify triangle points, and break the input triangle into
    // smaller output triangles if required. There are four possible outcomes...

    if (nInsidePointCount == 0) {
      // All points lie on the outside of plane, so clip whole triangle
      // It ceases to exist
      return 0; // No triangles returned
    }

    if (nInsidePointCount == 3) {
      // All points lie on the inside of plane, so do nothing
      // and allow the triangle to simply pass through
      triOut1 = triIn;
      return 1; // Just the one returned original triangle
    }

    if (nInsidePointCount == 1 && nOutsidePointCount == 2) {
      // Triangle should be clipped. As two points lie outside, the triangle becomes smaller;
      // a new triangle is created.

      // The inside point is valid, but we need two new points that are at the intersections
      // with the plane.
      triOut1.points[0] = *inside_points[0];

      // and the two new points are at the locations where the
      // edges intersect with the plane
      triOut1.points[1] =
        vector::IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
      triOut1.points[2] =
        vector::IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

      return 1; // Return the newly formed single triangle
    }

    if (nInsidePointCount == 2 && nOutsidePointCount == 1) {
      // Triangle should be clipped. As two points lie inside, the triangle becomes a quad.
      // We split the quad into two new triangles.

      // The first triangle consists of the two inside points and a new
      // point at the intersection between the first inside point and the
      // outside point
      triOut1.points[0] = *inside_points[0];
      triOut1.points[1] = *inside_points[1];
      triOut1.points[2] =
        vector::IntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

      // The second triangle consists of the second inside point, a new
      // point at the intersection between the second inside point and the
      // outside point, and the newly created point from the first triangle
      triOut2.points[0] = *inside_points[1];
      triOut2.points[1] = triOut1.points[2];
      triOut2.points[2] =
        vector::IntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

      return 2; // Return two newly formed triangles which form a quad
    }

    return 0;
  }

  void ProjectToScreen(const VecThree& vecIn, VecThree& vecOut, const Matrix& matrix) {
    float w = 1.0f;
    MultiplyMatrixVector(vecIn, vecOut, w, matrix);
    PerspectiveDivide(vecOut, w);
  }

  void ProjectToScreen(const VecThree& vecIn, VecThree& vecOut, const Matrix& matrix, int width,
                       int height) {
    ProjectToScreen(vecIn, vecOut, matrix);
    NDCToScreen(vecOut, width, height);
  }

} // namespace mathematics
