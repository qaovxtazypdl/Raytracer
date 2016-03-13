#include "Texture.hpp"

using namespace glm;
using namespace std;

Texture::Texture(string filename) {
  unsigned error = lodepng::decode(image, nx, ny, filename);
  if(error) {
    cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    throw "decoder error";
  }
}

dvec3 Texture::getColorAt(const pair<double, double> &uv) {
  return dvec3();
}

dvec4 Texture::normalD(const pair<double, double> &uv) {
  return dvec4();
}
