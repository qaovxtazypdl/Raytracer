#include "Intersection.hpp"

void IntersectionPoint::addIntersection(double intersect_t, const glm::dvec4 &point, const glm::dvec4 &normal, Material *material, Primitive *primitive) {
  if (m_added == 2) {
    std::cout << "ADDING TOO MUCH" << std::endl;
    throw "ADDING TOO MUCH";
    return;
  } else if (m_added == 0) {
    intersect_t_1 = intersect_t;
    normal_1 = normal;
    point_1 = point;
    m_material_1 = material;
    m_primitive_1 = primitive;
    m_added++;
  } else if (m_added == 1) {
    if (intersect_t_1 > intersect_t) {
      intersect_t_2 = intersect_t_1;
      normal_2 = normal_1;
      point_2 = point_1;
      m_material_2 = m_material_1;
      m_primitive_2 = m_primitive_1;

      intersect_t_1 = intersect_t;
      normal_1 = normal;
      point_1 = point;
      m_material_1 = material;
      m_primitive_1 = primitive;
    } else {
      intersect_t_2 = intersect_t;
      normal_2 = normal;
      point_2 = point;
      m_material_2 = material;
      m_primitive_2 = primitive;
    }
    valid = true;
    m_added++;
  }
}


IntersectionPoint IntersectionInfo::getFirstValidIntersection(double max_t) {
  const double EPSILON = 1E-11;
  IntersectionPoint result;
  double min_t = std::numeric_limits<double>::infinity();
  bool found = false;

  for (IntersectionPoint pt : intersections) {
    if (pt.intersect_t_1 < min_t && pt.intersect_t_1 > EPSILON && pt.intersect_t_1 < max_t) {
      min_t = pt.intersect_t_1;
      result = pt;
      found = true;
    }
  }

  if (found) {
    return result;
  } else {
    return IntersectionPoint();
  }
}

IntersectionInfo& IntersectionInfo::UNION(const IntersectionInfo &other) {
  for (IntersectionPoint pt : other.intersections) {
    if (pt.valid) {
      intersections.push_back(pt);
    }
  }
  return *this;
}

IntersectionInfo& IntersectionInfo::DIFFERENCE(const IntersectionInfo &other) {
  return *this;
}

IntersectionInfo& IntersectionInfo::INTERSECT(const IntersectionInfo &other) {
  return *this;
}

void IntersectionInfo::TRANSFORM_UP(const glm::dmat4 &T, const glm::dmat3 &T_invtrans) {
  for (IntersectionPoint &pt : intersections) {
    pt.normal_1 = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.normal_1), 0.0));
    pt.point_1 = T * pt.point_1;

    pt.normal_2 = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.normal_2), 0.0));
    pt.point_2 = T * pt.point_2;
  }
}

