#include "CSong.hpp"

CSong::CSong(CCamera * camera, TControlState * state, CSkybox * skybox)
	: m_camera(camera),
	  m_state(state),
	  m_skybox(skybox),
	  m_multipass(false) { }

CSong::~CSong(void) { } // objects are destroyed in children, foreign objects elsewhere

void CSong::multipassInit(void) {
	glGenFramebuffers(1, &m_frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);

	glGenTextures(1, &m_renderedTex);
	glBindTexture(GL_TEXTURE_2D, m_renderedTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_state->winWidth, m_state->winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // empty image

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderedTex, 0);

	glGenRenderbuffers(1, &m_renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_state->winWidth, m_state->winHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: Cannot init frame buffer." << std::endl;
	else m_multipass = true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
