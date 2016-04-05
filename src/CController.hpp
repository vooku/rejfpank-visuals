/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CCONTAINER
#define _REJFPANK_CCONTAINER

#include "data.hpp"
#include "CSkybox.hpp"
#include "CLoadedObj.hpp"
#include "CCamera.hpp"

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

		void switchState(const int ctrl) {
			ctrlMap[ctrl] = !ctrlMap[ctrl];
		}
	} state;
protected:
	CLoadedObj ** lego;
	CSkybox * skybox;

	TCommonShaderProgram skyboxShaderProgram;
	TCommonShaderProgram legoShaderProgram;
};

extern CController controller;

#endif // !_REJFPANK_CCONTAINER
