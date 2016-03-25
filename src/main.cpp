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

#include "pgr/Shader.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

CSkybox * skybox;
CLoadedObj * lego1;
TCommonShaderProgram skyboxShaderProgram;
TCommonShaderProgram legoShaderProgram;

void redraw(GLFWwindow* window) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 Vmatrix = glm::lookAt(
		CAMERA_INIT_POS,
		CAMERA_INIT_POS + CAMERA_INIT_DIR,
		CAMERA_INIT_UP);

	glm::mat4 Pmatrix = glm::perspective(
		CAMERA_VIEW_ANGLE,
		controlState.winWidth / (float)controlState.winHeight,
		CAMERA_VIEW_START,
		CAMERA_VIEW_DIST);

	skybox->draw(Pmatrix, Vmatrix);
	lego1->draw(Pmatrix, Vmatrix);

	glfwSwapBuffers(window);
}

static void errorCallback(int error, const char* description) {
	cerr << "Error " << error << ": " << description << endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action = GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_F11:
				controlState.switchControl(CTRL_FULLSCREEN);
				//TODO waiting for GLFW 3.2
		}
	}
}

static void winRefreshCallback(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &controlState.winWidth, &controlState.winHeight);
	glViewport(0, 0, controlState.winWidth, controlState.winHeight);

	redraw(window);
}

void callbacksInit(GLFWwindow * window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetWindowRefreshCallback(window, winRefreshCallback);
}

void shadersInit(void) {
	vector <GLuint> shaders;
	
	// Init skybox shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skyboxShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skyboxShader.frag"));
	skyboxShaderProgram.program = pgr::createProgram(shaders);

		// Get uniform locations
		skyboxShaderProgram.PVMmatrixLocation = glGetUniformLocation(skyboxShaderProgram.program, "PVMmatrix");
		// Get input locations
		skyboxShaderProgram.posLocation = glGetAttribLocation(skyboxShaderProgram.program, "position");

	shaders.clear();

	// Init lego shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skyboxShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skyboxShader.frag"));
	legoShaderProgram.program = pgr::createProgram(shaders);

		// Get uniform locations
		legoShaderProgram.PVMmatrixLocation = glGetUniformLocation(legoShaderProgram.program, "PVMmatrix");
		// Get input locations
		legoShaderProgram.posLocation = glGetAttribLocation(legoShaderProgram.program, "position");

	shaders.clear();
}

void modelsInit(void) {
	skybox = new CSkybox(glm::vec3(0.0f), glm::vec3(100.0f), &skyboxShaderProgram);
	lego1 = new CLoadedObj("../res/lego.obj", glm::vec3(0.0f), glm::vec3(1.0f), &legoShaderProgram);
}

void controlStateInit(void) {
	controlState.winWidth = INIT_WIN_WIDTH;
	controlState.winHeight = INIT_WIN_HEIGHT;
}

void rejfpankInit(GLFWwindow * window) {
	controlStateInit();
	shadersInit();
	modelsInit();
	callbacksInit(window);

	glClearColor(0.3f, 0.0f, 0.1f, 1.0f);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0, (GLsizei)INIT_WIN_WIDTH, (GLsizei)INIT_WIN_HEIGHT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL/GL_LINE

	glfwSwapInterval(1);
}

void rejfpankFin(void) {
	delete skybox;
	delete lego1;
}

int main (void) {
	// MIDI init
	if (!cMIDIControl.init()) {
		getchar();
		return -1;
	}

	// GLFW init
	GLFWwindow * window;
	glfwSetErrorCallback (errorCallback);

	if (!glfwInit()) return 1;
	
	window = glfwCreateWindow (INIT_WIN_WIDTH, INIT_WIN_HEIGHT, WIN_TITLE, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent (window);
	
	// GLEW init
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		cerr << "Error: " << glewGetErrorString(err) << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 2;
	}
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	// Leftover inits -- controlState, models, shaders, callbacks
	rejfpankInit(window);
	
	// main loop
	while (!glfwWindowShouldClose(window)) {
		redraw(window);
		glfwPollEvents();
	}

	glfwDestroyWindow (window);
	glfwTerminate ();
	
	return 0;
}
