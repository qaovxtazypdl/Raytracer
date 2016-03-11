#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess, double indexOfRefraction, bool isLight)
	: isLight(isLight)
  , m_kd(kd)
  , m_ks(ks)
	, m_indexOfRefraction(indexOfRefraction)
  , m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}
