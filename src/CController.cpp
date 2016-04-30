#include "CController.hpp"
#include <vector>
#include "pgr/pgr.hpp"
#include "CRock.hpp"
#include "CSquirrel.hpp"
#include "CDefinition.hpp"

CController controller;

CController::CController(void)
	: m_songCtr(FIRST_SONG),
	  m_song(NULL) {

	for (int i = 0; i < CTRL_COUNT; i++) m_state.ctrlMap[i] = false;
	for (int i = 0; i < KEY_COUNT; i++) m_state.keyMap[i] = false;
}

CController::~CController(void) {
	if (m_state.ctrlMap[CTRL_SONG_SET]) {
		m_state.ctrlMap[CTRL_SONG_SET] = false;
		delete m_song;
	}

	delete m_skybox;
}

void CController::shadersInit(void) {
	std::vector<GLuint> shaders;

	// Init skybox shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skyboxShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skyboxShader.frag"));
	m_skyboxShaderProgram.program = pgr::createProgram(shaders);

		// Get uniform locations
		m_skyboxShaderProgram.PVMMatrixLocation			= glGetUniformLocation(m_skyboxShaderProgram.program, "PVMMatrix");
		m_skyboxShaderProgram.colorMultiplierLocation	= glGetUniformLocation(m_skyboxShaderProgram.program, "colorMultiplier");
		// Get input locations
		m_skyboxShaderProgram.posLocation = glGetAttribLocation(m_skyboxShaderProgram.program, "position");

	shaders.clear();

	// Init banner shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/bannerShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/bannerShader.frag"));
	m_bannerShaderProgram.program = pgr::createProgram(shaders);

		// Get uniform locations
		m_bannerShaderProgram.PVMMatrixLocation		= glGetUniformLocation(m_bannerShaderProgram.program, "PVMMatrix");
		m_bannerShaderProgram.useTexLocation		= glGetUniformLocation(m_bannerShaderProgram.program, "useTex");
		m_bannerShaderProgram.texSamplerLocation	= glGetUniformLocation(m_bannerShaderProgram.program, "texSampler");
		m_bannerShaderProgram.ambientLocation		= glGetUniformLocation(m_bannerShaderProgram.program, "color");
		m_bannerShaderProgram.alphaLocation			= glGetUniformLocation(m_bannerShaderProgram.program, "alpha");
		m_bannerShaderProgram.fadeToBlackLocation	= glGetUniformLocation(m_bannerShaderProgram.program, "useTex");
		m_bannerShaderProgram.inverseLocation		= glGetUniformLocation(m_bannerShaderProgram.program, "inverse");
		m_bannerShaderProgram.reducePaletteLocation = glGetUniformLocation(m_bannerShaderProgram.program, "reducePalette");
		m_bannerShaderProgram.tearFlagLocation		= glGetUniformLocation(m_bannerShaderProgram.program, "tearFlag");
		m_bannerShaderProgram.tearNLocation			= glGetUniformLocation(m_bannerShaderProgram.program, "tearN");
		m_bannerShaderProgram.tearBordersLocation	= glGetUniformLocation(m_bannerShaderProgram.program, "tearBorders");
		m_bannerShaderProgram.tearOffsetsLocation	= glGetUniformLocation(m_bannerShaderProgram.program, "tearOffsets");
		// Get input locations
		m_bannerShaderProgram.posLocation		= glGetAttribLocation(m_bannerShaderProgram.program, "position");
		m_bannerShaderProgram.texCoordsLocation	= glGetAttribLocation(m_bannerShaderProgram.program, "texCoords");

	shaders.clear();
}

void CController::modelsInit(void) {
	m_skybox = new CSkybox(glm::vec3(0.0f), glm::vec3(100.0f), &m_skyboxShaderProgram);
	m_grainBanner = new CBanner(&m_camera, &m_bannerShaderProgram, (m_state.ctrlMap[CTRL_4TO3] ? TEX_GEN_NOISE_4TO3 : TEX_GEN_NOISE));
}

void CController::init(const int winWidth, const int winHeight) {
	m_state.winWidth = winWidth;
	m_state.winHeight = winHeight;
	if (m_state.winWidth * 3 == winHeight * 4) {
		m_state.ctrlMap[CTRL_4TO3] = true;

		bannerVertices[1] = -3.0f / 4.0f;
		bannerVertices[6] = -3.0f / 4.0f;
		bannerVertices[11] = 3.0f / 4.0f;
		bannerVertices[16] = 3.0f / 4.0f;
	}

	this->shadersInit();
	this->modelsInit();
	this->nextSong();
}

void CController::redraw(GLFWwindow * window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 VMatrix = glm::lookAt(
		m_camera.m_position,
		m_camera.m_position + m_camera.m_direction,
		m_camera.m_up);

	glm::mat4 PMatrix = glm::perspective(
		glm::radians(CAMERA_VIEW_ANGLE),
		m_state.winWidth / (float)m_state.winHeight,
		CAMERA_VIEW_START,
		CAMERA_VIEW_DIST);

	// all models in the song, incl. the skybox
	if (m_state.ctrlMap[CTRL_SONG_SET]) m_song->redraw(PMatrix, VMatrix);
	
	// always last
	m_grainBanner->draw(PMatrix, VMatrix);

	glfwSwapBuffers(window);
}

void CController::nextSong(void) {
	m_state.ctrlMap[CTRL_SONG_SET] = false;
	if (m_song != NULL) delete m_song;

	switch (m_songCtr) {
		case 0:
			m_song = new CSquirrel(&m_camera, &m_state, m_skybox, &m_bannerShaderProgram);
			break;
		case 1:
			m_song = new CRock(&m_camera, &m_state, m_skybox, &m_bannerShaderProgram);
			break;
		case 2:
			m_song = new CDefinition(&m_camera, &m_state, m_skybox);
			break;
	}

	m_camera.reset();

	m_songCtr = (m_songCtr + 1) % SONG_COUNT;
	m_state.ctrlMap[CTRL_SONG_SET] = true;
}

void CController::update(void) {
	const double time = glfwGetTime();

	if (m_state.keyMap[KEY_UP]    || m_state.keyMap[KEY_W]) m_camera.move(STEP_LENGTH);
	if (m_state.keyMap[KEY_DOWN]  || m_state.keyMap[KEY_S]) m_camera.move(-STEP_LENGTH);
	if (m_state.keyMap[KEY_LEFT]  || m_state.keyMap[KEY_A]) m_camera.sideStep(-STEP_LENGTH);
	if (m_state.keyMap[KEY_RIGHT] || m_state.keyMap[KEY_D]) m_camera.sideStep(STEP_LENGTH);
	if (m_state.keyMap[KEY_Q]) m_camera.roll(ROTATION_ANGLE_DELTA);
	if (m_state.keyMap[KEY_E]) m_camera.roll(-ROTATION_ANGLE_DELTA);

	// the song has to update first as it could do various funny stuff
	if (m_state.ctrlMap[CTRL_SONG_SET]) m_song->update(time);

	m_skybox->m_position = m_camera.m_position;
}

void CController::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	if (m_state.ctrlMap[CTRL_SONG_SET]) m_song->midiIn(status, note, velocity);
	else std::cerr << "No song currently set, MIDI message discarded: " << status << " " << note << " " << velocity << std::endl;
}