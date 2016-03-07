#include <iostream>
#include <fstream>
#include <limits>

#include <glm/ext.hpp>

#include "CSGNode.hpp"

using namespace glm;
using namespace std;

CSGNode::CSGNode( const std::string& name ): m_left(NULL), m_right(NULL), m_action(CSGAction::UNION), SceneNode(name)
{
}

IntersectionInfo CSGNode::testHit(const dvec4 &ray_origin, const dvec4 &ray_dir) const {
  IntersectionInfo intersectionInfo;
  dmat4 T = trans;
  dmat4 T_inv = invtrans;
  dmat3 T_invtrans = invtrans_transpose;

  for (SceneNode * node : children) {
    intersectionInfo.UNION(node->testHit(T_inv * ray_origin, T_inv * ray_dir));
  }

  IntersectionInfo CSGInt;
  if (m_action == CSGAction::UNION) {
    CSGInt.UNION(m_left->testHit(T_inv * ray_origin, T_inv * ray_dir).UNION(
      m_right->testHit(T_inv * ray_origin, T_inv * ray_dir)
    ));
  } else if (m_action == CSGAction::INTERSECT) {
    CSGInt.UNION(m_left->testHit(T_inv * ray_origin, T_inv * ray_dir).INTERSECT(
      m_right->testHit(T_inv * ray_origin, T_inv * ray_dir)
    ));
  } else if (m_action == CSGAction::DIFFERENCE) {
    CSGInt.UNION(m_left->testHit(T_inv * ray_origin, T_inv * ray_dir).DIFFERENCE(
      m_right->testHit(T_inv * ray_origin, T_inv * ray_dir)
    ));
  } else {
    cout <<  "Unrecognized CSG operation." << endl;
    throw "Unrecognized CSG operation.";
  }
  intersectionInfo.UNION(CSGInt);
  intersectionInfo.TRANSFORM_UP(T, T_invtrans);
  return intersectionInfo;
}

void CSGNode::setCSGChildren(CSGAction action, SceneNode *left, SceneNode *right) {
  m_left = left;
  m_right = right;
  m_action = action;
}

