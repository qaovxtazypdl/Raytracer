#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <utility>

#include "grid.hpp"

class A1 : public CS488Window {
public:
	A1();
	virtual ~A1();

	static const int DEFAULT_WIDTH = 1024;
	static const int DEFAULT_HEIGHT = 768;

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

private:
	void initGrid();
	void initCubes();

	void drawCubes(unsigned i);
	void drawCubeOutlines(unsigned i);
	void drawActiveCubeStack();
	void writeUnitCubeVerticesIntoBuffer(float *verts, unsigned *indices, size_t &start, size_t &idxStart, float x, float y, float z);
	void writeUnitCubeOutlineIntoBuffer(float *verts, unsigned *indices, size_t &start, size_t &idxStart, float x, float y, float z, bool isActiveOutline);
	void writeBaseOutlineIntoBuffer(float *verts, unsigned *indices, size_t &start, size_t &idxStart, float x, float z);

	void setColour(unsigned index);
  void setColour(float r, float g, float b);
	void setColour(float r, float g, float b, float a);

	// manipulation of data and logic
	void growCurrentSelectedCubeStack();
	void shrinkCurrentSelectedCubeStack();
	void moveFocusLeft(bool shiftHeld);
	void moveFocusRight(bool shiftHeld);
	void moveFocusUp(bool shiftHeld);
	void moveFocusDown(bool shiftHeld);
	void setCurrentColour(unsigned i, unsigned j);
	void resetToDefault();

	// Fields related to the current state of the grid
	static const size_t DIM = 16;
	Grid grid;
	std::pair<unsigned, unsigned> focusLocation;

	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	GLint P_uni; // Uniform location for Projection matrix.
	GLint V_uni; // Uniform location for View matrix.
	GLint M_uni; // Uniform location for Model matrix.

	// Fields related to grid geometry.
	GLuint m_grid_vao;
	GLuint m_grid_vbo;

	// Fields related to cube geometries.
	GLuint m_cubes_vao[DIM * DIM];
	GLuint m_cubes_vbo[DIM * DIM];
	GLuint m_cubes_element_vbo[DIM * DIM];

	GLuint m_cube_edges_vao[DIM * DIM];
	GLuint m_cube_edges_vbo[DIM * DIM];
	GLuint m_cube_edges_element_vbo[DIM * DIM];

	GLuint m_cube_active_vao;
	GLuint m_cube_active_vbo;
	GLuint m_cube_active_element_vbo;

	// Matrices controlling the camera and projection.
	glm::mat4 proj;
	glm::mat4 view;

	float colour[8][3];
	int current_col;
	int width;

	double prevMouseX, prevMouseY;
	bool leftMouseButtonDown;
	float current_rotationX;
	float current_rotationY;
	float scaling_factor;
};
