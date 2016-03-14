#pragma once

#include <lodepng/lodepng.h>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>
#include <iostream>

class Texture {
public:
  //file must be png.
  Texture(std::string filename);
  ~Texture() {}

  unsigned nx, ny;
  std::vector<std::vector<glm::dvec3>> grid;

  glm::dvec3 getColorAt(std::pair<double, double> uv);
  glm::dvec4 getNormPerturbance(const glm::dvec4 &norm, const std::pair<double, double> &uv, int bump_channel);
};
