#include "CSong.hpp"

CSong::CSong(CCamera * camera, CSkybox * skybox)
	: m_camera(camera),
	  m_skybox(skybox) { }

CSong::~CSong(void) { } // foreign objects are destroyed elsewhere