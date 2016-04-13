/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CCONTROLLER
#define _REJFPANK_CCONTROLLER

#include "data.hpp"
#include "TControlState.hpp"
#include "CCamera.hpp"
#include "CSkala.hpp"
//#include "CVeverka.hpp"
//#include "CDefinice.hpp"

/** A class managing the creation and control of songs.
 *
 */

struct CController {
public:
	CController(void);
	~CController(void);

	void redraw(GLFWwindow * window);
	void update(void);
	
	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);
	void nextSong(void);

	CCamera m_camera;

	TControlState m_state;
protected:
	CSong * song;
};

extern CController controller;

#endif // !_REJFPANK_CCONTROLLER
