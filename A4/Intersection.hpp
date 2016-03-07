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
  IntersectionPoint(): valid(false), m_added(0) {}
  void addIntersection(double intersect_t, const glm::dvec4 &point, const glm::dvec4 &normal, Material *material, Primitive *primitive);
private:
  int m_added;
};


class IntersectionInfo {
public:
  bool didIntersect;
  std::vector<IntersectionPoint> intersections;

  IntersectionInfo(const std::vector<IntersectionPoint> &ispts) :
    didIntersect(true), intersections(ispts)
  {}

  IntersectionInfo() : didIntersect(false) {}

  IntersectionPoint getFirstValidIntersection(double max_t);

  void UNION(const IntersectionInfo &other);
  void DIFFERENCE(const IntersectionInfo &other);
  void INTERSECT(const IntersectionInfo &other);
  void TRANSFORM_UP(const glm::dmat4 &T, const glm::dmat3 &T_invtrans);
};
