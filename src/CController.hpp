/**
 * @author	Vadim Petrov
 * @date		2016
 */

#ifndef _REJFPANK_CCONTROLLER
#define _REJFPANK_CCONTROLLER

#include "data.hpp"
#include "TControlState.hpp"
#include "CCamera.hpp"
#include "CRock.hpp"
#include "CSquirrel.hpp"
//#include "CDefinice.hpp"

/** A class managing the creation and control of songs.
 *
 */

class CController {
public:
	CController(void);
	~CController(void);

	/**
	 * Called as the last init.
	 *
	 * Initializes common shaders and models and the first song.
	 * For that, everything else has to be initialized already.
	 */
	void init(void);

	void redraw(GLFWwindow * window);
	void update(void);
	
	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);
	void nextSong(void);

	CCamera m_camera;

	TControlState m_state;
protected:
	void shadersInit(void);
	void modelsInit(void);

	CSong * m_song;

	CSkybox * m_skybox;
	TCommonShaderProgram m_skyboxShaderProgram;
	CBanner * m_grainBanner;
	TCommonShaderProgram m_bannerShaderProgram;
};

extern CController controller;

#endif // !_REJFPANK_CCONTROLLER
