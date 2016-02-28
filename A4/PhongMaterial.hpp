#pragma once

#include <glm/glm.hpp>
#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);
  virtual ~PhongMaterial();

  glm::vec3 m_kd;
  glm::vec3 m_ks;
  glm::vec3 m_refr;

  double m_indexOfRefraction;
  double m_shininess;
};
