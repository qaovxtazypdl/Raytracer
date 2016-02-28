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
  double tx_first = (m_pos[0] - ray_origin[0]) / ray_dir[0];
  double ty_first = (m_pos[1] - ray_origin[1]) / ray_dir[1];
  double tz_first = (m_pos[2] - ray_origin[2]) / ray_dir[2];

  double tx_second = (m_pos[0] + m_size[0] - ray_origin[0]) / ray_dir[0];
  double ty_second = (m_pos[1] + m_size[1] - ray_origin[1]) / ray_dir[1];
  double tz_second = (m_pos[2] + m_size[2] - ray_origin[2]) / ray_dir[2];

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

