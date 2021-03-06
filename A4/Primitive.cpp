#include "Primitive.hpp"
#include <iostream>
#include <glm/ext.hpp>

using namespace glm;
using namespace std;

const double EPSILON = 1E-12;

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


IntersectionInfo NonhierSphere::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, double max_t) {
  //m_pos;
  //m_radius;
  dvec4 a = ray_origin;
  dvec4 b = ray_dir + ray_origin;
  dvec4 c = dvec4(m_pos, 1.0);

  double roots[2];
  size_t numRoots = quadraticRoots(dot(b-a, b-a), 2*dot(b-a, a-c), dot(c-a, c-a)-m_radius*m_radius, roots);

  if (numRoots != 2) {
    return IntersectionInfo();
  } else {
    double t = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    if (length(ray_origin - c) < m_radius - 1) {
      t = t_2;
    }

    if (t > max_t || t < EPSILON) {
      return IntersectionInfo();
    } else {
      return IntersectionInfo(t, t*ray_dir + ray_origin, normalize((ray_origin + t * ray_dir) - c));
    }
  }
}

//refernce: https://tavianator.com/fast-branchless-raybounding-box-intersections/
IntersectionInfo NonhierBox::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, double max_t) {
  glm::dvec4 inv_ray_dir = 1.0/ray_dir;

  double tx_first = (m_pos[0] - ray_origin[0]) * inv_ray_dir[0];
  double ty_first = (m_pos[1] - ray_origin[1]) * inv_ray_dir[1];
  double tz_first = (m_pos[2] - ray_origin[2]) * inv_ray_dir[2];

  double tx_second = (m_pos[0] + m_size[0] - ray_origin[0]) * inv_ray_dir[0];
  double ty_second = (m_pos[1] + m_size[1] - ray_origin[1]) * inv_ray_dir[1];
  double tz_second = (m_pos[2] + m_size[2] - ray_origin[2]) * inv_ray_dir[2];

  double tmin = std::max(std::max(std::min(tx_first,tx_second), std::min(ty_first,ty_second)), std::min(tz_first,tz_second));
  double tmax = std::min(std::min(std::max(tx_first,tx_second), std::max(ty_first,ty_second)), std::max(tz_first,tz_second));

  if (tmax < -EPSILON || tmin > tmax || (tmin < EPSILON && tmax < EPSILON)) {
    return IntersectionInfo();
  } else {
    if (tmin < EPSILON && tmax > EPSILON) {
      tmin = tmax;
    }

    if (tmin > max_t) {
      return IntersectionInfo();
    }

    dvec4 normal;
    if (tmin == tx_first) normal = dvec4(-1,0,0,0);
    else if (tmin == tx_second) normal = dvec4(1,0,0,0);
    else if (tmin == ty_first) normal = dvec4(0,-1,0,0);
    else if (tmin == ty_second) normal = dvec4(0,1,0,0);
    else if (tmin == tz_first) normal = dvec4(0,0,-1,0);
    else if (tmin == tz_second) normal = dvec4(0,0,1,0);

    return IntersectionInfo(tmin, tmin*ray_dir + ray_origin, normal);
  }
}

