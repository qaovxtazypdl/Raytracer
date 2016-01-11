#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col(0), totalCubes(0)
{
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			heightMap[i][j] = 0;
		}
	}
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();
	initCubes();

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt(
		glm::vec3( 0.0f, float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );
	proj = glm::perspective(
		glm::radians( 45.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
}

void A1::initCubes() {
	// Create vertex objects for cubes
	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_cubes_vao );
	glBindVertexArray( m_cubes_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_cubes_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_cubes_vbo );

	// Create the cube vertex element buffer
	glGenBuffers( 1, &m_cubes_element_vbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_cubes_element_vbo );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Create vertex objects for outlines
	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_cube_edges_vao );
	glBindVertexArray( m_cube_edges_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_cube_edges_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_cube_edges_vbo );

  // Create the cube vertex element buffer
	glGenBuffers( 1, &m_cube_edges_element_vbo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_cube_edges_element_vbo );

	// Specify the means of extracting the position values properly.
	posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my*
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	CHECK_GL_ERRORS;
}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my*
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	// Place per frame, application logic here ...
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.

		ImGui::PushID( 0 );
		ImGui::ColorEdit3( "##Colour", colour );
		ImGui::SameLine();
		if( ImGui::RadioButton( "##Col", &current_col, 0 ) ) {
			// Select this colour.
		}
		ImGui::PopID();

/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );

	m_shader.enable();
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );

		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );
		glUniform3f( col_uni, 1, 1, 1 );
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );

		// Draw the cubes and highlight outline if active
		drawCubes();
		drawCubeOutlines();

	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

// x, y, z are the bottom left front corner.
void A1::writeUnitCubeVerticesIntoBuffer(float *verts, unsigned *indices, size_t &start, size_t &idxStart, int x, int y, int z) {
	size_t startIdx = start / 3; // divide by 3 because 3 entries is one index location.

	// "front" vertices
	verts[start++] = x + 1; verts[start++] = y + 0; verts[start++] = z + 0;
	verts[start++] = x + 1; verts[start++] = y + 1; verts[start++] = z + 0;
	verts[start++] = x + 1; verts[start++] = y + 1; verts[start++] = z + 1;
	verts[start++] = x + 1; verts[start++] = y + 0; verts[start++] = z + 1;

	// "back" vertices
	verts[start++] = x + 0; verts[start++] = y + 0; verts[start++] = z + 0;
	verts[start++] = x + 0; verts[start++] = y + 1; verts[start++] = z + 0;
	verts[start++] = x + 0; verts[start++] = y + 1; verts[start++] = z + 1;
	verts[start++] = x + 0; verts[start++] = y + 0; verts[start++] = z + 1;

  // front face index
	indices[idxStart++] = startIdx + 0; indices[idxStart++] = startIdx + 1; indices[idxStart++] = startIdx + 2;
	indices[idxStart++] = startIdx + 0; indices[idxStart++] = startIdx + 2; indices[idxStart++] = startIdx + 3;

	// right face index
	indices[idxStart++] = startIdx + 1; indices[idxStart++] = startIdx + 5; indices[idxStart++] = startIdx + 6;
	indices[idxStart++] = startIdx + 1; indices[idxStart++] = startIdx + 6; indices[idxStart++] = startIdx + 2;

	// left face index
	indices[idxStart++] = startIdx + 4; indices[idxStart++] = startIdx + 0; indices[idxStart++] = startIdx + 3;
	indices[idxStart++] = startIdx + 4; indices[idxStart++] = startIdx + 3; indices[idxStart++] = startIdx + 7;

	// top face index
	indices[idxStart++] = startIdx + 3; indices[idxStart++] = startIdx + 2; indices[idxStart++] = startIdx + 6;
	indices[idxStart++] = startIdx + 3; indices[idxStart++] = startIdx + 6; indices[idxStart++] = startIdx + 7;

	// bottom face index
	indices[idxStart++] = startIdx + 4; indices[idxStart++] = startIdx + 5; indices[idxStart++] = startIdx + 1;
	indices[idxStart++] = startIdx + 4; indices[idxStart++] = startIdx + 1; indices[idxStart++] = startIdx + 0;

	// back face index
	indices[idxStart++] = startIdx + 5; indices[idxStart++] = startIdx + 4; indices[idxStart++] = startIdx + 7;
	indices[idxStart++] = startIdx + 5; indices[idxStart++] = startIdx + 7; indices[idxStart++] = startIdx + 6;
}

void A1::writeUnitCubeOutlineIntoBuffer(float *verts, unsigned *indices, size_t &start, size_t &idxStart, int x, int y, int z) {
	size_t startIdx = start / 3; // divide by 3 because 3 entries is one index location.

	// "front" vertices
	verts[start++] = x + 1; verts[start++] = y + 0; verts[start++] = z + 0;
	verts[start++] = x + 1; verts[start++] = y + 1; verts[start++] = z + 0;
	verts[start++] = x + 1; verts[start++] = y + 1; verts[start++] = z + 1;
	verts[start++] = x + 1; verts[start++] = y + 0; verts[start++] = z + 1;

	// "back" vertices
	verts[start++] = x + 0; verts[start++] = y + 0; verts[start++] = z + 0;
	verts[start++] = x + 0; verts[start++] = y + 1; verts[start++] = z + 0;
	verts[start++] = x + 0; verts[start++] = y + 1; verts[start++] = z + 1;
	verts[start++] = x + 0; verts[start++] = y + 0; verts[start++] = z + 1;

  // front edges
	indices[idxStart++] = startIdx + 0; indices[idxStart++] = startIdx + 1;
	indices[idxStart++] = startIdx + 1; indices[idxStart++] = startIdx + 2;
	indices[idxStart++] = startIdx + 2; indices[idxStart++] = startIdx + 3;
	indices[idxStart++] = startIdx + 3; indices[idxStart++] = startIdx + 0;

  // back edges
	indices[idxStart++] = startIdx + 4; indices[idxStart++] = startIdx + 5;
	indices[idxStart++] = startIdx + 5; indices[idxStart++] = startIdx + 6;
	indices[idxStart++] = startIdx + 6; indices[idxStart++] = startIdx + 7;
	indices[idxStart++] = startIdx + 7; indices[idxStart++] = startIdx + 4;

  // side edges
	indices[idxStart++] = startIdx + 0; indices[idxStart++] = startIdx + 4;
	indices[idxStart++] = startIdx + 1; indices[idxStart++] = startIdx + 5;
	indices[idxStart++] = startIdx + 2; indices[idxStart++] = startIdx + 6;
	indices[idxStart++] = startIdx + 3; indices[idxStart++] = startIdx + 7;
}

void A1::drawCubes()
{
	size_t sz = totalCubes * 8 * 3;  //8 vertices / square, 3 dimensions / vertex
	size_t indexSz = totalCubes * 6 * 2 * 3;  // 6 faces / square, 2 triangles / square, 3 indices / triangle

	float *verts = new float[ sz ];
	unsigned *indices = new unsigned[ indexSz ];
	size_t vertStart = 0, idxStart = 0;

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int height = 0; height < heightMap[i][j]; height++) {
				writeUnitCubeVerticesIntoBuffer(verts, indices, vertStart, idxStart, i, height, j);
			}
		}
	}

	glBindVertexArray( m_cubes_vao );

	glBindBuffer( GL_ARRAY_BUFFER, m_cubes_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz * sizeof(float), verts, GL_DYNAMIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_cubes_element_vbo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexSz * sizeof(unsigned), indices, GL_DYNAMIC_DRAW );

	glDrawElements(	GL_TRIANGLES, indexSz, GL_UNSIGNED_INT, 0);

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;
	delete [] indices;
}

void A1::drawCubeOutlines()
{
	size_t sz = totalCubes * 8 * 3;  //8 vertices / square, 3 dimensions / vertex
	size_t indexSz = totalCubes * 12 * 2;  // 12 edges / square, 2 points per edge

	float *verts = new float[ sz ];
	unsigned *indices = new unsigned[ indexSz ]; // 1 square, 6 faces / square, 2 triangles / square, 3 indices / triangle
	size_t vertStart = 0, idxStart = 0;

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			for (int height = 0; height < heightMap[i][j]; height++) {
				writeUnitCubeOutlineIntoBuffer(verts, indices, vertStart, idxStart, i, height, j);
			}
		}
	}

	glBindVertexArray( m_cube_edges_vao );

	glBindBuffer( GL_ARRAY_BUFFER, m_cube_edges_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz * sizeof(float), verts, GL_DYNAMIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_cube_edges_element_vbo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexSz * sizeof(unsigned), indices, GL_DYNAMIC_DRAW );

	GLint uniformLocation_colour = m_shader.getUniformLocation("colour");
	glUniform3f(uniformLocation_colour, 0.0f, 0.0f, 0.0f);
	glDrawElements(	GL_LINES, indexSz, GL_UNSIGNED_INT, 0);
	glUniform3f(uniformLocation_colour, 1.0f, 1.0f, 1.0f);

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;
	delete [] indices;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

void A1::growCurrentSelectedCubeStack() {
	heightMap[focusLocation.first][focusLocation.second]++;
	totalCubes++;
}

void A1::shrinkCurrentSelectedCubeStack() {
	if (heightMap[focusLocation.first][focusLocation.second] > 0) {
		heightMap[focusLocation.first][focusLocation.second]--;
		totalCubes--;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A1::mouseMoveEvent(double xPos, double yPos)
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so
		// that you can rotate relative to the *change* in X.
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_SPACE) {
			growCurrentSelectedCubeStack();
			eventHandled = true;
		}
		if (key == GLFW_KEY_BACKSPACE) {
			shrinkCurrentSelectedCubeStack();
			eventHandled = true;
		}
	}

	return eventHandled;
}
