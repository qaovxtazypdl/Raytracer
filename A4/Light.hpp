#pragma once

#include <iosfwd>

#include <glm/glm.hpp>
#include "FlatPrimitives.hpp"
#include "Utils.hpp"
#include "PhongMaterial.hpp"

// Represents a simple point light.
class Light {
public:
  Light(const glm::dvec3 &colour, const glm::dvec3 &position, const glm::dvec3 &falloff);
  virtual ~Light() {}
  virtual glm::dvec3 lightColor(const FlatPrimitives &nodes, const PhongMaterial &mat, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal);
protected:
  glm::dvec3 colour;
  glm::dvec3 position;
  glm::dvec3 falloff;
};

class PlanarLight : public Light {
public:
  PlanarLight(const glm::dvec3 &colour, const glm::dvec3 &position, const glm::dvec3 &falloff, const glm::dvec3 &plane_vector_1, const glm::dvec3 &plane_vector_2);
  virtual ~PlanarLight() {}
  //virtual glm::dvec3 lightColor(const FlatPrimitives &nodes, const PhongMaterial &mat, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal);
protected:
  //plane centered at position, expanding through the directions given by the vectors - both subtracted and added.
  glm::dvec4 plane_vector_1;
  glm::dvec4 plane_vector_2;
};

class SphericalLight : public Light {
public:
  SphericalLight(const glm::dvec3 &colour, const glm::dvec3 &position, const glm::dvec3 &falloff, double radius);
  virtual ~SphericalLight() {}
  //virtual glm::dvec3 lightColor(const FlatPrimitives &nodes, const PhongMaterial &mat, const glm::dvec4 &v_eye, const glm::dvec4 &point, const glm::dvec4 &normal);
protected:
  double radius;
};
