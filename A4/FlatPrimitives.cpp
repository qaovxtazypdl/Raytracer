#include "FlatPrimitives.hpp"

  HierarchicalNodeInfo::HierarchicalNodeInfo(const SceneNode *node, glm::dmat4 mat) : node(node), mat(mat) {
    inv = glm::inverse(mat);
    invTranspose = glm::transpose(glm::inverse(glm::dmat3(mat)));
  }

IntersectionPoint FlatPrimitives::firstHitInNodeList(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_direction, double max_t) const {
  IntersectionPoint result;
  double min_t = std::numeric_limits<double>::infinity();

  for (HierarchicalNodeInfo ninfo : m_data) {
    glm::dmat4 T = ninfo.mat;
    glm::dmat4 T_inv = ninfo.inv;
    glm::dmat3 T_invtrans = ninfo.invTranspose;

    IntersectionInfo info = ninfo.node->testNode(T_inv * ray_origin, T_inv * ray_direction);
    info.TRANSFORM_UP(T, T_invtrans);
    IntersectionPoint ipt = info.getFirstValidIntersection(max_t);

    if (ipt.valid && ipt.intersect_t_1 < min_t) {
      result = ipt;
      min_t = ipt.intersect_t_1;
    }
  }

  return result;
}
