#pragma once

#include <iosfwd>

#include <glm/glm.hpp>
#include "FlatPrimitives.hpp"
#include "Utils.hpp"
#include "PhongMaterial.hpp"

// Represents a simple point light.
struct Light {
  Light();

  glm::dvec3 colour;
  glm::dvec3 position;
  double falloff[3];

  glm::dvec3 lightColor(const FlatPrimitives &nodes, const PhongMaterial &mat, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal);
};

std::ostream& operator<<(std::ostream& out, const Light& l);
