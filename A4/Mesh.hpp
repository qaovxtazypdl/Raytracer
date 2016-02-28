#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );
  virtual IntersectionInfo checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t);

  ~Mesh() {
    delete boundingBox;
  }
private:
  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
  NonhierBox *boundingBox;
  std::vector<glm::vec3> m_vertices;
  std::vector<Triangle> m_faces;
};
