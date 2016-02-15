#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"
#include "JointNode.hpp"
#include "GeometryNode.hpp"
#include "StateStack.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <map>

struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};


class A3 : public CS488Window {
public:
	A3(const std::string & luaSceneFile);
	virtual ~A3();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();
	void initViewMatrix();
	void initLightSources();

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(const SceneNode & node);
  void renderSceneNode(const SceneNode * root, glm::mat4 accumulatedTrans);
	void renderArcCircle();

  // my stuff
  void handleMouseMove(int buttonsDown, double xPos, double yPos);
  void processNodeHierarchy(SceneNode *parent, SceneNode *root);
  void pickJoint(double x, double y);
  void toggleFalseColorTo(bool state);

  std::vector<std::vector<double>> getCurrentJointState();
  void setCurrentJointState(std::vector<std::vector<double>> newState);
  void undoJointManipulation();
  void redoJointManipulation();

  void resetPosition();
  void resetOrientation();
  void resetJoints();
  void resetAll();

  int m_currentMode;
  int m_buttonsDown;
  double m_prevMouseX, m_prevMouseY;
  int m_width, m_height;
  bool m_isHeadSelected;
  JointNode *m_headNode;
  SceneNode *m_root_transform_node;
  bool show_gui, draw_circle, use_z_buffer, cull_back, cull_front;
  std::vector<JointNode *> m_joints;
  std::string m_undoRedoStatus;
  glm::mat4 m_puppet_posn, m_puppet_orientation;

  StateStack<std::vector<std::vector<double>>> m_states;
  // end my stuff


	glm::mat4 m_perpsective;
	glm::mat4 m_view;

	LightSource m_light;

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	ShaderProgram m_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;
};
