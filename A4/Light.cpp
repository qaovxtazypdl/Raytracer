#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"

using namespace glm;
using namespace std;

Light::Light(const dvec3 &colour, const dvec3 &position, const dvec3 &falloff)
  : colour(colour), position(position), falloff(falloff)
{
}

dvec3 Light::lightColor(const FlatPrimitives &nodes, const PhongMaterial &mat, const dvec4 &v_eye, const dvec4 &point, const dvec4 &normal) {
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

PlanarLight::PlanarLight(const glm::dvec3 &colour, const glm::dvec3 &position, const glm::dvec3 &falloff, const glm::dvec3 &plane_vector_1, const glm::dvec3 &plane_vector_2)
  : Light(colour, position, falloff), plane_vector_1(plane_vector_1, 0.0), plane_vector_2(plane_vector_2, 0.0)
{
}

SphericalLight::SphericalLight(const glm::dvec3 &colour, const glm::dvec3 &position, const glm::dvec3 &falloff, double radius)
  : Light(colour, position, falloff), radius(radius)
{
}

/*
glm::dvec3 PlanarLight::lightColor(const FlatPrimitives &nodes, const PhongMaterial &mat, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal) {

}

glm::dvec3 SphericalLight::lightColor(const FlatPrimitives &nodes, const PhongMaterial &mat, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal) {

}*/
