#include "Texture.hpp"

using namespace glm;
using namespace std;

Texture::Texture(string filename) {
  std::vector<unsigned char> image;
  unsigned error = lodepng::decode(image, nx, ny, filename);
  if(error) {
    cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    throw "decoder error";
  }

  for (int i = 0; i < nx; i++) {
    grid.push_back(vector<dvec3>(ny));
  }

  int pos = 0;
  while (pos < image.size()) {
    int x = (pos/4) % nx;
    int y = (pos/4) / nx;

    grid[x][y][0] = image[pos] / 255.0;
    grid[x][y][1] = image[pos+1] / 255.0;
    grid[x][y][2] = image[pos+2] / 255.0;

    pos += 4;
  }
}

dvec3 Texture::getColorAt(const pair<double, double> &uv) {
  return dvec3();
}

dvec4 Texture::normalD(const pair<double, double> &uv) {
  return dvec4();
}
