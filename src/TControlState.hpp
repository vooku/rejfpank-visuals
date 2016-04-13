/**
 * @author	Vadim Petrov
 * @date		2016
 */

#ifndef _REJFPANK_TCONTROLSTATE
#define _REJFPANK_TCONTROLSTATE

#include "data.hpp"

struct TControlState {
	int winWidth;
	int winHeight;

	bool keyMap[KEY_COUNT];
	bool ctrlMap[CTRL_COUNT];
	bool drumMap[DRUM_COUNT];
};

#endif // !_REJFPANK_TCONTROLSTATE