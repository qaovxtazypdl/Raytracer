#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "SceneNode.hpp"

struct HierarchicalNodeInfo {
  const SceneNode *node;
  glm::dmat4 mat;
  glm::dmat4 inv;
  glm::dmat3 invTranspose;

  HierarchicalNodeInfo(const SceneNode *node, glm::dmat4 mat);
};

struct FlatPrimitives {
  std::vector<HierarchicalNodeInfo> m_data;
  FlatPrimitives(const std::vector<HierarchicalNodeInfo> &data) : m_data(data) {}

  IntersectionPoint firstHitInNodeList(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_direction, double max_t) const;
};
