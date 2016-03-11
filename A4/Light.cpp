#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"

using namespace glm;
using namespace std;

Light::Light()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.colour)
  	  << ", " << glm::to_string(l.position) << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.falloff[i];
  }
  out << "]";
  return out;
}

dvec3 Light::shadowIntensity(const FlatPrimitives &nodes, const PhongMaterial &mat, const dvec4 &v_eye, const dvec4 &point, const dvec4 &normal) {
  dvec3 color;
  dvec4 l_dir = dvec4(position, 1.0) - point;
  IntersectionPoint pt = nodes.firstHitInNodeList(point, l_dir, 1.0);

  if (!pt.valid) {
    double d = length(l_dir);
    double attenuation = 1.0/(falloff[0] + falloff[1]*d + falloff[2]*d*d);
    dvec4 reflDirection = ggReflection(l_dir, normal);
    double l_dot_n = dot(normal, normalize(l_dir));
    double r_dot_v = dot(normalize(v_eye), normalize(reflDirection));

    if (l_dot_n > 0)
      color += attenuation * mat.m_kd * l_dot_n * colour;
    if (r_dot_v > 0)
      color += attenuation * mat.m_ks * pow(r_dot_v, mat.m_shininess) * colour;
  }

  return color;
}
