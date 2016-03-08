#include "Intersection.hpp"
#include <algorithm>

using namespace std;

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

IntersectionPoint IntersectionPoint::UNION(const IntersectionPoint &other) {
  IntersectionPoint result;
  if ((intersect_t_1 < other.intersect_t_1 && intersect_t_2 < other.intersect_t_1) ||
    (intersect_t_1 > other.intersect_t_1 && intersect_t_1 > other.intersect_t_2)
  ) {
    return result;
  }

  if (intersect_t_1 < other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result = IntersectionPoint(*this, other);
    } else {
      result = *this;
    }
  } else if (intersect_t_1 > other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result = other;
    } else {
      result = IntersectionPoint(other, *this);
    }
  } else if (intersect_t_1 == other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result = IntersectionPoint(*this, other);
    } else {
      result = *this;
    }
  }
  return result;
}

IntersectionInfo IntersectionInfo::UNION(const IntersectionInfo &other) {
  /*for(IntersectionPoint &p : intersections) {
    cout << p.intersect_t_1 << " A " << p.intersect_t_2 << endl;
  }
  for(const IntersectionPoint &p : other.intersections) {
    cout << p.intersect_t_1 << " B " << p.intersect_t_2 << endl;
  }*/

  vector<IntersectionPoint> result;
  if (intersections.size() == 0) {
    intersections = other.intersections;
    return *this;
  } if (other.intersections.size() == 0) {
    return *this;
  }

  int i = 1, j = 0;
  IntersectionPoint current = intersections[0];
  while(i < intersections.size() && j < other.intersections.size()) {
    bool doIncJ = false, doIncI = false;
    if (intersections[i].intersect_t_1 >= other.intersections[j].intersect_t_1) {
      IntersectionPoint isec = current.UNION(other.intersections[j]);
      if (!isec.valid) {
        result.push_back(current);
        current = other.intersections[j];
      } else {
        current = isec;
      }
      doIncJ = true;
    } if (intersections[i].intersect_t_1 <= other.intersections[j].intersect_t_1) {
      IntersectionPoint isec = current.UNION(intersections[i]);
      if (!isec.valid) {
        result.push_back(current);
        current = intersections[i];
      } else {
        current = isec;
      }
      doIncI = true;
    }
    i += doIncI;
    j += doIncJ;
  }
  while (i < intersections.size()) {
    IntersectionPoint isec = current.UNION(intersections[i]);
    if (!isec.valid) {
      result.push_back(current);
      current = intersections[i];
    } else {
      current = isec;
    }
    i++;
  }
  while (j < other.intersections.size()) {
    IntersectionPoint isec = current.UNION(other.intersections[j]);
    if (!isec.valid) {
      result.push_back(current);
      current = other.intersections[j];
    } else {
      current = isec;
    }
    j++;
  }
  result.push_back(current);
  /*for(IntersectionPoint &p : result) {
    cout << p.intersect_t_1 << " R " << p.intersect_t_2 << endl;
  }*/
  intersections = result;
  return *this;
}




IntersectionPoint IntersectionPoint::DIFFERENCE(const IntersectionPoint &other) {
  return IntersectionPoint();
}
IntersectionInfo IntersectionInfo::DIFFERENCE(const IntersectionInfo &other) {
  return *this;
}




IntersectionPoint IntersectionPoint::INTERSECT(const IntersectionPoint &other) {
  return IntersectionPoint();
}
IntersectionInfo IntersectionInfo::INTERSECT(const IntersectionInfo &other) {
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

