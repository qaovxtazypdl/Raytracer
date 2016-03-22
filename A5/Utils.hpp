#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>
#include "Flags.hpp"
#include <random>


class Utils {
public:
  static std::default_random_engine utils_rng;
  static std::uniform_real_distribution<double> utils_distr;

  static double randbtwn(double a, double b);
  static std::pair<glm::dvec4, glm::dvec4> generateCircleAxes(const glm::dvec3 &normal);
  static glm::dvec4 ggReflection(const glm::dvec4 &v, const glm::dvec4 &n);
  static bool ggRefraction(const glm::dvec4 &v, const glm::dvec4 &n, double n1, double n2, glm::dvec4 &out);
  static std::pair<double,double> fresnel(const glm::dvec4 &v, const glm::dvec4 &n, double n1, double n2);
};
