/**
* @author	Vadim Petrov
* @date		2016
* @brief	Basic defines and data structures.
*/

#ifndef _REJFPANK_DATA_HPP
#define _REJFPANK_DATA_HPP

#define MODEL_LEGO "../res/lego.obj"

#define MIDI_LONG_BUFFER_SIZE 1024
#define SELECT_MIDI_PORT_MAN false // for testing purposes
#define SELECT_MONITOR_MAN false // for testing purposes

#define INIT_WIN_WIDTH 848 ///< Width to height ratio is 16:9
#define INIT_WIN_HEIGHT 480 ///< Width to height ratio is 16:9
#define WIN_TITLE "rejfpank visuals"

#define CAMERA_INIT_POS glm::vec3(0.0f, 0.0f, 5.0f)
#define CAMERA_INIT_DIR glm::vec3(0.0f, 0.0f, -1.0f)
#define CAMERA_INIT_UP  glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_VIEW_ANGLE 90.0f
#define CAMERA_VIEW_START 0.01f
#define CAMERA_VIEW_DIST 350.0f

#define MOUSE_SENSITIVITY 0.01f
#define VIEW_ANGLE_DELTA 0.03f ///< in degrees
#define STEP_LENGTH 0.2f

/// controls in controlState
enum {
	CTRL_FULLSCREEN,
	CTRL_COUNT
};

/// keys in controlState
enum {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	KEY_W, KEY_S, KEY_A, KEY_D, KEY_Q, KEY_E,
	KEY_COUNT
};

const unsigned int cubeNVertices = 8;
const unsigned int cubeNAttribsPerVertex = 6;
const float cubeVertices[cubeNVertices * cubeNAttribsPerVertex] = {
	// x      y      z       nx     ny    nz
	-1.0f, -1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f
};

const unsigned int cubeNTriangles = 36;
const unsigned short cubeTriangles[cubeNTriangles] = {
	0, 2, 4,   4, 2, 6, // back face
	0, 1, 2,   2, 1, 3, // left face
	1, 5, 3,   3, 5, 7, // front face
	5, 4, 7,   7, 4, 6, // right face
	3, 7, 2,   2, 7, 6, // upper face
	0, 4, 1,   1, 4, 5, // lower face
};

#endif // !_REJFPANK_DATA_HPP