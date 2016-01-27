#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
#include <algorithm>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

const float PI = 3.14159265f;

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
    m_prevMouseY(0),
    m_currentMode('R'),
    m_width(768),
    m_height(768),
    M(mat4(1.0f)),
    V(mat4(1.0f)),
    P(mat4(1.0f)),
    WindowToViewport(mat3(1.0f)),
    m_near(0.75f),
    m_far(500.0f),
    m_fov(45.0f),
    m_vp_left(768 * 0.05),
    m_vp_right(768 - 768 * 0.05),
    m_vp_top(768 * 0.05),
    m_vp_bottom(768 - 768 * 0.05),
    m_new_viewport_start(),
    modes({
      {'O', "Rotate View"},
      {'N', "Translate View"},
      {'P', "Perspective"},
      {'R', "Rotate Model"},
      {'T', "Translate Model"},
      {'S', "Scale Model"},
      {'V', "Viewport"}
    })
{
  M = M * scale(vec3(0.15, 0.15, 0.15));
  P = perspective(m_fov, m_near, m_far);
  resizeViewport(pair<double, double>(m_vp_left, m_vp_top), pair<double, double>(m_vp_right, m_vp_bottom));
}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

void A2::reset() {
  M = mat4(1.0f) * scale(vec3(0.15, 0.15, 0.15));
  V = mat4(1.0f);

  m_currentMode = 'R';
  m_near = 0.75f;
  m_far = 500.0f;
  m_fov = 45.0f;

  P = perspective(m_fov, m_near, m_far);

  m_vp_left = m_width * 0.05;
  m_vp_right = m_width - m_width * 0.05;
  m_vp_top = m_height * 0.05;
  m_vp_bottom = m_height - m_height * 0.05;

  resizeViewport(pair<double, double>(m_vp_left, m_vp_top), pair<double, double>(m_vp_right, m_vp_bottom));
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

glm::mat4 A2::perspective(float fov, float n, float f) {
  const float aspect = (float)m_width / m_height;
  glm::mat4 P = glm::mat4( 0.0 );

  P[0][0] = (1/tan((fov/2) * PI / 180)) / aspect;
  P[1][1] = 1/tan((fov/2) * PI / 180);
  //P[2][2] = (n + f) / n;
  //P[3][2] = -f;
  //P[2][3] = 1/n;

  P[2][2] = -(f+n)/(f-n);
  P[3][2] = -2*f*n/(f-n);
  P[2][3] = -1;

  return P;
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

//mat[col][row]
glm::mat3 A2::translate(const glm::vec2 &positionOffset) {
  glm::mat3 T = glm::mat3( 1.0 );
  for(int i = 0; i < 2; i++) T[2][i] = positionOffset[i];

  return T;
}

//scaling factors in x,y,z directions.
glm::mat3 A2::scale(const glm::vec2 &scalingFactor) {
  glm::mat3 T = glm::mat3( 1.0 );
  for(int i = 0; i < 2; i++) T[i][i] = scalingFactor[i];

  return T;
}

//mat[col][row]
glm::mat4 A2::translate(const glm::vec3 &positionOffset) {
  glm::mat4 T = glm::mat4( 1.0 );
  for(int i = 0; i < 3; i++) T[3][i] = positionOffset[i];

  return T;
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
  vec3 z_mapped_v1 = WindowToViewport * vec3(v1[0] * m_near / v1[2], v1[1] * m_near / v1[2], 1);
  vec3 z_mapped_v2 = WindowToViewport * vec3(v2[0] * m_near / v2[2], v2[1] * m_near / v2[2], 1);
  drawLine(vec2(z_mapped_v1[0], z_mapped_v1[1]), vec2(z_mapped_v2[0], z_mapped_v2[1]));
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

  vector<glm::vec4> verts;
  verts.push_back(vec4(1, 1, 1, 1));
  verts.push_back(vec4(1, 1, -1, 1));
  verts.push_back(vec4(1, -1, -1, 1));
  verts.push_back(vec4(1, -1, 1, 1));
  verts.push_back(vec4(-1, 1, 1, 1));
  verts.push_back(vec4(-1, 1, -1, 1));
  verts.push_back(vec4(-1, -1, -1, 1));
  verts.push_back(vec4(-1, -1, 1, 1));

  // add model gnomon
  verts.push_back(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  verts.push_back(vec4(0.25f, 0.0f, 0.0f, 0.0f) *= 0.25f / length(M * vec4(0.25f, 0.0f, 0.0f, 0.0f))); verts[9][3] = 1.0f;
  verts.push_back(vec4(0.0f, 0.25f, 0.0f, 0.0f) *= 0.25f / length(M * vec4(0.0f, 0.25f, 0.0f, 0.0f))); verts[10][3] = 1.0f;
  verts.push_back(vec4(0.0f, 0.0f, 0.25f, 0.0f) *= 0.25f / length(M * vec4(0.0f, 0.0f, 0.25f, 0.0f))); verts[11][3] = 1.0f;

  // apply model transforms
  for (int i = 0; i < verts.size(); i++) {
    verts[i] = M * verts[i];
  } //model coordinates

  // add world gnomon
  verts.push_back(vec4(0.0f, 0.0f, 0.0f, 1.0f));
  verts.push_back(vec4(0.25f, 0.0f, 0.0f, 1.0f));
  verts.push_back(vec4(0.0f, 0.25f, 0.0f, 1.0f));
  verts.push_back(vec4(0.0f, 0.0f, 0.25f, 1.0f));

  // apply view transforms
  for (int i = 0; i < verts.size(); i++) {
    verts[i] = V * verts[i];
  } //view coordinates

  // clip near plane before applying projection
  // probably need to turn this into a list of edges by this point.
  for (int i = 0; i < verts.size(); i++) {
    verts[i] = P * verts[i];
  } //world coordinates

  //drawEdge draws device coords
	setLineColour(vec3(1.0f, 0.7f, 0.8f));
  drawEdge(verts[0], verts[1]);
  drawEdge(verts[1], verts[2]);
  drawEdge(verts[2], verts[3]);
  drawEdge(verts[3], verts[0]);
  drawEdge(verts[4], verts[5]);
  drawEdge(verts[5], verts[6]);
  drawEdge(verts[6], verts[7]);
  drawEdge(verts[7], verts[4]);
  drawEdge(verts[0], verts[4]);
  drawEdge(verts[1], verts[5]);
  drawEdge(verts[2], verts[6]);
  drawEdge(verts[3], verts[7]);

  //draw gnomons
  setLineColour(vec3(1.0f, 0.0f, 0.0f));
  drawEdge(verts[(8) + 0], verts[(8) + 1]);
  drawEdge(verts[(8+4) + 0], verts[(8+4) + 1]);
  setLineColour(vec3(0.0f, 1.0f, 0.0f));
  drawEdge(verts[(8) + 0], verts[(8) + 2]);
  drawEdge(verts[(8+4) + 0], verts[(8+4) + 2]);
  setLineColour(vec3(0.0f, 0.0f, 1.0f));
  drawEdge(verts[(8) + 0], verts[(8) + 3]);
  drawEdge(verts[(8+4) + 0], verts[(8+4) + 3]);

  //draw viewport lines
  drawLine(vec2(m_vp_left, m_vp_top), vec2(m_vp_right, m_vp_top));
  drawLine(vec2(m_vp_right, m_vp_top), vec2(m_vp_right, m_vp_bottom));
  drawLine(vec2(m_vp_right, m_vp_bottom), vec2(m_vp_left, m_vp_bottom));
  drawLine(vec2(m_vp_left, m_vp_bottom), vec2(m_vp_left, m_vp_top));
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

    for (auto it = modes.begin(); it != modes.end(); ++it) {
      ImGui::PushID(it->first);
      ImGui::Text("%c - %16s", it->first, it->second.c_str());
      ImGui::SameLine();
      if( ImGui::RadioButton( "##Col", (int *)&m_currentMode, (int)it->first ) ) {
        //
      }
      ImGui::PopID();
    }

    ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
    ImGui::Text( "Near Plane: %.3f", m_near);
    ImGui::Text( "Far Plane: %.3f", m_far);
    ImGui::Text( "FOV (degrees): %.2f", m_fov);
    ImGui::Text( "L: %f, R:%f", m_vp_left, m_vp_right);
    ImGui::Text( "T:%f, B:%f", m_vp_top, m_vp_bottom);

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

void A2::resizeViewport(const std::pair<double, double> &start, const std::pair<double, double> &end) {
  double startX = start.first < 0 ? 0 : start.first > m_width ? m_width : start.first;
  double startY = start.second < 0 ? 0 : start.second > m_height ? m_height : start.second;
  double endX = end.first < 0 ? 0 : end.first > m_width ? m_width : end.first;
  double endY = end.second < 0 ? 0 : end.second > m_height ? m_height : end.second;

  // convert to NDC
  m_vp_left = std::min(startX, endX) / m_width * 2 - 1;
  m_vp_right = std::max(startX, endX) / m_width * 2 - 1;
  m_vp_top = std::min(startY, endY) / m_height * -2 + 1;
  m_vp_bottom = std::max(startY, endY) / m_height * -2 + 1;

  //reconstruct the scaling/translation matrix from NDC to viewport
  //resize x: 2->(right-left) => factor of (right-left)/2
  //resize y: 2->(top-bottom) => factor of (top-bottom)/2

  //translate x 0->(right+left)/2
  //translate y 0->(top+bottom)/2
  WindowToViewport =
    translate(vec2((m_vp_right+m_vp_left)/2, (m_vp_top+m_vp_bottom)/2)) *
    scale(vec2((m_vp_right-m_vp_left)/2, (m_vp_top-m_vp_bottom)/2)) *
    mat3(1.0f);
}

void A2::handleMouseMove(int buttonsDown, double xPos, double yPos) {
  double movement = xPos - m_prevMouseX;
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
  } else if (m_currentMode == 'R') {
    //ROTATE
    const float SCALE = 2 * PI / m_width;
    const float diff = SCALE * movement;
    vec3 vRotate = vec3(
      (buttonsDown & 0x1) ? diff : 0.0f,
      (buttonsDown & 0x4) ? diff : 0.0f,
      (buttonsDown & 0x2) ? diff : 0.0f);
    M = M * rotate(vRotate);
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
  } else if (m_currentMode == 'P') {
    //PROJECTION
    const float SCALE = 2.0f / m_width;
    const float diff = SCALE * movement;

    if (buttonsDown & 0x1) m_fov += 25 * diff; //fov zooms in faster - 50 per screen
    if (buttonsDown & 0x4) m_near += diff;
    if (buttonsDown & 0x2) m_far += diff;

    if (m_fov < 5) m_fov = 5;
    if (m_fov > 160) m_fov = 160;
    P = perspective(m_fov, m_near, m_far);
  } else if (m_currentMode == 'V') {
    if (buttonsDown & 0x1) {
      //update viewport if position is not equal to starting posn
      pair<double, double> currentLoc(xPos, yPos);
      if (m_new_viewport_start != currentLoc) {
        resizeViewport(m_new_viewport_start, currentLoc);
      }
    }
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
      handleMouseMove(m_buttonsDown, xPos, yPos);
      eventHandled = true;
    }

    m_prevMouseX = xPos;
    m_prevMouseY = yPos;
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
      if (m_currentMode == 'V' && button == 0) {
        // record current point as starting pt
        m_new_viewport_start.first = m_prevMouseX;
        m_new_viewport_start.second = m_prevMouseY;
      }
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
  m_height = height;

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
    if (key == GLFW_KEY_R ||
      key == GLFW_KEY_T ||
      key == GLFW_KEY_S ||
      key == GLFW_KEY_O ||
      key == GLFW_KEY_N ||
      key == GLFW_KEY_P ||
      key == GLFW_KEY_V) {
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
