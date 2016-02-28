#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "polyroots.hpp"
#include <glm/gtx/io.hpp>

struct Triangle
{
  size_t v1;
  size_t v2;
  size_t v3;

  Triangle( size_t pv1, size_t pv2, size_t pv3 )
    : v1( pv1 )
    , v2( pv2 )
    , v3( pv3 )
  {}
};

class IntersectionInfo {
public:
  bool didIntersect;
  double intersect_t;
  glm::vec4 normal;
  glm::vec4 point;

  IntersectionInfo(double intersect_t, const glm::vec4 &point, const glm::vec4 &normal) :
    intersect_t(intersect_t), normal(normal), didIntersect(true), point(point)
  {}

  IntersectionInfo() : didIntersect(false), intersect_t(0) {}
};

class Primitive {
public:
  virtual ~Primitive();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {return IntersectionInfo();}
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t);

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  NonhierBox(const glm::vec3& pos, glm::vec3 size)
    : m_pos(pos), m_size(size)
  {
  }
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t);
  virtual ~NonhierBox();

private:
  glm::vec3 m_pos;
  glm::vec3 m_size;
};

class Sphere : public NonhierSphere {
public:
  Sphere()
    : NonhierSphere(glm::vec3(0,0,0), 1.0)
  {
  }
  virtual ~Sphere();
};

class Cube : public NonhierBox {
public:
  Cube()
    : NonhierBox(glm::vec3(0,0,0), 1.0)
  {
  }
  virtual ~Cube();
};
