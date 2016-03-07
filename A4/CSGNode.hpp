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
  INTERSECT,
  DIFFERENCE
};

// A polygonal CSGNode.
class CSGNode : public SceneNode {
public:
  CSGNode(const std::string& name);
  virtual IntersectionInfo testHit(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const;
  void setCSGChildren(CSGAction action, SceneNode *left, SceneNode *right);

  ~CSGNode() {}
private:
  SceneNode *m_left, *m_right;
  CSGAction m_action;
};
