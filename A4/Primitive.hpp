#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "polyroots.hpp"
#include "Material.hpp"
#include <glm/gtx/io.hpp>

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


class IntersectionPoint {
public:
  bool valid;

  double intersect_t_1;
  glm::dvec4 normal_1;
  glm::dvec4 point_1;

  double intersect_t_2;
  glm::dvec4 normal_2;
  glm::dvec4 point_2;

  Material *m_material_1;
  Primitive *m_primitive_1;

  Material *m_material_2;
  Primitive *m_primitive_2;

  IntersectionPoint(
    double intersect_t_1, const glm::dvec4 &point_1, const glm::dvec4 &normal_1, Material *material_1, Primitive *primitive_1,
    double intersect_t_2, const glm::dvec4 &point_2, const glm::dvec4 &normal_2, Material *material_2, Primitive *primitive_2
  ) :
    intersect_t_1(intersect_t_1), normal_1(normal_1), point_1(point_1),
    intersect_t_2(intersect_t_2), normal_2(normal_2), point_2(point_2),
    m_material_1(material_1), m_primitive_1(primitive_1),
    m_material_2(material_2), m_primitive_2(primitive_2),
    valid(true)
  {}

  IntersectionPoint(): valid(false) {}
};

class IntersectionInfo {
public:
  bool didIntersect;
  std::vector<IntersectionPoint> intersections;

  IntersectionInfo(const std::vector<IntersectionPoint> &ispts) :
    didIntersect(true), intersections(ispts)
  {}

  IntersectionInfo() : didIntersect(false) {}

  IntersectionPoint getFirstValidIntersection(double max_t) {
    const double EPSILON = 1E-11;
    IntersectionPoint result;
    double min_t = std::numeric_limits<double>::infinity();
    bool found = false;

    for (IntersectionPoint pt : intersections) {
      if (pt.intersect_t_1 < min_t && pt.intersect_t_1 > EPSILON && pt.intersect_t_1 < max_t) {
        min_t = pt.intersect_t_1;
        result = pt;
        found = true;
      }
    }

    if (found) {
      return result;
    } else {
      return IntersectionPoint();
    }
  }

  void UNION(const IntersectionInfo &other) {
    for (IntersectionPoint pt : other.intersections) {
      intersections.push_back(pt);
    }
  }

  void DIFFERENCE(const IntersectionInfo &other) {

  }

  void INTERSECT(const IntersectionInfo &other) {

  }

  void TRANSFORM_UP(const glm::dmat4 &T, const glm::dmat3 &T_invtrans) {
    for (IntersectionPoint &pt : intersections) {
      pt.normal_1 = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.normal_1), 0.0));
      pt.point_1 = T * pt.point_1;

      pt.normal_2 = glm::normalize(glm::dvec4(T_invtrans * glm::dvec3(pt.normal_2), 0.0));
      pt.point_2 = T * pt.point_2;
    }
  }
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
