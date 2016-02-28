#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess)
	: m_kd(kd)
  , m_ks(ks)
  , m_refr(0.0f)
	, m_indexOfRefraction(1)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}
