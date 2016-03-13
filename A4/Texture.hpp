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

  glm::dvec3 getColorAt(double u, double v);
  glm::dvec4 normalD(double u, double v);
};
