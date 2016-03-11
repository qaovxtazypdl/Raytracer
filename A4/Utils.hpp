#pragma once

#include <glm/glm.hpp>
#include "Flags.hpp"

glm::dvec4 ggReflection(const glm::dvec4 &v, const glm::dvec4 &n);
glm::dvec4 ggRefraction(const glm::dvec4 &v, const glm::dvec4 &n, double indexOfRefr);
