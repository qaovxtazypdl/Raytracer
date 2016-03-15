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

dvec3 Texture::getColorAt(pair<double, double> uv) {
  if (uv.first < 0) uv.first = 0;
  if (uv.second < 0) uv.second = 0;
  if (uv.first > 1) uv.first = 1;
  if (uv.second > 1) uv.second = 1;

  double di = (nx-2) * uv.first;
  double dj = (ny-2) * uv.second;

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

double brightness(const dvec3 &color, int bump_channel) {
  return (color[0] + color[1] + color[2]) / 3.0;
}

dvec4 Texture::getNormPerturbance(const glm::dvec4 &norm, const UVPackage &uvp, int bump_channel) {
  dvec3 norm3 = dvec3(norm);
  double epsilon = 1.0/512;

/*
if (!(dot(norm, uvp.Ov) < 1E-6 && dot(norm, uvp.Ou) < 1E-6 && dot(uvp.Ou, uvp.Ov) < 1E-6)) {
  cout << (abs(dot(norm3, dvec3(uvp.Ov)))) << endl;
  cout << (abs(dot(norm3, dvec3(uvp.Ou)))) << endl;
  cout << (abs(dot(dvec3(uvp.Ou), dvec3(uvp.Ov)))) << endl;
  cout << endl;
}*/

  //cout << cross(dvec3(uvp.Ou), dvec3(uvp.Ov)) / dvec3(norm) << endl;

  double Bu = (brightness(getColorAt({uvp.uv.first+epsilon, uvp.uv.second}), bump_channel) - brightness(getColorAt({uvp.uv.first-epsilon, uvp.uv.second}), bump_channel)) / 2.0 / epsilon;
  double Bv = (brightness(getColorAt({uvp.uv.first, uvp.uv.second+epsilon}), bump_channel) - brightness(getColorAt({uvp.uv.first, uvp.uv.second-epsilon}), bump_channel)) / 2.0 / epsilon;
  return dvec4(Bu * cross(norm3, dvec3(uvp.Ov)) - Bv * cross(norm3, dvec3(uvp.Ou)), 0.0);
}














