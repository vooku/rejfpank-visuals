/*
 * Copyright (C) 2016 Vadim Petrov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
/**
 * @author	Vadim Petrov
 * @date	2016
 */

#include "CMIDIControl.hpp" // has to be included before glfw!

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
//#include <glm/gtx/rotate_vector.hpp>

#include "data.hpp"
#include "TControlState.hpp"
#include "CSkybox.hpp"
#include "CLoadedObj.hpp"
#include "CCamera.hpp"

#include "pgr/Shader.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

CSkybox * skybox;
CLoadedObj ** lego;
TCommonShaderProgram skyboxShaderProgram;
TCommonShaderProgram legoShaderProgram;
TControlState controlState;

void redraw(GLFWwindow * window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 VMatrix = glm::lookAt(
		camera.position,
		camera.position + camera.direction,
		camera.up);

	glm::mat4 Pmatrix = glm::perspective(
		glm::radians(CAMERA_VIEW_ANGLE),
		controlState.winWidth / (float) controlState.winHeight,
		CAMERA_VIEW_START,
		CAMERA_VIEW_DIST);

	skybox->draw(Pmatrix, VMatrix);
	for (int i = 0; i < LEGO_BRICKS_COUNT; i++) lego[i]->draw(Pmatrix, VMatrix);

	glfwSwapBuffers(window);
}

static void errorCallback(int error, const char* description) {
	cerr << "Error " << error << ": " << description << endl;
}

static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_F11:
				controlState.switchState(CTRL_FULLSCREEN);
				//TODO waiting for GLFW 3.2
				break;
			case GLFW_KEY_F:
				camera.freedom = !camera.freedom;
				break;
			case GLFW_KEY_UP:
				controlState.keyMap[KEY_UP] = true;
				break;
			case GLFW_KEY_DOWN:
				controlState.keyMap[KEY_DOWN] = true;
				break;
			case GLFW_KEY_LEFT:
				controlState.keyMap[KEY_LEFT] = true;
				break;
			case GLFW_KEY_RIGHT:
				controlState.keyMap[KEY_RIGHT] = true;
				break;
			case GLFW_KEY_W:
				controlState.keyMap[KEY_W] = true;
				break;
			case GLFW_KEY_S:
				controlState.keyMap[KEY_S] = true;
				break;
			case GLFW_KEY_A:
				controlState.keyMap[KEY_A] = true;
				break;
			case GLFW_KEY_D:
				controlState.keyMap[KEY_D] = true;
				break;
			case GLFW_KEY_Q:
				controlState.keyMap[KEY_Q] = true;
				break;
			case GLFW_KEY_E:
				controlState.keyMap[KEY_E] = true;
				break;
			case GLFW_KEY_1:
				camera.position = CAMERA_INIT_POS;
				camera.direction = CAMERA_INIT_DIR;
				camera.up = CAMERA_INIT_UP;
				camera.right = glm::normalize(glm::cross(camera.direction, camera.up));
				break;
			default: // do nothing;
				break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_UP:
				controlState.keyMap[KEY_UP] = false;
				break;
			case GLFW_KEY_DOWN:
				controlState.keyMap[KEY_DOWN] = false;
				break;
			case GLFW_KEY_LEFT:
				controlState.keyMap[KEY_LEFT] = false;
				break;
			case GLFW_KEY_RIGHT:
				controlState.keyMap[KEY_RIGHT] = false;
				break;
			case GLFW_KEY_W:
				controlState.keyMap[KEY_W] = false;
				break;
			case GLFW_KEY_S:
				controlState.keyMap[KEY_S] = false;
				break;
			case GLFW_KEY_A:
				controlState.keyMap[KEY_A] = false;
				break;
			case GLFW_KEY_D:
				controlState.keyMap[KEY_D] = false;
				break;
			case GLFW_KEY_Q:
				controlState.keyMap[KEY_Q] = false;
				break;
			case GLFW_KEY_E:
				controlState.keyMap[KEY_E] = false;
				break;
			default: // do nothing;
				break;
		}
	}
}

void cursorPosCallback(GLFWwindow * window, double x, double y) {
	if (camera.firstMouse) {
		camera.lastX = x;
		camera.lastY = y;
		camera.firstMouse = false;
	}

	GLdouble offsetX = camera.lastX - x;
	GLdouble offsetY = camera.lastY - y;
	camera.lastX = x;
	camera.lastY = y;

	// Ignore movement induced by glfwSetCursorPos
	if (x == controlState.winWidth / 2 && y == controlState.winHeight / 2) return;
	// If the movement was 'real,' warp the cursor back
	glfwSetCursorPos(window, controlState.winWidth / 2, controlState.winHeight / 2);

	offsetX *= MOUSE_SENSITIVITY;
	offsetY *= MOUSE_SENSITIVITY;
	camera.rotate(offsetX, offsetY);
}

static void winRefreshCallback(GLFWwindow * window) {
	glfwGetFramebufferSize(window, &controlState.winWidth, &controlState.winHeight);
	glViewport(0, 0, controlState.winWidth, controlState.winHeight);

	redraw(window);
}

void callbacksInit(GLFWwindow * window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetWindowRefreshCallback(window, winRefreshCallback);
}

void shadersInit(void) {
	vector <GLuint> shaders;
	
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

void modelsInit(void) {
	skybox = new CSkybox(glm::vec3(0.0f), glm::vec3(100.0f), &skyboxShaderProgram);
	lego = new CLoadedObj * [LEGO_BRICKS_COUNT];
	lego[0] = new CLoadedObj(MODEL_LEGO, glm::vec3(0.0f, -2.0f, -2.0f), glm::vec3(1.0f), &legoShaderProgram);
	lego[1] = new CLoadedObj(MODEL_LEGO, glm::vec3(0.0f, -2.0f, 2.0f), glm::vec3(1.0f), &legoShaderProgram);
	lego[2] = new CLoadedObj(MODEL_LEGO, glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(1.0f), &legoShaderProgram);
	lego[3] = new CLoadedObj(MODEL_LEGO, glm::vec3(0.0f, 2.0f, -2.0f), glm::vec3(1.0f), &legoShaderProgram);
	lego[4] = new CLoadedObj(MODEL_LEGO, glm::vec3(3.0f, -2.0f, -2.0f), glm::vec3(1.0f), &legoShaderProgram);
	lego[5] = new CLoadedObj(MODEL_LEGO, glm::vec3(3.0f, -2.0f, 2.0f), glm::vec3(1.0f), &legoShaderProgram);
	lego[6] = new CLoadedObj(MODEL_LEGO, glm::vec3(3.0f, 2.0f, 2.0f), glm::vec3(1.0f), &legoShaderProgram);
	lego[7] = new CLoadedObj(MODEL_LEGO, glm::vec3(3.0f, 2.0f, -2.0f), glm::vec3(1.0f), &legoShaderProgram);
}

void controlStateInit(void) {
	controlState.winWidth = INIT_WIN_WIDTH;
	controlState.winHeight = INIT_WIN_HEIGHT;
}

void rejfpankInit(GLFWwindow * window) {
	srand((unsigned int) time(NULL));

	controlStateInit();
	shadersInit();
	modelsInit();
	callbacksInit(window);

	glClearColor(0.3f, 0.0f, 0.1f, 1.0f);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0, (GLsizei) controlState.winWidth, (GLsizei) controlState.winHeight);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL/GL_LINE

	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void rejfpankFin(void) {
	delete skybox;
	for (int i=0; i<LEGO_BRICKS_COUNT; i++) delete lego[i];
	delete[] lego;
}

void update(void) {
	if (controlState.keyMap[KEY_UP] || controlState.keyMap[KEY_W]) camera.move(STEP_LENGTH);
	if (controlState.keyMap[KEY_DOWN] || controlState.keyMap[KEY_S]) camera.move(-STEP_LENGTH);
	if (controlState.keyMap[KEY_LEFT] || controlState.keyMap[KEY_A]) camera.sideStep(-STEP_LENGTH);
	if (controlState.keyMap[KEY_RIGHT] || controlState.keyMap[KEY_D]) camera.sideStep(STEP_LENGTH);
	if (controlState.keyMap[KEY_Q]) camera.roll(VIEW_ANGLE_DELTA);
	if (controlState.keyMap[KEY_E]) camera.roll(-VIEW_ANGLE_DELTA);

	for (int i = 0; i < LEGO_BRICKS_COUNT; i++) lego[i]->rotate(glfwGetTime());
}

GLFWwindow * createWindow(void) {
	int count;
	GLFWmonitor ** monitors = glfwGetMonitors(&count);

	if (count == 1) return glfwCreateWindow(INIT_WIN_WIDTH, INIT_WIN_HEIGHT, WIN_TITLE, NULL, NULL);

	cout << "Available monitors:" << endl;
	for (int i = 0; i < count; i++) cout << "\t" << i + 1 << ":\t" << glfwGetMonitorName(monitors[i]) << endl;

	int selectedMonitor = -1;
	if (SELECT_MONITOR_MAN) {
		string data;
		do {
			cout << "\nPlease select a monitor:" << endl;
			getline(cin, data);
			if (data.length() > 3) continue;
			selectedMonitor = (unsigned int)atoi(data.c_str());
		} while (selectedMonitor < 1 || selectedMonitor > count);
	}
	else selectedMonitor = 2;
	cout << "Selected monitor " << selectedMonitor << "." << endl;
	selectedMonitor--; // the monitors display as starting from 1 instead of 0

	const GLFWvidmode * mode = glfwGetVideoMode(monitors[selectedMonitor]);
	controlState.winWidth = mode->width;
	controlState.winHeight = mode->height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	return glfwCreateWindow(controlState.winWidth, controlState.winHeight = mode->height, WIN_TITLE, monitors[selectedMonitor], NULL);
}

int main (void) {
	// MIDI init
	if (!cMIDIControl.init()) {
		cerr << "Error: Cannot initiate MIDI!" << endl;
		return -1;
	}

	// GLFW init
	GLFWwindow * window;
	glfwSetErrorCallback (errorCallback);

	if (!glfwInit()) {
		cerr << "Error: Cannot initiate GLFW!" << endl;
		return -2;
	}
	// print glfw version
	int glfwMaj, glfwMin, glfwRev;
	glfwGetVersion(&glfwMaj, &glfwMin, &glfwRev);
	cout << "Initialized GLFW " << glfwMaj << "." << glfwMin << "." << glfwRev << endl;

	window = createWindow();
	if (!window) {
		glfwTerminate();
		return -3;
	}
	glfwMakeContextCurrent(window);
	
	// GLEW init
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		cerr << "Error: " << glewGetErrorString(err) << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -4;
	}
	cout << "Initialized GLEW " << glewGetString(GLEW_VERSION) << endl;

	// Leftover inits -- controlState, models, shaders, callbacks
	rejfpankInit(window);
	
	// main loop
	while (!glfwWindowShouldClose(window)) {
		redraw(window);
		update();
		glfwPollEvents();
	}

	glfwDestroyWindow (window);
	glfwTerminate ();
	
	return 0;
}
