#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:
	JointNode(const std::string & name);
	virtual ~JointNode();

	void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);
  void set_joint_z(double min, double init, double max);

  void rotate_joint(double diff);
  void rotate_z(double diff);

  const glm::mat4 get_joint_transform() const;

	struct JointRange {
		double min, init, max;
	};

  double current_x_rot, current_y_rot, current_z_rot;
	JointRange m_joint_x, m_joint_y, m_joint_z;
};
