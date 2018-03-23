/**
* @author	Vadim Petrov
* @date		2016
* @brief	Basic defines and data structures.
*/

#ifndef _REJFPANK_DATA_HPP
#define _REJFPANK_DATA_HPP

#include <glm/glm.hpp>
#include <string>

// Macros
#define BEAT_LENGTH(bpm) (60.0 / bpm)
#define RGB_8BIT(color) (1 / 255.0f) * color

// testing
#define SELECT_MIDI_PORT_MAN true
#define SELECT_MIDI_PORT_DEFAULT 11
#define SELECT_MONITOR_MAN true
#define SELECT_MONITOR_DEFAULT 2

// Global constants
#define SONG_COUNT 6
#define FIRST_SONG 0
#define M_PI 3.14159265359f
#define MIDI_LONG_BUFFER_SIZE 1024
#define MOUSE_SENSITIVITY 0.01f
#define ROTATION_ANGLE_DELTA 0.02f ///< in degrees
#define STEP_LENGTH 0.1f
#define ROTATION_SPEED 2.0f
#define ROTATION_TIME 0.2f
#define FLOW_MAX_TIME 2.3f
#define EXP_DECAY 0.9f
#define SCREEN_TEARS 10 // check bannerShader.frag if there's enough space!

#define MATERIAL_GEN_AMBIENT_MULTI 0.1f
#define MATERIAL_LEGO_SPECULAR glm::vec3(0.3f)
#define MATERIAL_LEGO_SHININES 3.0f

#define BANNER_SIZE (2 * glm::tan(glm::radians(CAMERA_VIEW_ANGLE / 2.0f)))
#define BANNER_PARAM_NO_TEX "no_tex"
#define BANNER_PARAM_NO_TEX_BLACK "no_tex_black"
#define BANNER_PARAM_MULTIPASS "multipass"

#define INIT_WIN_WIDTH 848 ///< Width to height ratio is 16:9
#define INIT_WIN_HEIGHT 480 ///< Width to height ratio is 16:9
#define WIN_TITLE "rejfpank visuals"

#define CAMERA_INIT_POS glm::vec3(0.0f, 0.0f, -6.0f)
#define CAMERA_INIT_DIR glm::vec3(0.0f, 0.0f, 1.0f)
#define CAMERA_INIT_UP  glm::vec3(0.0f, 1.0f, 0.0f)
#define CAMERA_INIT_FREE true
#define CAMERA_VIEW_ANGLE 65.0f
#define CAMERA_VIEW_START 0.01f
#define CAMERA_VIEW_DIST 200.0f
#define CAMERA_DIR_FORWARD 1
#define CAMERA_DIR_BACKWARD -1
#define CAMERA_ROTATE_SPEED 0.02f
#define CAMERA_ENCIRCLE_SPEED 0.3f

// Resources
#define TEX_GEN_TEST		"res/tex/gen_test.png"
#define TEX_GEN_NOISE		"res/tex/gen_noise.jpg"
#define TEX_GEN_NOISE_4TO3	"res/tex/gen_noise-4to3.jpg"
#define TEX_GEN_FIN			"res/tex/gen_fin.jpg"
#define TEX_GEN_FIN_4TO3	"res/tex/gen_fin-4to3.jpg"
#define TEX_DEF_1			"res/tex/def_1.png"
#define TEX_DEF_1_4TO3		"res/tex/def_1-4to3.png"
#define TEX_DEF_2			"res/tex/def_2.png"
#define TEX_DEF_2_4TO3		"res/tex/def_2-4to3.png"
#define TEX_DEF_3			"res/tex/def_3.png"
#define TEX_DEF_3_4TO3		"res/tex/def_3-4to3.png"
#define TEX_DEF_MOD			"res/tex/def_mod.png"
#define TEX_DEF_MEANEYE		"res/tex/def_mean-eye.png"
#define TEX_DEF_IMPACT		"res/tex/def_impact.png"
#define TEX_DEF_FMOON		"res/tex/def_fmoon.jpg"
#define TEX_HER_1			"res/tex/her1.jpg"
#define TEX_HER_2			"res/tex/her2.jpg"
#define TEX_HER_3			"res/tex/her3.jpg"
#define TEX_HER_4			"res/tex/her4.jpg"
#define TEX_HER_5			"res/tex/her5.jpg"

#define IMG_GEN_TEST		"res/img/gen_test.png"
#define IMG_SQI_SQUIRREL1	"res/img/sqi_hairy.png"
#define IMG_SQI_SQUIRREL2	"res/img/sqi_hairy-mutant.png"
#define IMG_DEF_EYE_BLACK	"res/img/def_mean-eye-black.png"

#define MODEL_DEF_HONEY		"res/models/def_honey-comb.obj"
#define MODEL_ROC_LEGO_0	"res/models/roc_lego-brick-1x1.obj"
#define MODEL_ROC_LEGO_1	"res/models/roc_lego-brick-1x1-low.obj"
#define MODEL_ROC_LEGO_2	"res/models/roc_lego-brick-2x1.obj"
#define MODEL_ROC_LEGO_3	"res/models/roc_lego-brick-2x1-low.obj"
#define MODEL_ROC_LEGO_4	"res/models/roc_lego-brick-2x2.obj"
#define MODEL_ROC_LEGO_5	"res/models/roc_lego-brick-2x2-low.obj"
#define MODEL_ROC_LEGO_6	"res/models/roc_lego-brick-4x1.obj"
#define MODEL_ROC_LEGO_7	"res/models/roc_lego-brick-4x1-low.obj"
#define MODEL_ROC_LEGO_8	"res/models/roc_lego-brick-4x2.obj"
#define MODEL_ROC_LEGO_9	"res/models/roc_lego-brick-4x2-low.obj"
#define MODEL_HER_SPHERE	"res/models/lowpolysphere.obj"

// Song specific constants
#define HER_PROB1 0.3f
#define HER_PROB2 0.5f
#define HER_PROB3 0.15f
#define HER_PROB4 0.25f

#define DEF_HONEYCOMBS_N_PER_LINE 16
#define DEF_HONEYCOMBS_LINES_N 5 // only odd numbers, even ones break the pattern

#define LEGO_BRICKS_LOOPS 8
#define LEGO_BRICKS_DIST 5
#define LEGO_BRICKS_COUNT 8
#define MIDAS_TIME 3.0f

#define LEGO_BRICK_COLORS_COUNT 4
const glm::vec3 legoBrickColors[LEGO_BRICK_COLORS_COUNT] = {
	RGB_8BIT(glm::vec3(222.0f, 0.0f, 13.0f)),
	RGB_8BIT(glm::vec3(0.0f, 150.0f, 36.0f)),
	RGB_8BIT(glm::vec3(0.0f, 87.0f, 168.0f)),
	RGB_8BIT(glm::vec3(254.0f, 196.0f, 0.0f))
};

/// controls in controlState
enum {
	CTRL_FULLSCREEN, CTRL_INIT, CTRL_4TO3,
	CTRL_SONG_SET, CTRL_FIN,
	CTRL_COUNT
};

/// keys in controlState
enum {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	KEY_W, KEY_S, KEY_A, KEY_D, KEY_Q, KEY_E,
	KEY_COUNT
};

/// controls in CSongDefinition
enum {
	DEF_BANNER0, DEF_BANNER1, DEF_BANNER2,
	DEF_BLUE, DEF_RED, DEF_POINT,
	DEF_INVERSE, DEF_REDUCE, DEF_SWEEP,
	DEF_BACKGROUND,
	DEF_COUNT
};

/// controls in CSongRock
enum {
	ROCK_CAM_FORWARD, ROCK_CAM_BACKWARD, ROCK_CAM_Q, ROCK_CAM_E,
	ROCK_BANNER0, ROCK_BANNER1, ROCK_BANNER2,
	ROCK_BLACK,
	ROCK_KICK1, ROCK_HIHAT_CLOSED, ROCK_PLUCK,
	ROCK_COUNT
};

/// controls in CSongSquirrel
enum {
	SQUIR_ROTATE, SQUIR_INVERSE, SQUIR_REDUCE, SQUIR_TEAR,
	SQUIR_SQUIRREL1, SQUIR_SQUIRREL2,
	SQUIR_BANNER0, SQUIR_BANNER1, SQUIR_BANNER2,
	SQUIR_COUNT
};

/// controls in CSongHeros
enum {
	HER_BANNER0, HER_BANNER1,
	HER_INVERSE, HER_REDUCE, HER_STROBE, HER_KATAR1, HER_KATAR2, HER_KATAR3,
	HER_KICK1, HER_KICK2, HER_SNR1, HER_SNR2, HER_TOM1, HER_TOM2, HER_TOM3, HER_BASS1, HER_BASS2, HER_SUB1, HER_SUB2,
	HER_COUNT
};

#endif // !_REJFPANK_DATA_HPP