#pragma once

#include <glm/glm.hpp>
#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess);
  PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess, double indexOfRefraction, double opacity, double glossiness, bool isLight);
  virtual ~PhongMaterial();

  glm::dvec3 m_kd;
  glm::dvec3 m_ks;

  double m_indexOfRefraction;
  double m_shininess;
  double m_opacity;
  double m_glossiness;
  bool isLight;
};
