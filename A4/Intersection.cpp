#include "Intersection.hpp"

using namespace std;
using namespace glm;

void IntersectionPoint::addIntersection(double intersect_t, const glm::dvec4 &point, const glm::dvec4 &normal, const MaterialPackage &material, Primitive *primitive, const UVPackage &uvp) {
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
    uvp_1 = uvp;
    m_added++;
  } else if (m_added == 1) {
    if (intersect_t_1 > intersect_t) {
      intersect_t_2 = intersect_t_1;
      normal_2 = normal_1;
      point_2 = point_1;
      m_material_2 = m_material_1;
      m_primitive_2 = m_primitive_1;
      uvp_2 = uvp_1;

      intersect_t_1 = intersect_t;
      normal_1 = normal;
      point_1 = point;
      m_material_1 = material;
      uvp_1 = uvp;
    } else {
      intersect_t_2 = intersect_t;
      normal_2 = normal;
      point_2 = point;
      m_material_2 = material;
      m_primitive_2 = primitive;
      uvp_2 = uvp;
    }
    valid = true;
    m_added++;
  }
}


IntersectionPoint IntersectionInfo::getFirstValidIntersection(double max_t) {
  const double EPSILON = 1E-10;

  for (IntersectionPoint pt : intersections) {
    //check > 2 to check if its a shadow ray calculation.
    if ((max_t > 2 || !pt.m_material_1.m_material->isLight) && pt.intersect_t_1 > EPSILON && pt.intersect_t_1 < max_t) {
      return pt;
    } else if ((max_t > 2 || !pt.m_material_2.m_material->isLight) && pt.intersect_t_2 > EPSILON && pt.intersect_t_2 < max_t) {
      swap(pt.intersect_t_2, pt.intersect_t_1);
      swap(pt.normal_2, pt.normal_1);
      swap(pt.point_2, pt.point_1);
      swap(pt.m_material_2, pt.m_material_1);
      swap(pt.m_primitive_2, pt.m_primitive_1);
      swap(pt.uvp_2, pt.uvp_1);
      return pt;
    }
  }

  return IntersectionPoint();
}

IntersectionPoint IntersectionPoint::UNION(const IntersectionPoint &other) {
  IntersectionPoint result;
  if ((intersect_t_1 < other.intersect_t_1 && intersect_t_2 < other.intersect_t_1) ||
    (intersect_t_1 > other.intersect_t_1 && intersect_t_1 > other.intersect_t_2)
  ) {
    return result;
  }

  if (intersect_t_1 <= other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result = IntersectionPoint(*this, true, other, false);
    } else {
      result = *this;
    }
  } else if (intersect_t_1 > other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result = other;
    } else {
      result = IntersectionPoint(other, true, *this, false);
    }
  }
  return result;
}

IntersectionInfo IntersectionInfo::UNION(const IntersectionInfo &other) {
  vector<IntersectionPoint> result;
  if (intersections.size() == 0) {
    intersections = other.intersections;
    return *this;
  } if (other.intersections.size() == 0) {
    return *this;
  }

  int i, j;
  IntersectionPoint current;
  if (intersections[0].intersect_t_1 < other.intersections[0].intersect_t_1) {
    i = 1;
    j = 0;
    current = intersections[0];
  } else {
    i = 0;
    j = 1;
    current = other.intersections[0];
  }
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
  intersections = result;
  return *this;
}




std::pair<IntersectionPoint,IntersectionPoint> IntersectionPoint::DIFFERENCE(const IntersectionPoint &other) {
  IntersectionPoint result1, result2;
  if ((intersect_t_1 < other.intersect_t_1 && intersect_t_2 < other.intersect_t_1) ||
    (intersect_t_1 > other.intersect_t_1 && intersect_t_1 > other.intersect_t_2)
  ) {
    return std::pair<IntersectionPoint,IntersectionPoint>(*this, result2);
  }

  if (intersect_t_1 < other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result1 = IntersectionPoint(*this, true, other, true);
      result1.reverseNormal(false);
      result2 = IntersectionPoint();
    } else {
      result1 = IntersectionPoint(*this, true, other, true);
      result1.reverseNormal(false);
      result2 = IntersectionPoint(other, false, *this, false);
      result2.reverseNormal(true);
    }
  } else if (intersect_t_1 >= other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result1 = IntersectionPoint();
      result2 = IntersectionPoint();
    } else {
      result1 = IntersectionPoint();
      result2 = IntersectionPoint(other, false, *this, false);
      result2.reverseNormal(true);
    }
  }
  return std::pair<IntersectionPoint,IntersectionPoint>(result1, result2);
}

IntersectionInfo IntersectionInfo::DIFFERENCE(const IntersectionInfo &other) {
  vector<IntersectionPoint> result;
  if (intersections.size() == 0 || other.intersections.size() == 0) {
    return *this;
  }

  int i = 0, j = 0;
  while(i < intersections.size() && j < other.intersections.size()) {
    if (intersections[i].intersect_t_2 >= other.intersections[j].intersect_t_1) {
      //compute intersection, increment other.
      pair<IntersectionPoint,IntersectionPoint> isecs = intersections[i].DIFFERENCE(other.intersections[j]);
      if (isecs.first.valid) {
        if (other.intersections[j].intersect_t_2 > intersections[i].intersect_t_1) {
          result.push_back(isecs.first);
        }
      }
      if (isecs.second.valid) {
        intersections[i] = isecs.second;
        j++;
      } else {
        if (other.intersections[j].intersect_t_2 <= intersections[i].intersect_t_1) {
          j++;
        } else {
          i++;
        }
      }
    } else if (intersections[i].intersect_t_2 < other.intersections[j].intersect_t_1) {
      result.push_back(intersections[i]);
      i++;
    }
  }
  while (i < intersections.size()) {
    result.push_back(intersections[i]);
    i++;
  }

  intersections = result;
  return *this;
}


IntersectionPoint IntersectionPoint::INTERSECT(const IntersectionPoint &other) {
  IntersectionPoint result;
  if ((intersect_t_1 < other.intersect_t_1 && intersect_t_2 < other.intersect_t_1) ||
    (intersect_t_1 > other.intersect_t_1 && intersect_t_1 > other.intersect_t_2)
  ) {
    return result;
  }

  if (intersect_t_1 < other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result = IntersectionPoint(other, true, *this, false);
    } else {
      result = other;
    }
  } else if (intersect_t_1 >= other.intersect_t_1) {
    if (intersect_t_2 < other.intersect_t_2) {
      result = *this;
    } else {
      result = IntersectionPoint(*this, true, other, false);
    }
  }
  return result;
}

IntersectionInfo IntersectionInfo::INTERSECT(const IntersectionInfo &other) {
  vector<IntersectionPoint> result;
  if (intersections.size() == 0 || other.intersections.size() == 0) {
    intersections = result;
    return *this;
  }

  int i = 0, j = 0;
  while(i < intersections.size() && j < other.intersections.size()) {
    if (intersections[i].intersect_t_2 >= other.intersections[j].intersect_t_1) {
      //compute intersection, increment other.
      IntersectionPoint isec = intersections[i].INTERSECT(other.intersections[j]);
      if (isec.valid) {
        result.push_back(isec);
      }
      if (intersections[i].intersect_t_2 >= other.intersections[j].intersect_t_2) {
        j++;
      } else {
        i++;
      }
    } else if (intersections[i].intersect_t_2 < other.intersections[j].intersect_t_1) {
      i++;
    }
  }
  intersections = result;
  return *this;
}

void IntersectionInfo::TRANSFORM_UP(const glm::dmat4 &T, const glm::dmat3 &T_invtrans) {
  for (IntersectionPoint &pt : intersections) {
    pt.normal_1 = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.normal_1), 0.0));
    pt.uvp_1.Ou = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.uvp_1.Ou), 0.0));
    pt.uvp_1.Ov = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.uvp_1.Ov), 0.0));
    pt.point_1 = T * pt.point_1;

    pt.normal_2 = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.normal_2), 0.0));
    pt.uvp_2.Ou = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.uvp_2.Ou), 0.0));
    pt.uvp_2.Ov = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.uvp_2.Ov), 0.0));
    pt.point_2 = T * pt.point_2;
  }
}

void IntersectionPoint::reverseNormal(bool first) {
  if (first) {
    normal_1 *= -1.0;
  } else {
    normal_2 *= -1.0;
  }
}

