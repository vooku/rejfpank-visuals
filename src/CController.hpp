/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CCONTAINER
#define _REJFPANK_CCONTAINER

#include "data.hpp"
#include "CCamera.hpp"
#include "CSkybox.hpp"
#include "CLoadedObj.hpp"
#include "CBanner.hpp"

/** A class serving as both a container and a controller for all objects.
 *
 */

struct CController {
public:
	CController(void);
	~CController(void);

	void shadersInit(void);
	void modelsInit(void);

	void redraw(GLFWwindow * window);
	void update(void);
	void midiIn(const unsigned int midiStatus, const unsigned int midiParam1, const unsigned int midiParam2);

	CCamera camera;

	struct TControlState {
		GLsizei winWidth;
		GLsizei winHeight;

		bool keyMap[KEY_COUNT];
		bool ctrlMap[CTRL_COUNT];
		bool drumMap[DRUM_COUNT];
	} state;
protected:
	/// Helper function for midiIn()
	void setCamFlow(const bool flowForward, const bool flowBackward, const bool rollQ, const bool rollE, const bool trigger);
	/// Helper function for update()
	void replaceLoop(const int dir);


	CDrawable ** legoData;
	CDrawable ** lego;
	CDrawable * skybox;
	CDrawable ** banners;

	int loopCtr;

	TCommonShaderProgram * shaderPrograms;
};

extern CController controller;

#endif // !_REJFPANK_CCONTAINER
