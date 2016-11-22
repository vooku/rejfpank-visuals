#include "CSongSoundcheck.hpp"
//#include "pgr/pgr.hpp"

CSongSoundcheck::CSongSoundcheck(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram)
	: CSong(camera, state, skybox),
	m_bannerShaderProgram(bannerShaderProgram),
	m_colorShift(0) {

	this->multipassInit();
	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Soundcheck" << std::endl;
}

CSongSoundcheck::~CSongSoundcheck(void) {
	for (unsigned int i = 0; i < m_bannersCount; i++) delete m_banners[i];
	delete[] m_banners;

	std::cout << "destroyed song: Soundcheck" << std::endl;
}

void CSongSoundcheck::shadersInit(void) { }

void CSongSoundcheck::modelsInit(void) {
	// banners
	m_bannersCount = 1;
	m_banners = new CBanner *[m_bannersCount];

	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram);
	m_banners[0]->setColor(glm::vec3(0.5f, 0.0f, 1.0f));
}

void CSongSoundcheck::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (m_multipass) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, m_renderedTex);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_banners[0]->setColorShift(m_colorShift);
		m_banners[0]->draw(PMatrix, VMatrix);
	}
}

void CSongSoundcheck::update(double time) { }

void CSongSoundcheck::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	m_colorShift = (m_colorShift + 1) % 3;
}