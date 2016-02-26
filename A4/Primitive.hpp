#pragma once

#include <glm/glm.hpp>

class IntersectionInfo {
public:
  bool didIntersect;
  double intersect_t;
  glm::vec4 normal;

  IntersectionInfo(double intersect_t, const glm::vec4 &normal) :
    intersect_t(intersect_t), normal(normal), didIntersect(true)
  {}

  IntersectionInfo() : didIntersect(false) {}
};

class Primitive {
public:
  virtual ~Primitive();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir) {return IntersectionInfo();}
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir) {return IntersectionInfo();}
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir) {return IntersectionInfo();}
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir);

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

  virtual ~NonhierBox();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir);

private:
  glm::vec3 m_pos;
  double m_size;
};
