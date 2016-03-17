#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess)
	: isLight(false)
  , m_kd(kd)
  , m_ks(ks)
	, m_indexOfRefraction(1.0)
  , m_shininess(shininess)
  , m_opacity(1.0)
  , m_glossiness(0.0)
{}

PhongMaterial::PhongMaterial(const glm::dvec3& kd, const glm::dvec3& ks, double shininess, double indexOfRefraction, double opacity, double glossiness, bool isLight)
  : m_kd(kd)
  , m_ks(ks)
  , m_indexOfRefraction(indexOfRefraction)
  , m_shininess(shininess)
  , m_opacity(opacity)
  , m_glossiness(glossiness)
  , isLight(isLight)
{}

PhongMaterial::~PhongMaterial()
{}
