#include "CController.hpp"
#include "midicodes.hpp"
#include "pgr/Shader.hpp"

#include <vector>

using namespace std;

CController controller;

CController::CController() {
	state.winWidth = INIT_WIN_WIDTH;
	state.winHeight = INIT_WIN_HEIGHT;
}

CController::~CController(void) {
	delete skybox;
	for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i++) delete lego[i];
	delete[] lego;
	for (int i = 0; i < 3; i++) delete legoData[i];
	delete legoData;
}

void CController::redraw(GLFWwindow * window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 VMatrix = glm::lookAt(
		camera.position,
		camera.position + camera.direction,
		camera.up);

	glm::mat4 Pmatrix = glm::perspective(
		glm::radians(CAMERA_VIEW_ANGLE),
		state.winWidth / (float)state.winHeight,
		CAMERA_VIEW_START,
		CAMERA_VIEW_DIST);

	skybox->draw(Pmatrix, VMatrix);
	for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10) {
		if (state.drumMap[DRUM_KICK1])
			for (int j = 0; j < 4; j++)
				lego[i + j]->draw(Pmatrix, VMatrix);

		if (state.drumMap[DRUM_HIHAT_CLOSED])
			for (int j = 4; j < 6; j++)
				lego[i + j]->draw(Pmatrix, VMatrix);

		if (state.drumMap[DRUM_PLUCK])
			for (int j = 6; j < 10; j++)
				lego[i + j]->draw(Pmatrix, VMatrix);
	}

	glfwSwapBuffers(window);
}

void CController::shadersInit(void) {
	vector<GLuint> shaders;

	// Init skybox shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skyboxShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skyboxShader.frag"));
	skyboxShaderProgram.program = pgr::createProgram(shaders);

		// Get uniform locations
		skyboxShaderProgram.PVMMatrixLocation = glGetUniformLocation(skyboxShaderProgram.program, "PVMMatrix");
		// Get input locations
		skyboxShaderProgram.posLocation = glGetAttribLocation(skyboxShaderProgram.program, "position");

	shaders.clear();

	// Init lego shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/commonShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/commonShader.frag"));
	legoShaderProgram.program = pgr::createProgram(shaders);

		// Get uniform locations
		legoShaderProgram.PVMMatrixLocation = glGetUniformLocation(legoShaderProgram.program, "PVMMatrix");
		legoShaderProgram.VMatrixLocation = glGetUniformLocation(legoShaderProgram.program, "VMatrix");
		legoShaderProgram.MMatrixLocation = glGetUniformLocation(legoShaderProgram.program, "MMatrix");
		legoShaderProgram.normalMatrixLocation = glGetUniformLocation(legoShaderProgram.program, "normalMatrix");
		legoShaderProgram.diffuseLocation = glGetUniformLocation(legoShaderProgram.program, "material.diffuse");
		legoShaderProgram.ambientLocation = glGetUniformLocation(legoShaderProgram.program, "material.ambient");
		legoShaderProgram.specularLocation = glGetUniformLocation(legoShaderProgram.program, "material.specular");
		legoShaderProgram.shininessLocation = glGetUniformLocation(legoShaderProgram.program, "material.shininess");
		// Get input locations
		legoShaderProgram.posLocation = glGetAttribLocation(legoShaderProgram.program, "position");
		legoShaderProgram.normalLocation = glGetAttribLocation(legoShaderProgram.program, "normal");

	shaders.clear();
}

void CController::modelsInit(void) {
	// skybox
	skybox = new CSkybox(glm::vec3(0.0f), glm::vec3(100.0f), &skyboxShaderProgram);
	
	// lego
	legoData = new CLoadedObj * [3];
	legoData[0] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(0.0f), glm::vec3(1.0f), &legoShaderProgram); // kick
	legoData[1] = new CLoadedObj(MODEL_LEGO_7, glm::vec3(0.0f), glm::vec3(1.0f), &legoShaderProgram); // hihat
	legoData[2] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(0.0f), glm::vec3(1.0f), &legoShaderProgram); // pluck

	lego = new CLoadedObj * [LEGO_BRICKS_LOOPS * 10];
	glm::vec2 kickPos  = glm::vec2(LEGO_BRICKS_DIST * glm::cos(glm::radians(45.0f)), LEGO_BRICKS_DIST * glm::sin(glm::radians(45.0f)));
	glm::vec2 hihatPos = glm::vec2(LEGO_BRICKS_DIST * 1.0f, LEGO_BRICKS_DIST * 0.0f);
	glm::vec2 pluckPos = glm::vec2(LEGO_BRICKS_DIST * glm::cos(glm::radians(22.5f)), LEGO_BRICKS_DIST * glm::sin(glm::radians(22.5f)));
	for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10) {
		// kick bricks
		lego[i + 0] = new CLoadedObj(MODEL_LEGO_8, glm::vec3( kickPos.x,  kickPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[0]);
		lego[i + 1] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(-kickPos.x,  kickPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[0]);
		lego[i + 2] = new CLoadedObj(MODEL_LEGO_8, glm::vec3(-kickPos.x, -kickPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[0]);
		lego[i + 3] = new CLoadedObj(MODEL_LEGO_8, glm::vec3( kickPos.x, -kickPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[0]);
		// hihat bricks
		lego[i + 4] = new CLoadedObj(MODEL_LEGO_7, glm::vec3( hihatPos.x, hihatPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[1]);
		lego[i + 5] = new CLoadedObj(MODEL_LEGO_7, glm::vec3(-hihatPos.x, hihatPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[1]);
		// pluck bricks
		lego[i + 6] = new CLoadedObj(MODEL_LEGO_9, glm::vec3( pluckPos.x,  pluckPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[2]);
		lego[i + 7] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(-pluckPos.x,  pluckPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[2]);
		lego[i + 8] = new CLoadedObj(MODEL_LEGO_9, glm::vec3(-pluckPos.x, -pluckPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[2]);
		lego[i + 9] = new CLoadedObj(MODEL_LEGO_9, glm::vec3( pluckPos.x, -pluckPos.y, i / 10 * LEGO_BRICKS_DIST), glm::vec3(1.0f), &legoShaderProgram, legoData[2]);
	}
}

void CController::update(void) {
	if (state.keyMap[KEY_UP] || controller.state.keyMap[KEY_W]) camera.move(STEP_LENGTH);
	if (state.keyMap[KEY_DOWN] || controller.state.keyMap[KEY_S]) camera.move(-STEP_LENGTH);
	if (state.keyMap[KEY_LEFT] || controller.state.keyMap[KEY_A]) camera.sideStep(-STEP_LENGTH);
	if (state.keyMap[KEY_RIGHT] || controller.state.keyMap[KEY_D]) camera.sideStep(STEP_LENGTH);
	if (state.keyMap[KEY_Q]) camera.roll(VIEW_ANGLE_DELTA);
	if (state.keyMap[KEY_E]) camera.roll(-VIEW_ANGLE_DELTA);

	for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i++) lego[i]->rotate(glfwGetTime());
}

void CController::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	if (status == MIDI_NOTE_ON_CH02) {
		switch (note) {
			case MIDI_DRUM_KICK1:
				state.drumMap[DRUM_KICK1] = true;
				for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10)
					for (int j = 0; j < 4; j++)
						lego[i + j]->switchRotAxis(glfwGetTime());
				break;
			case MIDI_DRUM_HIHAT_CLOSED:
				state.drumMap[DRUM_HIHAT_CLOSED] = true;
				for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10)
					for (int j = 4; j < 6; j++)
						lego[i + j]->switchRotAxis(glfwGetTime());
				break;
			case MIDI_DRUM_SNARE2:
				state.drumMap[DRUM_PLUCK] = true;
				for (int i = 0; i < LEGO_BRICKS_LOOPS * 10; i += 10)
					for (int j = 6; j < 10; j++)
						lego[i + j]->switchRotAxis(glfwGetTime());
				break;
			default: 
				cout  << "Unresolved midi note:" << status << " " << note << " " << velocity << endl;
				break;
		}
	}
}