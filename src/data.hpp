/**
* @author	Vadim Petrov
* @date		2016
* @brief	Basic defines and data structures.
*/

#ifndef _REJFPANK_DATA_HPP
#define _REJFPANK_DATA_HPP

#include <glm/glm.hpp>
#include <string>

#define TEX_TEST "../res/test-tex.png"
#define TEX_NOISE "../res/noise.jpg"

#define IMG_TEST "../res/test-img.png"
#define IMG_SQUIRREL1 "../res/sq-hairy.png"
#define IMG_SQUIRREL2 "../res/sq-hairy-mutant.png"
#define IMG_SQUIRREL3 "../res/sq-hairless.png"
#define IMG_SQUIRREL4 "../res/sq-hairless-mutant.png"

#define MIDI_LONG_BUFFER_SIZE 1024

#define SELECT_MIDI_PORT_MAN false // for testing purposes
#define SELECT_MIDI_PORT_DEFAULT 9
#define SELECT_MONITOR_MAN false // for testing purposes
#define SELECT_MONITOR_DEFAULT 2

#define INIT_WIN_WIDTH 848 ///< Width to height ratio is 16:9
#define INIT_WIN_HEIGHT 480 ///< Width to height ratio is 16:9
#define WIN_TITLE "rejfpank visuals"

#define CAMERA_INIT_POS glm::vec3(0.0f, 0.0f, -6.0f)
#define CAMERA_INIT_DIR glm::vec3(0.0f, 0.0f, 1.0f)
#define CAMERA_INIT_UP  glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_INIT_FREE false
#define CAMERA_VIEW_ANGLE 65.0f
#define CAMERA_VIEW_START 0.01f
#define CAMERA_VIEW_DIST 350.0f
#define CAMERA_DIR_FORWARD 1
#define CAMERA_DIR_BACKWARD -1

#define MOUSE_SENSITIVITY 0.01f
#define ROTATION_ANGLE_DELTA 0.02f ///< in degrees
#define STEP_LENGTH 0.1f
#define ROTATION_SPEED 2.0f
#define ROTATION_TIME 0.2f
#define FLOW_MAX_TIME 2.3f

#define MATERIAL_GEN_AMBIENT_MULTI 0.1f
#define MATERIAL_LEGO_SPECULAR glm::vec3(0.3f)
#define MATERIAL_LEGO_SHININES 3.0f

#define BANNER_SIZE (2 * glm::tan(glm::radians(CAMERA_VIEW_ANGLE / 2.0f)))

#define SONG_COUNT 3
const std::string songNames [SONG_COUNT] = {
	"Veverka",
	"Skala",
	"Definice"
};
#define ACTIVE_SONG songNames[0] // for testing

#define LEGO_BRICKS_LOOPS 5
#define LEGO_BRICKS_DIST 5
#define MIDAS_TIME 3.0f

#define MODEL_HONEY "../res/honey-comb.obj"

// lego brick models kindly provided by Floris 'floriuszzz' Smit www.tf3dm.com/3d-model/lego-all-sizes-colors-94903.html
#define MODEL_LEGO_0 "../res/lego-brick-1x1.obj"
#define MODEL_LEGO_1 "../res/lego-brick-1x1-low.obj"
#define MODEL_LEGO_2 "../res/lego-brick-2x1.obj"
#define MODEL_LEGO_3 "../res/lego-brick-2x1-low.obj"
#define MODEL_LEGO_4 "../res/lego-brick-2x2.obj"
#define MODEL_LEGO_5 "../res/lego-brick-2x2-low.obj"
#define MODEL_LEGO_6 "../res/lego-brick-4x1.obj"
#define MODEL_LEGO_7 "../res/lego-brick-4x1-low.obj"
#define MODEL_LEGO_8 "../res/lego-brick-4x2.obj"
#define MODEL_LEGO_9 "../res/lego-brick-4x2-low.obj"

const glm::vec3 legoBrickColors[4] = {
	(1 / 255.0f) * glm::vec3(222.0f, 0.0f, 13.0f),
	(1 / 255.0f) * glm::vec3(0.0f, 150.0f, 36.0f),
	(1 / 255.0f) * glm::vec3(0.0f, 87.0f, 168.0f),
	(1 / 255.0f) * glm::vec3(254.0f, 196.0f, 0.0f)
};

/// controls in controlState
enum {
	CTRL_FULLSCREEN, CTRL_INIT,
	CTRL_CAM_FLOW_FORWARD, CTRL_CAM_FLOW_BACKWARD,
	CTRL_BANNER0, CTRL_BANNER1, CTRL_BANNER2, CTRL_BANNER3,
	CTRL_BLACK,
	CTRL_SONG_SET,
	CTRL_COUNT
};

/// active drums
enum {
	DRUM_KICK1, DRUM_HIHAT_CLOSED, DRUM_PLUCK, // skala
	DRUM_COUNT
};

/// keys in controlState
enum {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	KEY_W, KEY_S, KEY_A, KEY_D, KEY_Q, KEY_E,
	KEY_COUNT
};

const unsigned int nCubeVertices = 8;
const unsigned int nCubeAttribsPerVertex = 6;
const float cubeVertices[nCubeVertices * nCubeAttribsPerVertex] = {
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

const unsigned int nCubeTriangles = 36;
const unsigned short cubeTriangles[nCubeTriangles] = {
	0, 2, 4,   4, 2, 6, // back face
	0, 1, 2,   2, 1, 3, // left face
	1, 5, 3,   3, 5, 7, // front face
	5, 4, 7,   7, 4, 6, // right face
	3, 7, 2,   2, 7, 6, // upper face
	0, 4, 1,   1, 4, 5, // lower face
};

const unsigned int nBannerVertices = 4;
const unsigned int nBannerAttribsPerVertex = 5;
const float bannerVertices[nBannerVertices * nBannerAttribsPerVertex] = {
	// x      y     z     u     v
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
};

#endif // !_REJFPANK_DATA_HPP