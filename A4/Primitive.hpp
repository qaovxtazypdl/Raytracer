#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "polyroots.hpp"
#include "Flags.hpp"
#include "PhongMaterial.hpp"
#include "PhongMaterial.hpp"
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
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material) {return IntersectionInfo();}
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::dvec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);

protected:
  glm::dvec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::dvec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  NonhierBox(const glm::dvec3& pos, glm::dvec3 size)
    : m_pos(pos), m_size(size)
  {
  }
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);
  virtual ~NonhierBox();

protected:
  glm::dvec3 m_pos;
  glm::dvec3 m_size;
};

class Sphere : public NonhierSphere {
public:
  Sphere()
    : NonhierSphere(glm::dvec3(0,0,0), 1.0)
  {
  }
  virtual ~Sphere();
};

class Cube : public NonhierBox {
public:
  Cube()
    : NonhierBox(glm::dvec3(0,0,0), 1.0)
  {
  }
  virtual ~Cube();
};

class OneSidedCube : public NonhierBox {
public:
  OneSidedCube()
    : NonhierBox(glm::dvec3(0,0,0), 1.0)
  {
  }
  virtual ~OneSidedCube();
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);
};


class Cone : public Primitive {
public:
  Cone()
  {
  }

  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);
  virtual ~Cone();

protected:
  glm::dvec3 m_pos;
  glm::dvec3 m_size;
};


class Cylinder : public Primitive {
public:
  Cylinder()
  {
  }

  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);
  virtual ~Cylinder();
};


class Torus : public Primitive {
public:
  Torus(double inner) : m_inner(inner)
  {
  }
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);
  virtual ~Torus();
protected:
  double m_inner;
};

class Hyperboloid : public Primitive {
public:
  Hyperboloid(double inner) : m_inner(inner)
  {
  }
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);
  virtual ~Hyperboloid();

protected:
  double m_inner;
};
