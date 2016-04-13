#include "CSkala.hpp"
#include "midicodes.hpp"
#include "pgr/pgr.hpp"
#include <vector>

using namespace std;

CSkala::CSkala(CCamera * camera, TControlState * state)
	: CSong(camera, state),
	  m_loopCtr(0) {

	this->shadersInit();
	this->modelsInit();
	cout << "loaded song Skala!" << endl;
}

void CSkala::shadersInit(void) {
	vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[3];

	// Init skybox shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skyboxShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skyboxShader.frag"));
	m_shaderPrograms[0].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[0].PVMMatrixLocation = glGetUniformLocation(m_shaderPrograms[0].program, "PVMMatrix");
		m_shaderPrograms[0].colorMultiplierLocation = glGetUniformLocation(m_shaderPrograms[0].program, "colorMultiplier");
		// Get input locations
		m_shaderPrograms[0].posLocation = glGetAttribLocation(m_shaderPrograms[0].program, "position");

	shaders.clear();

	// Init lego shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/commonShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/commonShader.frag"));
	m_shaderPrograms[1].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[1].PVMMatrixLocation = glGetUniformLocation(m_shaderPrograms[1].program, "PVMMatrix");
		m_shaderPrograms[1].VMatrixLocation = glGetUniformLocation(m_shaderPrograms[1].program, "VMatrix");
		m_shaderPrograms[1].MMatrixLocation = glGetUniformLocation(m_shaderPrograms[1].program, "MMatrix");
		m_shaderPrograms[1].normalMatrixLocation = glGetUniformLocation(m_shaderPrograms[1].program, "normalMatrix");
		m_shaderPrograms[1].diffuseLocation = glGetUniformLocation(m_shaderPrograms[1].program, "material.diffuse");
		m_shaderPrograms[1].ambientLocation = glGetUniformLocation(m_shaderPrograms[1].program, "material.ambient");
		m_shaderPrograms[1].specularLocation = glGetUniformLocation(m_shaderPrograms[1].program, "material.specular");
		m_shaderPrograms[1].shininessLocation = glGetUniformLocation(m_shaderPrograms[1].program, "material.shininess");
		m_shaderPrograms[1].booleanFlagLocation = glGetUniformLocation(m_shaderPrograms[1].program, "fadeToBlack");
		// Get input locations
		m_shaderPrograms[1].posLocation = glGetAttribLocation(m_shaderPrograms[1].program, "position");
		m_shaderPrograms[1].normalLocation = glGetAttribLocation(m_shaderPrograms[1].program, "normal");

	shaders.clear();

	// Init banner shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/bannerShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/bannerShader.frag"));
	m_shaderPrograms[2].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[2].PVMMatrixLocation = glGetUniformLocation(m_shaderPrograms[2].program, "PVMMatrix");
		m_shaderPrograms[2].texSamplerLocation = glGetUniformLocation(m_shaderPrograms[2].program, "texSampler");
		m_shaderPrograms[2].ambientLocation = glGetUniformLocation(m_shaderPrograms[2].program, "color");
		m_shaderPrograms[2].alphaLocation = glGetUniformLocation(m_shaderPrograms[2].program, "alpha");
		m_shaderPrograms[2].booleanFlagLocation = glGetUniformLocation(m_shaderPrograms[2].program, "useTex");
		// Get input locations
		m_shaderPrograms[2].posLocation = glGetAttribLocation(m_shaderPrograms[2].program, "position");
		m_shaderPrograms[2].texCoordsLocation = glGetAttribLocation(m_shaderPrograms[2].program, "texCoords");

	shaders.clear();
}

void CSkala::modelsInit(void) {
	// skybox
	m_skybox = new CSkybox(glm::vec3(0.0f), glm::vec3(100.0f), &m_shaderPrograms[0]);

	// lego
	m_legoData = new CDrawable * [3];
	m_legoData[0] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[1]); // kick
	m_legoData[1] = new CLoadedObj(MODEL_LEGO_7, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[1]); // hihat
	m_legoData[2] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[1]); // pluck

	m_lego = new CDrawable * [LEGO_BRICKS_LOOPS * 10];

	glm::vec2 kickPos = glm::vec2(LEGO_BRICKS_DIST * glm::cos(glm::radians(45.0f)), LEGO_BRICKS_DIST * glm::sin(glm::radians(45.0f)));
	glm::vec2 hihatPos = glm::vec2(LEGO_BRICKS_DIST * 1.0f, LEGO_BRICKS_DIST * 0.0f);
	glm::vec2 pluckPos = glm::vec2(LEGO_BRICKS_DIST * glm::cos(glm::radians(22.5f)), LEGO_BRICKS_DIST * glm::sin(glm::radians(22.5f)));

	for (int i = 0; i < LEGO_BRICKS_LOOPS; i++) {
		// kick bricks
		m_lego[i * 10 + 0] = new CLoadedObj(MODEL_LEGO_8, glm::vec3( kickPos.x,  kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[0], i);
		m_lego[i * 10 + 1] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(-kickPos.x,  kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[0], i);
		m_lego[i * 10 + 2] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(-kickPos.x, -kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[0], i);
		m_lego[i * 10 + 3] = new CLoadedObj(MODEL_LEGO_8, glm::vec3( kickPos.x, -kickPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[0], i);
		// hihat bricks
		m_lego[i * 10 + 4] = new CLoadedObj(MODEL_LEGO_7, glm::vec3( hihatPos.x, hihatPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[1], i);
		m_lego[i * 10 + 5] = new CLoadedObj(MODEL_LEGO_7, glm::vec3(-hihatPos.x, hihatPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[1], i);
		// pluck bricks
		m_lego[i * 10 + 6] = new CLoadedObj(MODEL_LEGO_9, glm::vec3( pluckPos.x,  pluckPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[2], i);
		m_lego[i * 10 + 7] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(-pluckPos.x,  pluckPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[2], i);
		m_lego[i * 10 + 8] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(-pluckPos.x, -pluckPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[2], i);
		m_lego[i * 10 + 9] = new CLoadedObj(MODEL_LEGO_9, glm::vec3( pluckPos.x, -pluckPos.y, i * LEGO_BRICKS_DIST), glm::vec3(1.0f), &m_shaderPrograms[1], (CLoadedObj *)m_legoData[2], i);
	}

	// banners
	m_banners = new CDrawable * [3];
	m_banners[0] = new CBanner(m_camera, &m_shaderPrograms[2], false);
	m_banners[1] = new CBanner(m_camera, &m_shaderPrograms[2], false);
	m_banners[2] = new CBanner(m_camera, &m_shaderPrograms[2], true, TEX_NOISE);
	((CBanner *)m_banners[0])->setColor(glm::vec3(1.0f));
	((CBanner *)m_banners[1])->setColor(glm::vec3(1.0f, 1.0f, 0.0f));
}

CSkala::~CSkala(void) {
	delete m_shaderPrograms;

	delete m_skybox;

	for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i++) delete m_lego[i];
	delete[] m_lego;
	for (int i = 0; i < 3; i++) delete m_legoData[i];
	delete[] m_legoData;

	for (int i = 0; i < 3; i++) delete m_banners[i];
	delete[] m_banners;
}

void CSkala::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	// skybox
	m_skybox->draw(PMatrix, VMatrix);

	// lego
	for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10) {
		if (m_state->drumMap[DRUM_KICK1])
			for (int j = 0; j < 4; j++)
				m_lego[i + j]->draw(PMatrix, VMatrix);

		if (m_state->drumMap[DRUM_HIHAT_CLOSED])
			for (int j = 4; j < 6; j++)
				m_lego[i + j]->draw(PMatrix, VMatrix);

		if (m_state->drumMap[DRUM_PLUCK])
			for (int j = 6; j < 10; j++)
				m_lego[i + j]->draw(PMatrix, VMatrix);
	}

	// banners
	if (m_state->ctrlMap[CTRL_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_state->ctrlMap[CTRL_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);
	m_banners[2]->draw(PMatrix, VMatrix);
}

void CSkala::replaceLoop(const int dir) {
	if (dir == CAMERA_DIR_FORWARD) {
		int index = m_loopCtr * 10;
		for (int i = 0; i < 10; i++)
			m_lego[index + i]->m_position.z += LEGO_BRICKS_DIST * LEGO_BRICKS_LOOPS;
		m_loopCtr = (m_loopCtr + 1) % LEGO_BRICKS_LOOPS;
	}
	else { // CAMERA_DIR_BACKWARD
		int index = m_loopCtr == 0 ? 4 * 10 : (m_loopCtr - 1) * 10;
		for (int i = 0; i < 10; i++)
			m_lego[index + i]->m_position.z -= LEGO_BRICKS_DIST * LEGO_BRICKS_LOOPS;
		m_loopCtr = m_loopCtr == 0 ? 4 : m_loopCtr - 1;
	}
}

void CSkala::setCamFlow(const bool flowForward, const bool flowBackward, const bool rollQ, const bool rollE, const bool trigger) {
	m_state->ctrlMap[CTRL_CAM_FLOW_FORWARD] = flowForward;
	m_state->ctrlMap[CTRL_CAM_FLOW_BACKWARD] = flowBackward;
	m_state->keyMap[KEY_Q] = rollQ;
	m_state->keyMap[KEY_E] = rollE;

	if (trigger) m_camera->m_triggerTime = glfwGetTime();
}

void CSkala::update(double time) {
	if (m_state->ctrlMap[CTRL_CAM_FLOW_FORWARD]) m_camera->flow(time, CAMERA_DIR_FORWARD);
	if (m_state->ctrlMap[CTRL_CAM_FLOW_BACKWARD]) m_camera->flow(time, CAMERA_DIR_BACKWARD);

	m_skybox->m_position = m_camera->m_position;

	if (m_camera->m_position.z >= m_lego[m_loopCtr * 10]->m_position.z - 1) this->replaceLoop(CAMERA_DIR_FORWARD);
	if (m_camera->m_position.z <  m_lego[m_loopCtr * 10]->m_position.z - 7) this->replaceLoop(CAMERA_DIR_BACKWARD);

	for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i++) m_lego[i]->rotate(time);

	if (m_state->ctrlMap[CTRL_BANNER0]) {
		((CBanner *)m_banners[0])->updateAlpha(time);
		if (time - m_banners[0]->m_triggerTime >= MIDAS_TIME) m_state->ctrlMap[CTRL_BANNER0] = false;
	}
	if (m_state->ctrlMap[CTRL_BANNER1]) {
		((CBanner *)m_banners[1])->updateAlpha(time);
		if (time - m_banners[1]->m_triggerTime >= MIDAS_TIME) m_state->ctrlMap[CTRL_BANNER1] = false;
	}
}

void CSkala::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	//-------------------------------------------------------------------> AKAI MPX16
	if (status == MIDI_NOTE_ON_CH10) {
		switch (note) {
		case MPX16_PAD01:
			this->setCamFlow(true, false, false, false, true);
			break;
		case MPX16_PAD02:
			this->setCamFlow(false, true, false, false, true);
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
			m_state->ctrlMap[CTRL_CAM_FLOW_FORWARD] = false;
			m_state->ctrlMap[CTRL_CAM_FLOW_BACKWARD] = false;
			if (!m_state->keyMap[KEY_Q] && !m_state->keyMap[KEY_E])
				rand() % 2 == 0 ? m_state->keyMap[KEY_Q] = true : m_state->keyMap[KEY_E] = true;
			else {
				m_state->keyMap[KEY_Q] = !m_state->keyMap[KEY_Q];
				m_state->keyMap[KEY_E] = !m_state->keyMap[KEY_E];
			}
			m_camera->m_triggerTime = glfwGetTime();
			break;
		case MPX16_PAD09:
			this->setCamFlow(false, true, false, false, true);
			break;
		case MPX16_PAD10: // block bass
			this->setCamFlow(false, false, false, false, false);
			break;
		case MPX16_PAD11: // block midas
			m_state->ctrlMap[CTRL_BANNER0] = false;
			m_state->ctrlMap[CTRL_BANNER1] = false;
			break;
		case MPX16_PAD12:
			m_state->ctrlMap[CTRL_BANNER0] = true;
			m_banners[0]->m_triggerTime = glfwGetTime();
			if (!m_state->ctrlMap[CTRL_BLACK]) {
				m_state->ctrlMap[CTRL_BLACK] = true;
				for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i++) ((CLoadedObj *)m_lego[i])->fadeToBlack();
				((CSkybox *)m_skybox)->m_colorMultiplier = 0.1f;
			}
			break;
		case MPX16_PAD13:
			this->setCamFlow(false, true, false, false, true);
			break;
		case MPX16_PAD14: // block bass
			this->setCamFlow(false, false, false, false, false);
			break;
		case MPX16_PAD15:  // block midas
			m_state->ctrlMap[CTRL_BANNER0] = false;
			m_state->ctrlMap[CTRL_BANNER1] = false;
			break;
		case MPX16_PAD16:
			m_state->ctrlMap[CTRL_BANNER1] = true;
			m_banners[1]->m_triggerTime = glfwGetTime();
			break;
		default:
			cout << "Unresolved midi note from MPX16:" << status << " " << note << " " << velocity << endl;
			break;
		}
	}
	//-------------------------------------------------------------------> ALESIS SR16
	else if (status == MIDI_NOTE_ON_CH02) {
		switch (note) {
		case MIDI_DRUM_KICK1:
			m_state->drumMap[DRUM_KICK1] = true;
			for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10)
				for (int j = 0; j < 4; j++)
					m_lego[i + j]->switchRotAxis(glfwGetTime());
			break;
		case MIDI_DRUM_HIHAT_CLOSED:
			m_state->drumMap[DRUM_HIHAT_CLOSED] = true;
			for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10)
				for (int j = 4; j < 6; j++)
					m_lego[i + j]->switchRotAxis(glfwGetTime());
			break;
		case MIDI_DRUM_SNARE2:
			m_state->drumMap[DRUM_PLUCK] = true;
			for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10)
				for (int j = 6; j < 10; j++)
					m_lego[i + j]->switchRotAxis(glfwGetTime());
			break;
		default:
			cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << endl;
			break;
		}
	}
}
