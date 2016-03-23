#ifndef _REJFPANK_TCONTROLSTATE
#define _REJFPANK_TCONTROLSTATE

#include "data.hpp"

struct TControlState {
	GLsizei winWidth;
	GLsizei winHeight;

	//bool keyMap[KEYS_COUNT];
	bool controlMap[CTRL_COUNT];

	void switchControl(const int ctrl) {
		controlMap[ctrl] = !controlMap[ctrl];
	}
} controlState;

#endif // !_REJFPANK_TCONTROLSTATE
