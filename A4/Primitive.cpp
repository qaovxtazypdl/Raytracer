#include "Primitive.hpp"
#include <iostream>
#include <glm/ext.hpp>

using namespace glm;
using namespace std;

const double EPSILON = 1E-4;

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
      return IntersectionInfo(t, t*ray_dir + ray_origin, normalize((ray_origin + (float)t * ray_dir) - c), 1);
    }
  }
}

IntersectionInfo NonhierBox::checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {
  //m_pos;
  //m_size;
  //box from posx posy posz to posx+sx posy+sy posz+sz
  //plane x = posx, y=posy, z=posz, so on

  double tx_first = (m_pos[0] - ray_origin[0]) / ray_dir[0];
  double ty_first = (m_pos[1] - ray_origin[1]) / ray_dir[1];
  double tz_first = (m_pos[2] - ray_origin[2]) / ray_dir[2];

  double tx_second = (m_pos[0] + m_size - ray_origin[0]) / ray_dir[0];
  double ty_second = (m_pos[1] + m_size - ray_origin[1]) / ray_dir[1];
  double tz_second = (m_pos[2] + m_size - ray_origin[2]) / ray_dir[2];

  double tmin = std::max(std::max(std::min(tx_first,tx_second), std::min(ty_first,ty_second)), std::min(tz_first,tz_second));
  double tmax = std::min(std::min(std::max(tx_first,tx_second), std::max(ty_first,ty_second)), std::max(tz_first,tz_second));

  if (tmax < 0 || tmin > tmax || tmin < EPSILON) {
    return IntersectionInfo();
  } else {
    vec4 normal;
    if (tmin == tx_first) normal = vec4(-1,0,0,0);
    else if (tmin == tx_second) normal = vec4(1,0,0,0);
    else if (tmin == ty_first) normal = vec4(0,-1,0,0);
    else if (tmin == ty_second) normal = vec4(0,1,0,0);
    else if (tmin == tz_first) normal = vec4(0,0,-1,0);
    else if (tmin == tz_second) normal = vec4(0,0,1,0);

    return IntersectionInfo(tmin, tmin*ray_dir + ray_origin, normal, 0);
  }
}


IntersectionInfo TrianglesPrimitive::checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {
  const double EPSILON = 1E-3;
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
    vec3 z = vec3(-ray_dir[0], -ray_dir[1], -ray_dir[2]);

    //intersect
    mat3 A = mat3(x,y,z);

    double D = determinant(A);
    if (abs(D) < 1E-6) {
      continue;
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
    if (dot(intersect_normal, ray_dir) > 0) intersect_normal *= -1;
    return IntersectionInfo(min_t, min_t*ray_dir + ray_origin, intersect_normal, matched_comp);
  } else {
    return IntersectionInfo();
  }
}
