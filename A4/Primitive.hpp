#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "polyroots.hpp"
#include "Material.hpp"
#include <glm/gtx/io.hpp>
#include "Intersection.hpp"

class Primitive;
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



class Primitive {
public:
  virtual ~Primitive();
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {return IntersectionInfo();}
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material);

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
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material);
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


class Cone : public Primitive {
public:
  Cone()
  {
  }

  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material);
  virtual ~Cone();

private:
  glm::vec3 m_pos;
  glm::vec3 m_size;
};


class Cylinder : public Primitive {
public:
  Cylinder()
  {
  }

  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material);
  virtual ~Cylinder();
};


class Torus : public Primitive {
public:
  Torus(double inner) : m_inner(inner)
  {
  }
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material);
  virtual ~Torus();
private:
  double m_inner;
};

class Hyperboloid : public Primitive {
public:
  Hyperboloid(double inner) : m_inner(inner)
  {
  }
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material);
  virtual ~Hyperboloid();

private:
  double m_inner;
};
