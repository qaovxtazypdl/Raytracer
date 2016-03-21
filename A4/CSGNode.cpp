#include <iostream>
#include <fstream>
#include <limits>

#include <glm/ext.hpp>

#include "CSGNode.hpp"

using namespace glm;
using namespace std;

CSGNode::CSGNode( const std::string& name ): m_left(NULL), m_right(NULL), m_action(CSGAction::UNION), SceneNode(name)
{
  m_nodeType = NodeType::CSGNode;
}

IntersectionInfo CSGNode::testNode(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir) const {
  IntersectionInfo CSGInt;
  if (m_action == CSGAction::UNION) {
    CSGInt = CSGInt.UNION(m_left->testHit(ray_origin, ray_dir).UNION(
      m_right->testHit(ray_origin, ray_dir)
    ));
  } else if (m_action == CSGAction::INTERSECT) {
    CSGInt = CSGInt.UNION(m_left->testHit(ray_origin, ray_dir).INTERSECT(
      m_right->testHit(ray_origin, ray_dir)
    ));
  } else if (m_action == CSGAction::DIFFERENCE) {
    CSGInt = CSGInt.UNION(m_left->testHit(ray_origin, ray_dir).DIFFERENCE(
      m_right->testHit(ray_origin, ray_dir)
    ));
  } else {
    cout <<  "Unrecognized CSG operation." << endl;
    throw "Unrecognized CSG operation.";
  }
  return CSGInt;
}

IntersectionInfo CSGNode::testHit(const dvec4 &ray_origin, const dvec4 &ray_dir) const {
  IntersectionInfo intersectionInfo;
  dmat4 T = trans;
  dmat4 T_inv = invtrans;
  dmat3 T_invtrans = invtrans_transpose;

  if (m_boundingObject != NULL) {
    IntersectionPoint bounding = m_boundingObject->testHit(T_inv * ray_origin, T_inv * ray_dir).getFirstValidIntersection(std::numeric_limits<double>::infinity());
    if (!bounding.valid) {
      return intersectionInfo;
    }
  }

  for (SceneNode * node : children) {
    intersectionInfo = intersectionInfo.UNION(node->testHit(T_inv * ray_origin, T_inv * ray_dir));
  }

  intersectionInfo = intersectionInfo.UNION(testNode(T_inv * ray_origin, T_inv * ray_dir));
  intersectionInfo.TRANSFORM_UP(T, T_invtrans);
  return intersectionInfo;
}

void CSGNode::setCSGChildren(SceneNode *left, SceneNode *right, const string &action) {
  m_left = left;
  m_right = right;

  if (action == "UNION") {
    m_action = CSGAction::UNION;
  } else if (action == "INTERSECT") {
    m_action = CSGAction::INTERSECT;
  } else if (action == "DIFFERENCE") {
    m_action = CSGAction::DIFFERENCE;
  } else {
    cout <<  "Unrecognized CSG operation." << endl;
    throw "Unrecognized CSG operation.";
  }
}

