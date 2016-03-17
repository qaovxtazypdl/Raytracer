#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"

using namespace glm;
using namespace std;

Light::Light(const dvec3 &colour, const dvec3 &position, const dvec3 &falloff)
  : colour(colour), position(position), falloff(falloff)
{
}

dvec3 Light::lightColor(const FlatPrimitives &nodes, const dvec3 &kd, const dvec3 &ks, double shininess, const dvec4 &v_eye, const dvec4 &point, const dvec4 &normal) {
  dvec3 color;
  dvec4 l_dir = dvec4(position, 1.0) - point;
  IntersectionPoint pt = nodes.firstHitInNodeList(point, l_dir, 1.0);

  if (!pt.valid) {
    double d = length(l_dir);
    double attenuation = 1.0/(falloff[0] + falloff[1]*d + falloff[2]*d*d);
    dvec4 reflDirection = Utils::ggReflection(l_dir, normal);
    double l_dot_n = dot(normal, normalize(l_dir));
    double r_dot_v = dot(normalize(v_eye), normalize(reflDirection));

    if (l_dot_n > 0)
      color += attenuation * kd * l_dot_n * colour;
    if (r_dot_v > 0)
      color += attenuation * ks * pow(r_dot_v, shininess) * colour;
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

glm::dvec3 PlanarLight::lightColor(const FlatPrimitives &nodes, const dvec3 &kd, const dvec3 &ks, double shininess, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal) {
  dvec3 color;

  double sampleFactor = 1.0/(MACRO_NUM_PLANAR_LIGHT_SAMPLES*MACRO_NUM_PLANAR_LIGHT_SAMPLES);
  dvec4 corner = dvec4(position, 1.0) - plane_vector_1 - plane_vector_2;
  for (int i = 0; i < MACRO_NUM_PLANAR_LIGHT_SAMPLES; i++) {
    for (int j = 0; j < MACRO_NUM_PLANAR_LIGHT_SAMPLES; j++) {
      dvec4 lightPoint = corner + 2.0 * (
        (1.0/MACRO_NUM_PLANAR_LIGHT_SAMPLES*i + 0.5/MACRO_NUM_PLANAR_LIGHT_SAMPLES + Utils::randbtwn(-0.40/MACRO_NUM_PLANAR_LIGHT_SAMPLES, 0.40/MACRO_NUM_PLANAR_LIGHT_SAMPLES)) * plane_vector_1 +
        (1.0/MACRO_NUM_PLANAR_LIGHT_SAMPLES*j + 0.5/MACRO_NUM_PLANAR_LIGHT_SAMPLES + Utils::randbtwn(-0.40/MACRO_NUM_PLANAR_LIGHT_SAMPLES, 0.40/MACRO_NUM_PLANAR_LIGHT_SAMPLES)) * plane_vector_2
      );

      dvec4 l_dir = lightPoint - point;
      IntersectionPoint pt = nodes.firstHitInNodeList(point, l_dir, 1.0);

      if (!pt.valid) {
        double d = length(l_dir);
        double attenuation = 1.0/(falloff[0] + falloff[1]*d + falloff[2]*d*d);
        dvec4 reflDirection = Utils::ggReflection(l_dir, normal);
        double l_dot_n = dot(normal, normalize(l_dir));
        double r_dot_v = dot(normalize(v_eye), normalize(reflDirection));

        if (l_dot_n > 0)
          color += sampleFactor * attenuation * kd * l_dot_n * colour;
        if (r_dot_v > 0)
          color += sampleFactor * attenuation * ks * pow(r_dot_v, shininess) * colour;
      }
    }
  }

  return color;
}

glm::dvec3 SphericalLight::lightColor(const FlatPrimitives &nodes, const dvec3 &kd, const dvec3 &ks, double shininess, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal) {
  //based on direction to center, generate normal.
  //generate random radius
  //partition radius into sqrt segments, partition angle.

  dvec3 color;
  dvec4 circCenter = vec4(position, 1.0);
  dvec3 circNormal = normalize(dvec3(circCenter - point));
  double sampleFactor = 1.0/(MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL*MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR);
  for (int i = 0; i < MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL; i++) {
    for (int j = 0; j < MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR; j++) {
      double rsqCenter = 1.0/MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL * i + 0.5/MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL + Utils::randbtwn(-0.40/MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL, 0.40/MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_RADIAL);
      double thetaCenter = 1.0/MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR * j + Utils::randbtwn(-0.40/MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR, 0.40/MACRO_NUM_SPHERICAL_LIGHT_SAMPLES_ANGULAR);

      double r = sqrt(rsqCenter) * radius;
      double theta = thetaCenter * 2 * PI;

      double weight = 1.4-r/(2*radius);

      pair<dvec4, dvec4> axes = Utils::generateCircleAxes(circNormal);
      dvec4 lightPoint = circCenter +
        r * cos(theta) * axes.first +
        r * sin(theta) * axes.second;

      dvec4 l_dir = lightPoint - point;
      IntersectionPoint pt = nodes.firstHitInNodeList(point, l_dir, 1.0);

      if (!pt.valid) {
        double d = length(l_dir);
        double attenuation = 1.0/(falloff[0] + falloff[1]*d + falloff[2]*d*d);
        dvec4 reflDirection = Utils::ggReflection(l_dir, normal);
        double l_dot_n = dot(normal, normalize(l_dir));
        double r_dot_v = dot(normalize(v_eye), normalize(reflDirection));

        if (l_dot_n > 0)
          color += weight * sampleFactor * attenuation * kd * l_dot_n * colour;
        if (r_dot_v > 0)
          color += weight * sampleFactor * attenuation * ks * pow(r_dot_v, shininess) * colour;
      }
    }
  }
  return color;
}





















