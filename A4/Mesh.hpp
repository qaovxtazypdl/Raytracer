#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"


// A polygonal mesh.
class Mesh : public TrianglesPrimitive {
public:
  Mesh( const std::string& fname );

private:
  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};
