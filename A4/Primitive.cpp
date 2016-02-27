#include "Primitive.hpp"
#include <iostream>
#include <glm/ext.hpp>

using namespace glm;
using namespace std;

const double EPSILON = -1E-2;

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}


IntersectionInfo NonhierSphere::checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {
  //m_pos;
  //m_radius;
  vec4 a = ray_origin;
  vec4 b = ray_dir + ray_origin;
  vec4 c = vec4(m_pos, 1.0f);

  double roots[2];
  size_t numRoots = quadraticRoots(dot(b-a, b-a), 2*dot(b-a, a-c), dot(c-a, c-a)-m_radius*m_radius, roots);

  if (numRoots != 2) {
    return IntersectionInfo();
  } else {
    double t = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    if (t > max_t || t < EPSILON) {
      return IntersectionInfo();
    } else {
      return IntersectionInfo(t, normalize((ray_origin + (float)t * ray_dir) - c), 1);
    }
  }
}


IntersectionInfo TrianglesPrimitive::checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {
  const double EPSILON = 1E-6;
  double min_t = std::numeric_limits<double>::infinity();
  bool foundOne = false;
  vec4 intersect_normal;

  int i = 0, matched_comp = -1;
  //return maximum intersection z
  for (Triangle tri : m_faces) {
    i++;

    vec3 v1 = m_vertices[tri.v1], v2 = m_vertices[tri.v2], v3 = m_vertices[tri.v3];
    vec4 ray_b = ray_origin + ray_dir;
    vec3 a = vec3(ray_origin[0], ray_origin[1], ray_origin[2]);
    vec3 b = vec3(ray_b[0], ray_b[1], ray_b[2]);

    vec3 R = a-v1;
    vec3 x = v2-v1;
    vec3 y = v3-v1;
    vec3 z = a-b;

    //intersect
    mat3 A = mat3(x,y,z);

    double D = determinant(A);
    if (abs(D) < EPSILON) {
      return IntersectionInfo();
    }

    A[0] = R;
    double D1 = determinant(A);

    A[0] = x;
    A[1] = R;
    double D2 = determinant(A);

    A[1] = y;
    A[2] = R;
    double D3 = determinant(A);

    double beta = D1/D;
    double gamma = D2/D;
    double t = D3/D;

    //check for intersection
    if (t >= EPSILON && t <= max_t && beta >= 0 && gamma >= 0 && beta+gamma <= 1) {
      //found! update if t value of intersection is less. (find minimum t)
      if (min_t > t) {
        foundOne = true;
        min_t = t;
        intersect_normal = normalize(vec4(cross(v2-v1, v3-v1), 0.0f));
        matched_comp = i;
      }
    }
  }

  if (foundOne) {
    return IntersectionInfo(min_t, intersect_normal, matched_comp);
  } else {
    return IntersectionInfo();
  }
}
