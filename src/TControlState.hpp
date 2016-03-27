#ifndef _REJFPANK_TCONTROLSTATE
#define _REJFPANK_TCONTROLSTATE

#include "data.hpp"

struct TControlState {
	GLsizei winWidth;
	GLsizei winHeight;

	bool keyMap[KEY_COUNT];
	bool ctrlMap[CTRL_COUNT];

	void switchState(const int ctrl) {
		ctrlMap[ctrl] = !ctrlMap[ctrl];
	}
};

extern TControlState controlState;

#endif // !_REJFPANK_TCONTROLSTATE
