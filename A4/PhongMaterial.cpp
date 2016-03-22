#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess)
	: m_kd(kd)
  , m_ks(ks)
  , m_refr(0.0)
	, m_indexOfRefraction(1)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}
