#include "CRock.hpp"
#include "midicodes.hpp"
#include "pgr/pgr.hpp"
#include <vector>

CRock::CRock(CCamera * camera, TCommonShaderProgram * bannerShaderProgram, CSkybox * skybox)
	: CSong(camera),
	  m_bannerShaderProgram(bannerShaderProgram),
	  m_skybox(skybox),
	  m_loopCtr(0) {

	m_innerMap = new bool[ROCK_COUNT];
	for (int i = 0; i < ROCK_COUNT; i++) m_innerMap[i] = false;

	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Skala" << std::endl;
}

CRock::~CRock(void) {
	delete[] m_shaderPrograms;

	for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i++) delete m_lego[i];
	delete[] m_lego;
	for (int i = 0; i < 3; i++) delete m_legoData[i];
	delete[] m_legoData;

	for (int i = 0; i < 2; i++) delete m_banners[i];
	delete[] m_banners;

	delete[] m_innerMap;
}

void CRock::shadersInit(void) {
	std::vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[1];

	// Init lego shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/commonShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/commonShader.frag"));
	m_shaderPrograms[0].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[0].PVMMatrixLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "PVMMatrix");
		m_shaderPrograms[0].VMatrixLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "VMatrix");
		m_shaderPrograms[0].MMatrixLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "MMatrix");
		m_shaderPrograms[0].normalMatrixLocation	= glGetUniformLocation(m_shaderPrograms[0].program, "normalMatrix");
		m_shaderPrograms[0].diffuseLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "material.diffuse");
		m_shaderPrograms[0].ambientLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "material.ambient");
		m_shaderPrograms[0].specularLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "material.specular");
		m_shaderPrograms[0].shininessLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "material.shininess");
		m_shaderPrograms[0].booleanFlagLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "fadeToBlack");
		// Get input locations
		m_shaderPrograms[0].posLocation		= glGetAttribLocation(m_shaderPrograms[0].program, "position");
		m_shaderPrograms[0].normalLocation	= glGetAttribLocation(m_shaderPrograms[0].program, "normal");

	shaders.clear();
}

void CRock::modelsInit(void) {
	// lego
	m_legoData = new CLoadedObj * [3];
	m_legoData[0] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0]); // kick
	m_legoData[1] = new CLoadedObj(MODEL_LEGO_7, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0]); // hihat
	m_legoData[2] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0]); // pluck

	m_lego = new CLoadedObj * [LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT];

	glm::vec2 kickPos = glm::vec2(LEGO_BRICKS_DIST * glm::cos(glm::radians(45.0f)), LEGO_BRICKS_DIST * glm::sin(glm::radians(45.0f)));
	glm::vec2 hihatPos = glm::vec2(LEGO_BRICKS_DIST * 1.0f, 0.0f);
	glm::vec2 pluckPos = glm::vec2(0.0f, LEGO_BRICKS_DIST * 1.0f);

	for (int i = 0; i < LEGO_BRICKS_LOOPS; i++) {
		// kick bricks
		m_lego[i * LEGO_BRICKS_COUNT + 0] = new CLoadedObj(MODEL_LEGO_8, glm::vec3( kickPos.x,  kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[0], i);
		m_lego[i * LEGO_BRICKS_COUNT + 1] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(-kickPos.x,  kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[0], i);
		m_lego[i * LEGO_BRICKS_COUNT + 2] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(-kickPos.x, -kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[0], i);
		m_lego[i * LEGO_BRICKS_COUNT + 3] = new CLoadedObj(MODEL_LEGO_8, glm::vec3( kickPos.x, -kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[0], i);
		// hihat bricks
		m_lego[i * LEGO_BRICKS_COUNT + 4] = new CLoadedObj(MODEL_LEGO_7, glm::vec3( hihatPos.x, hihatPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[1], i);
		m_lego[i * LEGO_BRICKS_COUNT + 5] = new CLoadedObj(MODEL_LEGO_7, glm::vec3(-hihatPos.x, hihatPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[1], i);
		// pluck bricks
		m_lego[i * LEGO_BRICKS_COUNT + 6] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(pluckPos.x,  pluckPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[2], i);
		m_lego[i * LEGO_BRICKS_COUNT + 7] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(pluckPos.x, -pluckPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[0], m_legoData[2], i);
	}

	// banners
	m_banners = new CBanner * [2];
	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram, false);
	m_banners[1] = new CBanner(m_camera, m_bannerShaderProgram, false);
	m_banners[0]->setColor(glm::vec3(1.0f));
	m_banners[1]->setColor(glm::vec3(1.0f, 1.0f, 0.0f));
}

void CRock::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	// lego
	for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i += LEGO_BRICKS_COUNT) {
		if (m_innerMap[ROCK_KICK1])
			for (int j = 0; j < 4; j++)
				m_lego[i + j]->draw(PMatrix, VMatrix);

		if (m_innerMap[ROCK_HIHAT_CLOSED])
			for (int j = 4; j < 6; j++)
				m_lego[i + j]->draw(PMatrix, VMatrix);

		if (m_innerMap[ROCK_PLUCK])
			for (int j = 6; j < LEGO_BRICKS_COUNT; j++)
				m_lego[i + j]->draw(PMatrix, VMatrix);
	}

	// banners
	if (m_innerMap[ROCK_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_innerMap[ROCK_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);
}

void CRock::replaceLoop(const int dir) {
	if (dir == CAMERA_DIR_FORWARD) {
		int index = m_loopCtr * LEGO_BRICKS_COUNT;
		for (int i = 0; i < LEGO_BRICKS_COUNT; i++)
			m_lego[index + i]->m_position.z += LEGO_BRICKS_DIST * LEGO_BRICKS_LOOPS;
		m_loopCtr = (m_loopCtr + 1) % LEGO_BRICKS_LOOPS;
	}
	else { // CAMERA_DIR_BACKWARD
		m_loopCtr = m_loopCtr == 0 ? LEGO_BRICKS_LOOPS - 1 : m_loopCtr - 1;
		int index = m_loopCtr * LEGO_BRICKS_COUNT;
		for (int i = 0; i < LEGO_BRICKS_COUNT; i++)
			m_lego[index + i]->m_position.z -= LEGO_BRICKS_DIST * LEGO_BRICKS_LOOPS;
	}
}

void CRock::update(double time) {
	if (m_innerMap[ROCK_CAM_FORWARD]) m_camera->flow(time, CAMERA_DIR_FORWARD);
	if (m_innerMap[ROCK_CAM_BACKWARD]) m_camera->flow(time, CAMERA_DIR_BACKWARD);
	if (m_innerMap[ROCK_CAM_Q]) m_camera->roll(ROTATION_ANGLE_DELTA);
	if (m_innerMap[ROCK_CAM_E]) m_camera->roll(-ROTATION_ANGLE_DELTA);

	if (m_camera->m_position.z >= m_lego[m_loopCtr * LEGO_BRICKS_COUNT]->m_position.z - 1) this->replaceLoop(CAMERA_DIR_FORWARD);
	if (m_camera->m_position.z <  m_lego[m_loopCtr * LEGO_BRICKS_COUNT]->m_position.z - 7) this->replaceLoop(CAMERA_DIR_BACKWARD);

	for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i++) m_lego[i]->rotate(time);

	if (m_innerMap[ROCK_BANNER0]) {
		m_banners[0]->updateAlpha(time);
		if (time - m_banners[0]->m_triggerTime >= MIDAS_TIME) m_innerMap[ROCK_BANNER0] = false;
	}
	if (m_innerMap[ROCK_BANNER1]) {
		m_banners[1]->updateAlpha(time);
		if (time - m_banners[1]->m_triggerTime >= MIDAS_TIME) m_innerMap[ROCK_BANNER1] = false;
	}
}

void CRock::setCamFlow(const bool flowForward, const bool flowBackward, const bool rollQ, const bool rollE, const bool trigger) {
	m_innerMap[ROCK_CAM_FORWARD] = flowForward;
	m_innerMap[ROCK_CAM_BACKWARD] = flowBackward;
	m_innerMap[ROCK_CAM_Q] = rollQ;
	m_innerMap[ROCK_CAM_E] = rollE;

	if (trigger) m_camera->m_triggerTime = glfwGetTime();
}

void CRock::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	//-------------------------------------------------------------------> AKAI MPX16
	if (status == MIDI_NOTE_ON_CH10) {
		switch (note) {
		case MPX16_PAD01:
			this->setCamFlow(true, false, false, false, true);
			break;
		case MPX16_PAD02:
			if (m_innerMap[ROCK_CAM_BACKWARD]) this->setCamFlow(true, false, false, false, true);
			else  this->setCamFlow(false, true, false, false, true);
			break;
		case MPX16_PAD03:
			this->setCamFlow(true, false, false, false, true);
			break;
		case MPX16_PAD04: // block bass
			this->setCamFlow(false, false, false, false, false);
			break;
		case MPX16_PAD05:
			this->setCamFlow(true, false, false, true, true);
			break;
		case MPX16_PAD06:
			this->setCamFlow(false, true, true, false, true);
			break;
		case MPX16_PAD07:
			this->setCamFlow(true, false, false, true, true);
			break;
		case MPX16_PAD08:
			if (m_innerMap[ROCK_CAM_Q]) this->setCamFlow(false, false, false, true, true);
			else this->setCamFlow(false, false, true, false, true);
			break;
		case MPX16_PAD09:
			this->setCamFlow(false, true, false, false, true);
			break;
		case MPX16_PAD10: // block bass
			this->setCamFlow(false, false, false, false, false);
			break;
		case MPX16_PAD11: // block midas
			m_innerMap[ROCK_BANNER0] = false;
			m_innerMap[ROCK_BANNER1] = false;
			break;
		case MPX16_PAD12:
			m_innerMap[ROCK_BANNER0] = true;
			m_banners[0]->m_triggerTime = glfwGetTime();
			if (!m_innerMap[ROCK_BLACK]) {
				m_innerMap[ROCK_BLACK] = true;
				for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i++) m_lego[i]->fadeToBlack();
				m_skybox->m_colorMultiplier = 0.1f;
			}
			break;
		case MPX16_PAD13:
			this->setCamFlow(false, true, false, false, true);
			break;
		case MPX16_PAD14: // block bass
			this->setCamFlow(false, false, false, false, false);
			break;
		case MPX16_PAD15:  // block midas
			m_innerMap[ROCK_BANNER0] = false;
			m_innerMap[ROCK_BANNER1] = false;
			break;
		case MPX16_PAD16:
			m_innerMap[ROCK_BANNER1] = true;
			m_banners[1]->m_triggerTime = glfwGetTime();
			break;
		default:
			std::cout << "Unresolved midi note from MPX16:" << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
	//-------------------------------------------------------------------> ALESIS SR16
	else if (status == MIDI_NOTE_ON_CH02) {
		switch (note) {
		case MIDI_DRUM_KICK1:
			m_innerMap[ROCK_KICK1] = true;
			for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i += LEGO_BRICKS_COUNT)
				for (int j = 0; j < 4; j++)
					m_lego[i + j]->switchRotAxis(glfwGetTime());
			break;
		case MIDI_DRUM_HIHAT_CLOSED:
			m_innerMap[ROCK_HIHAT_CLOSED] = true;
			for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i += LEGO_BRICKS_COUNT)
				for (int j = 4; j < 6; j++)
					m_lego[i + j]->switchRotAxis(glfwGetTime());
			break;
		case MIDI_DRUM_SNARE1: // snare
			m_innerMap[ROCK_PLUCK] = true;
			for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i += LEGO_BRICKS_COUNT)
				for (int j = 6; j < LEGO_BRICKS_COUNT; j++)
					m_lego[i + j]->switchRotAxis(glfwGetTime());
			break;
		case MIDI_DRUM_SNARE2: // pluck
			m_innerMap[ROCK_PLUCK] = true;
			for (int i = 0; i < LEGO_BRICKS_LOOPS * LEGO_BRICKS_COUNT; i += LEGO_BRICKS_COUNT)
				for (int j = 6; j < LEGO_BRICKS_COUNT; j++)
					m_lego[i + j]->switchRotAxis(glfwGetTime());
			break;
		default:
			std::cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
}
