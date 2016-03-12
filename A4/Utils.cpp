#include "Utils.hpp"

using namespace std;
using namespace glm;

dvec4 ggReflection(const dvec4 &v, const dvec4 &n) {
  return v - 2.0 * n * (dot(v,n));
}

bool ggRefraction(const dvec4 &v, dvec4 n, double n1, double n2, dvec4 &out) {
  if (dot(v,n) < 0) {
    n = -1.0 * n;
  }

  double t_i = acos(dot(v,n)/length(v)/length(n));

  if (n1 > n2 && t_i >= asin(n2/n1)) {
    return false;
  }

  double t_out = asin(n1 * sin(t_i)/n2);
  out = n1/n2 * v + (n1/n2*cos(t_i) - sqrt(1-pow(sin(t_out),2))) * n;

  return true;
}
