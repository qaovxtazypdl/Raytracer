#include "Utils.hpp"

using namespace std;
using namespace glm;

dvec4 ggReflection(const dvec4 &v, const dvec4 &n) {
  return v - 2.0 * n * (dot(v,n));
}

bool ggRefraction(const dvec4 &v, const dvec4 &n, double n1, double n2, dvec4 &out) {
  dvec4 incidence = normalize(v);
  dvec4 norm = dot(incidence, n) > 0 ? -1.0*n : n;

  double cos_ti = -dot(incidence,norm);

  //critical angle check
  if (n1 > n2 && sqrt(1 - cos_ti*cos_ti) > n2/n1) {
    return false;
  }

  double sinsq_tout = n1*n1/n2/n2 * (1 - cos_ti*cos_ti);
  out = n1/n2 * incidence + (n1/n2*cos_ti - sqrt(1 - sinsq_tout)) * norm;
  return true;
}
