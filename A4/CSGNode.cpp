#include <iostream>
#include <fstream>
#include <limits>

#include <glm/ext.hpp>

#include "CSGNode.hpp"

using namespace glm;
using namespace std;

CSGNode::CSGNode( const std::string& fname ): m_left(NULL), m_right(NULL), m_action(CSGAction::UNION)
{
}

void CSGNode::set_operation(SceneNode* left, SceneNode* right, CSGAction action) {
  //TODO: compute bounding box for overall CSG

  //
}


IntersectionInfo CSGNode::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, double max_t) {
    return IntersectionInfo();
}

