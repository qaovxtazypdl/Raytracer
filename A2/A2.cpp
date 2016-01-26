#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.reserve(kMaxVertices);
	colours.reserve(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f)),
    m_buttonsDown(0),
    m_prevMouseX(0),
    m_currentMode('R'),
    m_width(768),
    M(mat4(0.5f)),
    V(mat4(1.0f)),
    P(mat4(1.0f)),
    MGnomon(mat4(0.5f))
{
  M[3][3] = 1.0f;
  MGnomon[3][3] = 1.0f;
}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

void A2::reset() {
  M = mat4(0.5f); M[3][3] = 1.0f;
  V = mat4(1.0f);
  P = mat4(1.0f);
  MGnomon = mat4(0.5f); MGnomon[3][3] = 1.0f;
  m_currentMode = 'R';
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);


	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & v0,   // Line Start (NDC coordinate)
		const glm::vec2 & v1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = v0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = v1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

//mat[col][row]
glm::mat4 A2::translate(const glm::vec3 &positionOffset) {
  glm::mat4 T = glm::mat4( 1.0 );
  for(int i = 0; i < 3; i++) T[3][i] = positionOffset[i];

  return T;
}

void printMatrix(const mat4 &m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cout << m[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

//scaling factors in x,y,z directions.
glm::mat4 A2::scale(const glm::vec3 &scalingFactor) {
  glm::mat4 T = glm::mat4( 1.0 );
  for(int i = 0; i < 3; i++) T[i][i] = scalingFactor[i];

  return T;
}

// rotation angle vector is x radians of rotation in x,y,z respectively.
glm::mat4 A2::rotate(const glm::vec3 &rotationAngle) {
  // ALWAYS do x rotation, then y rotation, then z rotation.
  glm::mat4 Tx = glm::mat4( 1.0 );
  glm::mat4 Ty = glm::mat4( 1.0 );
  glm::mat4 Tz = glm::mat4( 1.0 );
  float angle;

  // x rotation
  angle = rotationAngle[0];
  Tx[1][1] = cos(angle);
  Tx[2][1] = -sin(angle);
  Tx[1][2] = sin(angle);
  Tx[2][2] = cos(angle);

  // y rotation
  angle = rotationAngle[1];
  Ty[0][0] = cos(angle);
  Ty[2][0] = sin(angle);
  Ty[0][2] = -sin(angle);
  Ty[2][2] = cos(angle);

  // z rotation
  angle = rotationAngle[2];
  Tz[0][0] = cos(angle);
  Tz[1][0] = -sin(angle);
  Tz[0][1] = sin(angle);
  Tz[1][1] = cos(angle);

  return Tz * Ty * Tx;
}

void A2::drawEdge(const glm::vec4 &v1, const glm::vec4 &v2) {
  drawLine(vec2(v1[0], v1[1]), vec2(v2[0], v2[1]));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

  vector<glm::vec4> cubeVerts;
  cubeVerts.push_back(vec4(1, 1, 1, 1));
  cubeVerts.push_back(vec4(1, 1, -1, 1));
  cubeVerts.push_back(vec4(1, -1, -1, 1));
  cubeVerts.push_back(vec4(1, -1, 1, 1));
  cubeVerts.push_back(vec4(-1, 1, 1, 1));
  cubeVerts.push_back(vec4(-1, 1, -1, 1));
  cubeVerts.push_back(vec4(-1, -1, -1, 1));
  cubeVerts.push_back(vec4(-1, -1, 1, 1));

  for (int i = 0; i < cubeVerts.size(); i++) {
    cubeVerts[i] = P * V * M * cubeVerts[i];
  }

	setLineColour(vec3(1.0f, 0.7f, 0.8f));
  drawEdge(cubeVerts[0], cubeVerts[1]);
  drawEdge(cubeVerts[1], cubeVerts[2]);
  drawEdge(cubeVerts[2], cubeVerts[3]);
  drawEdge(cubeVerts[3], cubeVerts[0]);
  drawEdge(cubeVerts[4], cubeVerts[5]);
  drawEdge(cubeVerts[5], cubeVerts[6]);
  drawEdge(cubeVerts[6], cubeVerts[7]);
  drawEdge(cubeVerts[7], cubeVerts[4]);
  drawEdge(cubeVerts[0], cubeVerts[4]);
  drawEdge(cubeVerts[1], cubeVerts[5]);
  drawEdge(cubeVerts[2], cubeVerts[6]);
  drawEdge(cubeVerts[3], cubeVerts[7]);

  //draw gnomons
  vector<glm::vec4> gnomon;
  gnomon.push_back(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  gnomon.push_back(vec4(0.25f, 0.0f, 0.0f, 1.0f));
  gnomon.push_back(vec4(0.0f, 0.25f, 0.0f, 1.0f));
  gnomon.push_back(vec4(0.0f, 0.0f, 0.25f, 1.0f));

  setLineColour(vec3(1.0f, 0.0f, 0.0f));
  drawEdge(P * V * MGnomon * gnomon[0], P * V * MGnomon * gnomon[1]);
  drawEdge(P * V * gnomon[0], P * V * gnomon[1]);
  setLineColour(vec3(0.0f, 1.0f, 0.0f));
  drawEdge(P * V * MGnomon * gnomon[0], P * V * MGnomon * gnomon[2]);
  drawEdge(P * V * gnomon[0], P * V * gnomon[2]);
  setLineColour(vec3(0.0f, 0.0f, 1.0f));
  drawEdge(P * V * MGnomon * gnomon[0], P * V * MGnomon * gnomon[3]);
  drawEdge(P * V * gnomon[0], P * V * gnomon[3]);
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
    if( ImGui::Button( "Reset" ) ) {
      reset();
    }

    vector<char> modes = {'O', 'N', 'P', 'R', 'T', 'S', 'V'};
    for (auto it = modes.begin(); it != modes.end(); ++it) {
      ImGui::PushID(*it);
      ImGui::Text("%c", *it);
      ImGui::SameLine();
      if( ImGui::RadioButton( "##Col", (int *)&m_currentMode, (int)*it ) ) {
        //
      }
      ImGui::PopID();
    }

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

void A2::handleMouseMove(int buttonsDown, double movement) {
  const float PI = 3.14159265f;
  //m_currentMode is the current
  if (m_currentMode == 'T') {
    //TRANSLATE
    const float SCALE = 2.0f / m_width;
    const float diff = SCALE * movement;
    vec3 vTranslate = vec3(
      (buttonsDown & 0x1) ? diff : 0.0f,
      (buttonsDown & 0x4) ? diff : 0.0f,
      (buttonsDown & 0x2) ? diff : 0.0f);
    M = M * translate(vTranslate);
    MGnomon = MGnomon * translate(vTranslate);
  } else if (m_currentMode == 'R') {
    //ROTATE
    const float SCALE = 2 * PI / m_width;
    const float diff = SCALE * movement;
    vec3 vRotate = vec3(
      (buttonsDown & 0x1) ? diff : 0.0f,
      (buttonsDown & 0x4) ? diff : 0.0f,
      (buttonsDown & 0x2) ? diff : 0.0f);
    M = M * rotate(vRotate);
    MGnomon = MGnomon * rotate(vRotate);
  } else if (m_currentMode == 'S') {
    //SCALE
    //TODO - cannot scale below certain amount??
    const float SCALE = 3.0f/m_width;
    const float diff = 1.0f + SCALE * movement;
    vec3 vScale = vec3(
      (buttonsDown & 0x1) ? diff : 1.0f,
      (buttonsDown & 0x4) ? diff : 1.0f,
      (buttonsDown & 0x2) ? diff : 1.0f);
    M = M * scale(vScale);
  } else if (m_currentMode == 'O') {
    //ROTATE_VIEW
    const float SCALE = 2 * PI / m_width;
    const float diff = SCALE * movement;
    vec3 vRotate = vec3(
      (buttonsDown & 0x1) ? diff : 0.0f,
      (buttonsDown & 0x4) ? diff : 0.0f,
      (buttonsDown & 0x2) ? diff : 0.0f);
    V = inverse(rotate(vRotate)) * V;
  } else if (m_currentMode == 'N') {
    //TRANSLATE_VIEW
    const float SCALE = 2.0f / m_width;
    const float diff = SCALE * movement;
    vec3 vTranslate = vec3(
      (buttonsDown & 0x1) ? diff : 0.0f,
      (buttonsDown & 0x4) ? diff : 0.0f,
      (buttonsDown & 0x2) ? diff : 0.0f);
    V = inverse(translate(vTranslate)) * V;
  }
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
    // Put some code here to handle rotations.  Probably need to
    // check whether we're *dragging*, not just moving the mouse.
    // Probably need some instance variables to track the current
    // rotation amount, and maybe the previous X position (so
    // that you can rotate relative to the *change* in X.
    if (m_buttonsDown) {
      handleMouseMove(m_buttonsDown, xPos - m_prevMouseX);
      eventHandled = true;
    }

    m_prevMouseX = xPos;
  }

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

  if (actions == 0) {
    //release
    m_buttonsDown &= ~(0x1 << button);
    eventHandled = true;
  }

  if (!ImGui::IsMouseHoveringAnyWindow()) {
    // The user clicked in the window.  If it's the left
    // mouse button, initiate a rotation.
    if (actions == 1) {
      //hold
      m_buttonsDown |= (0x1 << button);
      eventHandled = true;
    }
  }

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

  m_width = width;

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_R || key == GLFW_KEY_T || key == GLFW_KEY_S || key == GLFW_KEY_O || key == GLFW_KEY_N) {
      m_currentMode = key;
      eventHandled = true;
    }
    if (key == GLFW_KEY_Q) {
      glfwSetWindowShouldClose(m_window, GL_TRUE);
      eventHandled = true;
    }
    if (key == GLFW_KEY_A) {
      reset();
      eventHandled = true;
    }
  }

	return eventHandled;
}
