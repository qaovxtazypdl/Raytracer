#pragma once

#include <glm/glm.hpp>
#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess, double indexOfRefraction, bool isLight);
  virtual ~PhongMaterial();

  glm::dvec3 m_kd;
  glm::dvec3 m_ks;
  glm::dvec3 m_refr;

  double m_indexOfRefraction;
  double m_shininess;
  bool isLight;
};
