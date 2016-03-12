#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>
#include "Flags.hpp"

glm::dvec4 ggReflection(const glm::dvec4 &v, const glm::dvec4 &n);
bool ggRefraction(const glm::dvec4 &v, const glm::dvec4 &n, double n1, double n2, glm::dvec4 &out);
std::pair<double,double> fresnel(const glm::dvec4 &v, const glm::dvec4 &n, double n1, double n2);
