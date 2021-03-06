#include "SceneNode.hpp"


#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;


// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	m_nodeType(NodeType::SceneNode),
	trans(dmat4()),
	invtrans(dmat4()),
	m_nodeId(nodeInstanceCount++),
  m_boundingObject(NULL)
{

}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode & other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans),
    m_boundingObject(NULL)
{
	for(SceneNode * child : other.children) {
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
	for(SceneNode * child : children) {
		delete child;
	}
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::dmat4& m) {
	trans = m;
	invtrans = glm::inverse(m);
  invtrans_transpose = glm::transpose(glm::inverse(glm::dmat3(m)));
}

//---------------------------------------------------------------------------------------
const glm::dmat4& SceneNode::get_transform() const {
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::dmat4& SceneNode::get_inverse() const {
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
	children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child) {
	children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
	        break;
		case 'z':
			rot_axis = vec3(0,0,1);
	        break;
		default:
			break;
	}
	dmat4 rot_matrix = glm::rotate((float)(angle / 180.0 * PI), rot_axis);
	set_transform( rot_matrix * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 & amount) {
	set_transform( dmat4(glm::scale(amount)) * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount) {
	set_transform( dmat4(glm::translate(amount)) * trans );
}


//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
	return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream & operator << (std::ostream & os, const SceneNode & node) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType) {
		case NodeType::SceneNode:
			os << "SceneNode";
			break;
		case NodeType::GeometryNode:
			os << "GeometryNode";
			break;
		case NodeType::JointNode:
			os << "JointNode";
			break;
    case NodeType::CSGNode:
      os << "CSGNode";
      break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}

IntersectionInfo SceneNode::testNode(const dvec4 &ray_origin, const dvec4 &ray_dir) const {
  return testHit(ray_origin, ray_dir);
}

IntersectionInfo SceneNode::testHit(const dvec4 &ray_origin, const dvec4 &ray_dir) const {
  IntersectionInfo intersectionInfo;
  dmat4 T = trans;
  dmat4 T_inv = invtrans;
  dmat3 T_invtrans = invtrans_transpose;

  if (m_boundingObject != NULL) {
    if (MACRO_RENDER_BOUNDING_BOX) {
      return m_boundingObject->testHit(T_inv * ray_origin, T_inv * ray_dir);
    } else {
      IntersectionPoint bounding = m_boundingObject->testHit(T_inv * ray_origin, T_inv * ray_dir).getFirstValidIntersection(std::numeric_limits<double>::infinity());
      if (!bounding.valid) {
        return intersectionInfo;
      }
    }
  }

  for (SceneNode * node : children) {
    intersectionInfo = intersectionInfo.UNION(node->testHit(T_inv * ray_origin, T_inv * ray_dir));
  }

  intersectionInfo.TRANSFORM_UP(T, T_invtrans);

  return intersectionInfo;
}

void SceneNode::set_bounding_object(SceneNode* bound) {
  m_boundingObject = bound;
}

