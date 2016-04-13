#include "CSong.hpp"

CSong::CSong(CCamera * camera, TControlState * state)
	: m_camera(camera),
	  m_state(state) { }

CSong::~CSong(void) { }