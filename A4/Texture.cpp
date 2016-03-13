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
  double di = (nx-1) * uv.first;
  double dj = (ny-1) * uv.second;

  int i = (int)di;
  int j = (int)dj;

  double up = di - (int)di;
  double vp = dj - (int)dj;

  dvec3 c00 = grid[i][j];
  dvec3 c01 = grid[i][j+1];
  dvec3 c10 = grid[i+1][j];
  dvec3 c11 = grid[i+1][j+1];

  return (1-up)*(1-vp)*c00 + (1-up)*(vp)*c01 + up*(1-vp)*c10 + (up)*(vp)*c11;
}

dvec4 Texture::normalD(const pair<double, double> &uv) {
  return dvec4();
}
