#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"
#include "SceneNode.hpp"
#include "Flags.hpp"

enum CSGAction {
  UNION,
  INTERSECTION,
  DIFFERENCE
};

// A polygonal CSGNode.
class CSGNode : public Primitive {
public:
  CSGNode(const std::string& fname);
  void set_operation(SceneNode* left, SceneNode* right, CSGAction action);
  virtual IntersectionInfo checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, double max_t);

  ~CSGNode() {
    delete boundingBox;
  }
private:
  NonhierBox *boundingBox;
  SceneNode *m_left, *m_right;
  CSGAction m_action;
};
