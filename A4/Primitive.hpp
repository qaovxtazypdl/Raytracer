#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "polyroots.hpp"

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
  int component;

  IntersectionInfo(double intersect_t, const glm::vec4 &normal, int component) :
    intersect_t(intersect_t), normal(normal), didIntersect(true), component(component)
  {}

  IntersectionInfo() : didIntersect(false), intersect_t(0), component(-1) {}
};

class Primitive {
public:
  virtual ~Primitive();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {return IntersectionInfo();}
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {return IntersectionInfo();}
};

class Cube : public Primitive {
public:
  virtual ~Cube();
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

class TrianglesPrimitive : public Primitive {
public:
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t);
protected:
  std::vector<glm::vec3> m_vertices;
  std::vector<Triangle> m_faces;
};

using namespace std;
using namespace glm;

class NonhierBox : public TrianglesPrimitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
      m_vertices.push_back( glm::vec3(m_pos[0], m_pos[1], m_pos[2]));
      m_vertices.push_back( glm::vec3(m_pos[0], m_pos[1], m_pos[2] + m_size));
      m_vertices.push_back( glm::vec3(m_pos[0], m_pos[1] + m_size, m_pos[2]));
      m_vertices.push_back( glm::vec3(m_pos[0], m_pos[1] + m_size, m_pos[2] + m_size));
      m_vertices.push_back( glm::vec3(m_pos[0] + m_size, m_pos[1], m_pos[2]));
      m_vertices.push_back( glm::vec3(m_pos[0] + m_size, m_pos[1], m_pos[2] + m_size));
      m_vertices.push_back( glm::vec3(m_pos[0] + m_size, m_pos[1] + m_size, m_pos[2]));
      m_vertices.push_back( glm::vec3(m_pos[0] + m_size, m_pos[1] + m_size, m_pos[2] + m_size));

//front
      m_faces.push_back(Triangle(0,2,4));
      m_faces.push_back(Triangle(6,4,2));
      std::cout << "front " << to_string(normalize(vec4(cross(m_vertices[4]-m_vertices[0], m_vertices[2]-m_vertices[0]), 0.0f))) << std::endl;
      std::cout << "front " << to_string(normalize(vec4(cross(m_vertices[2]-m_vertices[6], m_vertices[4]-m_vertices[6]), 0.0f))) << std::endl;

//back
      m_faces.push_back(Triangle(5,7,1));
      m_faces.push_back(Triangle(3,1,7));
      std::cout << "back " << to_string(normalize(vec4(cross(m_vertices[1]-m_vertices[5], m_vertices[7]-m_vertices[5]), 0.0f))) << std::endl;
      std::cout << "back " << to_string(normalize(vec4(cross(m_vertices[7]-m_vertices[3], m_vertices[1]-m_vertices[3]), 0.0f))) << std::endl;


//top
      m_faces.push_back(Triangle(2,3,6));
      m_faces.push_back(Triangle(7,6,3));
      std::cout << "top " << to_string(normalize(vec4(cross(m_vertices[3]-m_vertices[2], m_vertices[6]-m_vertices[2]), 0.0f))) << std::endl;
      std::cout << "top " << to_string(normalize(vec4(cross(m_vertices[6]-m_vertices[7], m_vertices[3]-m_vertices[7]), 0.0f))) << std::endl;

//bottom
      m_faces.push_back(Triangle(0,4,1));
      m_faces.push_back(Triangle(5,1,4));
      std::cout << "bottom " << to_string(normalize(vec4(cross(m_vertices[4]-m_vertices[0], m_vertices[1]-m_vertices[0]), 0.0f))) << std::endl;
      std::cout << "bottom " << to_string(normalize(vec4(cross(m_vertices[1]-m_vertices[5], m_vertices[4]-m_vertices[5]), 0.0f))) << std::endl;

//left
      m_faces.push_back(Triangle(0,1,2));
      m_faces.push_back(Triangle(3,2,1));
      std::cout << "left " << to_string(normalize(vec4(cross(m_vertices[1]-m_vertices[0], m_vertices[2]-m_vertices[0]), 0.0f))) << std::endl;
      std::cout << "left " << to_string(normalize(vec4(cross(m_vertices[2]-m_vertices[3], m_vertices[1]-m_vertices[3]), 0.0f))) << std::endl;

//right
      m_faces.push_back(Triangle(4,6,5));
      m_faces.push_back(Triangle(7,5,6));
      std::cout << "right " << to_string(normalize(vec4(cross(m_vertices[6]-m_vertices[4], m_vertices[5]-m_vertices[4]), 0.0f))) << std::endl;
      std::cout << "right " << to_string(normalize(vec4(cross(m_vertices[5]-m_vertices[7], m_vertices[6]-m_vertices[7]), 0.0f))) << std::endl;

      cout << to_string(m_vertices[6]-m_vertices[4]) << endl;
      cout << to_string(m_vertices[5]-m_vertices[4]) << endl;
      cout << to_string(cross(m_vertices[5]-m_vertices[7], m_vertices[6]-m_vertices[7])) << endl;
  }

  virtual ~NonhierBox();

private:
  glm::vec3 m_pos;
  double m_size;
};
