#include "Utils.hpp"

using namespace std;
using namespace glm;

dvec4 ggReflection(const dvec4 &v, const dvec4 &n) {
  return v - 2.0 * n * (dot(v,n));
}

bool ggRefraction(const dvec4 &v, const dvec4 &n, double n1, double n2, dvec4 &out) {
  double lv = length(v);
  dvec4 incidence = v / lv;
  dvec4 norm = dot(incidence, n) > 0 ? -1.0*n : n;

  double cos_ti = -dot(incidence,norm);

  //critical angle check
  if (n1 > n2 && sqrt(1 - cos_ti*cos_ti) > n2/n1) {
    return false;
  }

  double sinsq_tout = n1*n1/n2/n2 * (1 - cos_ti*cos_ti);
  out = lv * (n1/n2 * incidence + (n1/n2*cos_ti - sqrt(1 - sinsq_tout)) * norm);
  return true;
}

pair<double,double> fresnel(const dvec4 &v, const dvec4 &n, double n1, double n2) {
  if (!MACRO_REFRACTION_ON || n1 <= 0 || n2 <= 0) {
    return pair<double, double>(1,0);
  }

  double lv = length(v);
  dvec4 incidence = v / lv;
  dvec4 norm = dot(incidence, n) > 0 ? -1.0*n : n;

  double cos_ti = -dot(incidence,norm);

  if (n1 > n2 && sqrt(1 - cos_ti*cos_ti) > n2/n1) {
    return pair<double, double>(1,0);
  }

  double cos_to = sqrt(1 - n1*n1/n2/n2 * (1 - cos_ti*cos_ti));

  double Rperp = pow((n2*cos_ti - n1*cos_to)/(n2*cos_ti + n1*cos_to), 2);
  double Rpara = pow((n1*cos_ti - n2*cos_to)/(n1*cos_ti + n2*cos_to), 2);
  double R = (Rperp + Rpara) / 2;
  double T = 1-R;

  return pair<double, double>(R,T);
}
