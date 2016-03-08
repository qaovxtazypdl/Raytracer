#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <glm/gtx/io.hpp>
#include "Material.hpp"

class Primitive;
class IntersectionPoint {
public:
  bool valid;

  double intersect_t_1;
  glm::dvec4 normal_1;
  glm::dvec4 point_1;

  double intersect_t_2;
  glm::dvec4 normal_2;
  glm::dvec4 point_2;

  Material *m_material_1;
  Primitive *m_primitive_1;

  Material *m_material_2;
  Primitive *m_primitive_2;

  IntersectionPoint(
    double intersect_t_1, const glm::dvec4 &point_1, const glm::dvec4 &normal_1, Material *material_1, Primitive *primitive_1,
    double intersect_t_2, const glm::dvec4 &point_2, const glm::dvec4 &normal_2, Material *material_2, Primitive *primitive_2
  ) :
    intersect_t_1(intersect_t_1), normal_1(normal_1), point_1(point_1),
    intersect_t_2(intersect_t_2), normal_2(normal_2), point_2(point_2),
    m_material_1(material_1), m_primitive_1(primitive_1),
    m_material_2(material_2), m_primitive_2(primitive_2),
    valid(true), m_added(2)
  {}

  IntersectionPoint(const IntersectionPoint &first, bool first_1, const IntersectionPoint &second, bool first_2) :
    intersect_t_1(first_1?first.intersect_t_1:first.intersect_t_2), normal_1(first_1?first.normal_1:first.normal_2), point_1(first_1?first.point_1:first.point_2),
    intersect_t_2(first_2?second.intersect_t_1:second.intersect_t_2), normal_2(first_2?second.normal_1:second.normal_2), point_2(first_2?second.point_1:second.point_2),
    m_material_1(first_1?first.m_material_1:first.m_material_2), m_primitive_1(first_1?first.m_primitive_1:first.m_primitive_2),
    m_material_2(first_2?second.m_material_1:second.m_material_2), m_primitive_2(first_2?second.m_primitive_1:second.m_primitive_2),
    valid(true), m_added(2)
  {
    if (intersect_t_1 == intersect_t_2) {
      valid = false;
    }
  }

  IntersectionPoint(): valid(false), m_added(0) {}
  void addIntersection(double intersect_t, const glm::dvec4 &point, const glm::dvec4 &normal, Material *material, Primitive *primitive);

  IntersectionPoint UNION(const IntersectionPoint &other);
  std::pair<IntersectionPoint,IntersectionPoint> DIFFERENCE(const IntersectionPoint &other);
  IntersectionPoint INTERSECT(const IntersectionPoint &other);
private:
  int m_added;
};


class IntersectionInfo {
public:
  std::vector<IntersectionPoint> intersections;

  IntersectionInfo(const std::vector<IntersectionPoint> &ispts) {
    for (IntersectionPoint pt : ispts) {
      if (pt.valid) {
        intersections.push_back(pt);
      }
    }

    sort(intersections.begin(), intersections.end(), [](const IntersectionPoint &pt1, const IntersectionPoint &pt2) {
      return pt1.intersect_t_1 < pt2.intersect_t_1;
    });
  }

  IntersectionInfo() {}

  IntersectionPoint getFirstValidIntersection(double max_t);

  IntersectionInfo UNION(const IntersectionInfo &other);
  IntersectionInfo DIFFERENCE(const IntersectionInfo &other);
  IntersectionInfo INTERSECT(const IntersectionInfo &other);
  void TRANSFORM_UP(const glm::dmat4 &T, const glm::dmat3 &T_invtrans);
};
