#pragma once

#include <lodepng/lodepng.h>
#include <glm/glm.hpp>
#include <iostream>

class Texture {
public:
  //file must be png.
  Texture(std::string filename);

  int nx, ny;
  int **pixels;

  glm::dvec3 getColorAt(const std::pair<double, double> &uv);
  glm::dvec4 normalD(const std::pair<double, double> &uv);
};
