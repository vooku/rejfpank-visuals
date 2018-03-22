#include "CSongStrobe.hpp"

CSongStrobe::CSongStrobe(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram)
	: CSong(camera, state, skybox),
	  m_bannerShaderProgram(bannerShaderProgram),
	  m_toDraw(0),
	  m_strobeTime(0.0) {

	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Strobe" << std::endl;
}

CSongStrobe::~CSongStrobe(void) {
	for (unsigned int i = 0; i < m_bannersCount; i++) delete m_banners[i];
	delete[] m_banners;

	std::cout << "destroyed song: Strobe" << std::endl;
}

void CSongStrobe::shadersInit(void) { }

void CSongStrobe::modelsInit(void) {
	// banners
	m_bannersCount = 2;
	m_banners = new CBanner *[m_bannersCount];

	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram);
	m_banners[0]->setColor(glm::vec3(1.0f));
	m_banners[1] = new CBanner(m_camera, m_bannerShaderProgram);
	m_banners[1]->setColor(glm::vec3(0.0f));
}

void CSongStrobe::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	m_banners[m_toDraw]->draw(PMatrix, VMatrix);
}

void CSongStrobe::update(double time) {
	const double halftime = BEAT_LENGTH(175) / 16.0;
	const double fulltime = BEAT_LENGTH(175) / 8.0;

	if (time - m_strobeTime >= fulltime) m_strobeTime = time;

	if (time - m_strobeTime < halftime) m_toDraw = 0;
	else m_toDraw = 1;
}

void CSongStrobe::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) { }