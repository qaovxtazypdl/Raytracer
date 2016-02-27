#include "Primitive.hpp"

using namespace glm;

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}


IntersectionInfo NonhierSphere::checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {
  return IntersectionInfo();
}


IntersectionInfo NonhierBox::checkRayIntersection(const glm::vec4 &ray_origin, const glm::vec4 &ray_dir, double max_t) {
  return IntersectionInfo();
}
