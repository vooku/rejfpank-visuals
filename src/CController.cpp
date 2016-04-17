#include "CController.hpp"
#include <vector>
#include "pgr/pgr.hpp"

CController controller;

CController::CController(void) {
	m_state.winWidth = INIT_WIN_WIDTH;
	m_state.winHeight = INIT_WIN_HEIGHT;

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
		m_bannerShaderProgram.texSamplerLocation	= glGetUniformLocation(m_bannerShaderProgram.program, "texSampler");
		m_bannerShaderProgram.ambientLocation		= glGetUniformLocation(m_bannerShaderProgram.program, "color");
		m_bannerShaderProgram.alphaLocation			= glGetUniformLocation(m_bannerShaderProgram.program, "alpha");
		m_bannerShaderProgram.booleanFlagLocation	= glGetUniformLocation(m_bannerShaderProgram.program, "useTex");
		// Get input locations
		m_bannerShaderProgram.posLocation		= glGetAttribLocation(m_bannerShaderProgram.program, "position");
		m_bannerShaderProgram.texCoordsLocation	= glGetAttribLocation(m_bannerShaderProgram.program, "texCoords");

	shaders.clear();
}

void CController::modelsInit(void) {
	m_skybox = new CSkybox(glm::vec3(0.0f), glm::vec3(100.0f), &m_skyboxShaderProgram);
	m_grainBanner = new CBanner(&m_camera, &m_bannerShaderProgram, true, TEX_NOISE);
}

void CController::init(void) {
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

	// always first
	m_skybox->draw(PMatrix, VMatrix);

	// all models in the song
	if (m_state.ctrlMap[CTRL_SONG_SET]) m_song->redraw(PMatrix, VMatrix);
	
	// always last
	m_grainBanner->draw(PMatrix, VMatrix);

	glfwSwapBuffers(window); // nechat!
}

void CController::nextSong(void) {
	if (!m_state.ctrlMap[CTRL_SONG_SET]) {
		if (ACTIVE_SONG == "Skala") m_song = new CRock(&m_camera, &m_bannerShaderProgram, m_skybox);
		else if (ACTIVE_SONG == "Veverka") m_song = new CSquirrel(&m_camera, &m_bannerShaderProgram);
		//else if (ACTIVE_SONG == "Definice") m_song = new CSquirrel(&m_camera,&m_bannerShaderProgram);
		else {
			std::cerr << "No song selected!" << std::endl;
			return;
		}
		m_state.ctrlMap[CTRL_SONG_SET] = true;
	}
	// TODO destroy
	// TODO select from list in data
	// TODO init
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