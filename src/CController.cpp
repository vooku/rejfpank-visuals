#include "CController.hpp"

using namespace std;

CController controller;

CController::CController(void) {
	m_state.winWidth = INIT_WIN_WIDTH;
	m_state.winHeight = INIT_WIN_HEIGHT;

	for (int i = 0; i < CTRL_COUNT; i++) m_state.ctrlMap[i] = false;
	for (int i = 0; i < DRUM_COUNT; i++) m_state.drumMap[i] = false;
	for (int i = 0; i < KEY_COUNT; i++) m_state.keyMap[i] = false;
}

CController::~CController(void) {
	if (m_state.ctrlMap[CTRL_SONG_SET]) {
		m_state.ctrlMap[CTRL_SONG_SET] = false;
		delete song;
	}
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

	if (m_state.ctrlMap[CTRL_SONG_SET]) song->redraw(PMatrix, VMatrix);

	glfwSwapBuffers(window); // nechat!
}

void CController::nextSong(void) {
	if (!m_state.ctrlMap[CTRL_SONG_SET]) {
		song = new CSkala(&m_camera, &m_state);
		m_state.ctrlMap[CTRL_SONG_SET] = true;
	}
	// TODO destroy
	// TODO select from list in data
	// TODO init
}

void CController::update(void) {
	const double t = glfwGetTime();
	if (m_state.keyMap[KEY_UP]    || m_state.keyMap[KEY_W]) m_camera.move(STEP_LENGTH);
	if (m_state.keyMap[KEY_DOWN]  || m_state.keyMap[KEY_S]) m_camera.move(-STEP_LENGTH);
	if (m_state.keyMap[KEY_LEFT]  || m_state.keyMap[KEY_A]) m_camera.sideStep(-STEP_LENGTH);
	if (m_state.keyMap[KEY_RIGHT] || m_state.keyMap[KEY_D]) m_camera.sideStep(STEP_LENGTH);
	if (m_state.keyMap[KEY_Q]) m_camera.roll(ROTATION_ANGLE_DELTA);
	if (m_state.keyMap[KEY_E]) m_camera.roll(-ROTATION_ANGLE_DELTA);

	if (m_state.ctrlMap[CTRL_SONG_SET]) song->update(t);
}

void CController::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	if (m_state.ctrlMap[CTRL_SONG_SET]) song->midiIn(status, note, velocity);
	else cerr << "No song currently set, MIDI message discarded: " << status << " " << note << " " << velocity << endl;
}