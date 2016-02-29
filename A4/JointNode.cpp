#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include "cs488-framework/MathUtils.hpp"


#include "JointNode.hpp"

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
  : SceneNode(name), current_x_rot(0), current_y_rot(0), current_z_rot(0)
{
  m_nodeType = NodeType::JointNode;
  set_joint_z(0,0,0);
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

void JointNode::set_joint_z(double min, double init, double max) {
  m_joint_z.min = min;
  m_joint_z.init = init;
  m_joint_z.max = max;
  current_z_rot = init;
}

void JointNode::rotate_joint(double diff) {
  //rotates x and y
  current_x_rot += diff;
  current_y_rot += diff;

  if (current_x_rot > m_joint_x.max) current_x_rot = m_joint_x.max;
  if (current_x_rot < m_joint_x.min) current_x_rot = m_joint_x.min;

  if (current_y_rot > m_joint_y.max) current_y_rot = m_joint_y.max;
  if (current_y_rot < m_joint_y.min) current_y_rot = m_joint_y.min;
}

void JointNode::rotate_z(double diff) {
  //rotates x and y
  current_z_rot += diff;

  if (current_z_rot > m_joint_z.max) current_z_rot = m_joint_z.max;
  if (current_z_rot < m_joint_z.min) current_z_rot = m_joint_z.min;
}


const glm::dmat4 JointNode::get_joint_transform() const {
  glm::dmat4 rot_matrix_x = glm::rotate(degreesToRadians((float)current_x_rot), glm::vec3(1,0,0));
  glm::dmat4 rot_matrix_y = glm::rotate(degreesToRadians((float)current_y_rot), glm::vec3(0,1,0));
  glm::dmat4 rot_matrix_z = glm::rotate(degreesToRadians((float)current_z_rot), glm::vec3(0,0,1));
  return rot_matrix_x * rot_matrix_y * rot_matrix_z;
}
