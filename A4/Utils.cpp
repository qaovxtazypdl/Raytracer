#include "Utils.hpp"

using namespace std;
using namespace glm;


dvec4 ggReflection(const dvec4 &v, const dvec4 &n) {
  return v - 2.0 * n * (dot(v,n));
}

dvec4 ggRefraction(const dvec4 &v, const dvec4 &n, double indexOfRefr) {
  double n1 = 1.33;
  double n2 = 1.67;

  double t_i = acos(-dot(v,n)/length(v)/length(n));
  double t_out = n1 * t_i/n2;

  if (t_out >= PI/2) return dvec4(0,0,0,0);
  return n1/n2 * v + (n1/n2*cos(t_i) - sqrt(1-pow(sin(t_out),2))) * n;
}
