#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"
#include "PhongMaterial.hpp"
#include "Material.hpp"
#include "Flags.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &m_material);

  ~Mesh() {
    delete boundingBox;
  }
private:
  bool computeBGT(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Triangle tri, double &beta, double &gamma, double &t);
  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
  NonhierBox *boundingBox;
  std::vector<glm::vec3> m_vertices;
  std::vector<Triangle> m_faces;
};
