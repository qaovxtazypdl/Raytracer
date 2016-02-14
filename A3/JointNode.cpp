#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include "cs488-framework/MathUtils.hpp"


#include "JointNode.hpp"

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name), current_x_rot(0), current_y_rot(0)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {
}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
  current_x_rot = init;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
  current_y_rot = init;
}


const glm::mat4 JointNode::get_joint_transform() const {
  glm::mat4 rot_matrix_x = glm::rotate(degreesToRadians(current_x_rot), glm::vec3(1,0,0));
  glm::mat4 rot_matrix_y = glm::rotate(degreesToRadians(current_y_rot), glm::vec3(0,1,0));
  return rot_matrix_x * rot_matrix_y;
}
