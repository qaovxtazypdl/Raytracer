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
  virtual IntersectionInfo testNode(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const;
  virtual IntersectionInfo testHit(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const;
  void setCSGChildren(SceneNode *left, SceneNode *right, const std::string &action);

  ~CSGNode() {}
private:
  SceneNode *m_left, *m_right;
  CSGAction m_action;
};
