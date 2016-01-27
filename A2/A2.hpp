#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
  VertexData();

  std::vector<glm::vec2> positions;
  std::vector<glm::vec3> colours;
  GLuint index;
  GLsizei numVertices;
};

class A2 : public CS488Window {
public:
  A2();
  virtual ~A2();

protected:
  virtual void init() override;
  virtual void appLogic() override;
  virtual void guiLogic() override;
  virtual void draw() override;
  virtual void cleanup() override;

  virtual bool cursorEnterWindowEvent(int entered) override;
  virtual bool mouseMoveEvent(double xPos, double yPos) override;
  virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
  virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
  virtual bool windowResizeEvent(int width, int height) override;
  virtual bool keyInputEvent(int key, int action, int mods) override;

  void createShaderProgram();
  void enableVertexAttribIndices();
  void generateVertexBuffers();
  void mapVboDataToVertexAttributeLocation();
  void uploadVertexDataToVbos();

  void initLineData();

  void setLineColour(const glm::vec3 & colour);

  void drawLine (const glm::vec2 & v0, const glm::vec2 & v1);

  //additions
  //2d
  glm::mat3 translate(const glm::vec2 &positionOffset);
  glm::mat3 scale(const glm::vec2 &scalingFactor);
  //3d
  glm::mat4 translate(const glm::vec3 &positionOffset);
  glm::mat4 scale(const glm::vec3 &scalingFactor);
  glm::mat4 rotate(const glm::vec3 &rotationAngle);
  glm::mat4 perspective(float fov, float n, float f);
  //misc
  void drawEdge(const glm::vec4 &v1, const glm::vec4 &v2);
  void handleMouseMove(int buttonsDown, double xPos, double yPos);
  void resizeViewport(const std::pair<double, double> &start, const std::pair<double, double> &end);
  void reset();

  ShaderProgram m_shader;

  GLuint m_vao;            // Vertex Array Object
  GLuint m_vbo_positions;  // Vertex Buffer Object
  GLuint m_vbo_colours;    // Vertex Buffer Object

  VertexData m_vertexData;

  glm::vec3 m_currentLineColour;

  glm::mat4 M,V,P;
  glm::mat3 WindowToViewport;

  float m_near, m_far, m_fov;
  int m_buttonsDown;
  int m_prevMouseX, m_prevMouseY;
  int m_width, m_height;
  char m_currentMode;
  double m_vp_left, m_vp_right, m_vp_top, m_vp_bottom;
  std::pair<double, double> m_new_viewport_start;
  const std::map<char, std::string> m_modes;
};
