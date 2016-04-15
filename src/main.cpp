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

#include <IL/il.h>

#include "data.hpp"
#include "CController.hpp"

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

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
				//controller.m_state.switchState(CTRL_FULLSCREEN);
				//TODO waiting for GLFW 3.2
				break;
			case GLFW_KEY_F:
				controller.m_camera.m_freedom = !controller.m_camera.m_freedom;
				break;
			case GLFW_KEY_UP:
				controller.m_state.keyMap[KEY_UP] = true;
				break;
			case GLFW_KEY_DOWN:
				controller.m_state.keyMap[KEY_DOWN] = true;
				break;
			case GLFW_KEY_LEFT:
				controller.m_state.keyMap[KEY_LEFT] = true;
				break;
			case GLFW_KEY_RIGHT:
				controller.m_state.keyMap[KEY_RIGHT] = true;
				break;
			case GLFW_KEY_W:
				controller.m_state.keyMap[KEY_W] = true;
				break;
			case GLFW_KEY_S:
				controller.m_state.keyMap[KEY_S] = true;
				break;
			case GLFW_KEY_A:
				controller.m_state.keyMap[KEY_A] = true;
				break;
			case GLFW_KEY_D:
				controller.m_state.keyMap[KEY_D] = true;
				break;
			case GLFW_KEY_Q:
				controller.m_state.keyMap[KEY_Q] = true;
				break;
			case GLFW_KEY_E:
				controller.m_state.keyMap[KEY_E] = true;
				break;
			case GLFW_KEY_1:
				controller.m_camera.reset();
				break;
			case GLFW_KEY_P:
				if (mods == GLFW_MOD_CONTROL) controller.nextSong();
				break;
			default: // do nothing;
				break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_UP:
				controller.m_state.keyMap[KEY_UP] = false;
				break;
			case GLFW_KEY_DOWN:
				controller.m_state.keyMap[KEY_DOWN] = false;
				break;
			case GLFW_KEY_LEFT:
				controller.m_state.keyMap[KEY_LEFT] = false;
				break;
			case GLFW_KEY_RIGHT:
				controller.m_state.keyMap[KEY_RIGHT] = false;
				break;
			case GLFW_KEY_W:
				controller.m_state.keyMap[KEY_W] = false;
				break;
			case GLFW_KEY_S:
				controller.m_state.keyMap[KEY_S] = false;
				break;
			case GLFW_KEY_A:
				controller.m_state.keyMap[KEY_A] = false;
				break;
			case GLFW_KEY_D:
				controller.m_state.keyMap[KEY_D] = false;
				break;
			case GLFW_KEY_Q:
				controller.m_state.keyMap[KEY_Q] = false;
				break;
			case GLFW_KEY_E:
				controller.m_state.keyMap[KEY_E] = false;
				break;
			default: // do nothing;
				break;
		}
	}
}

void cursorPosCallback(GLFWwindow * window, double x, double y) {
	if (controller.m_camera.m_firstMouse) {
		controller.m_camera.m_lastX = x;
		controller.m_camera.m_lastY = y;
		controller.m_camera.m_firstMouse = false;
	}

	GLdouble offsetX = controller.m_camera.m_lastX - x;
	GLdouble offsetY = controller.m_camera.m_lastY - y;
	controller.m_camera.m_lastX = x;
	controller.m_camera.m_lastY = y;

	// Ignore movement induced by glfwSetCursorPos
	if (x == controller.m_state.winWidth / 2 && y == controller.m_state.winHeight / 2) return;
	// If the movement was 'real,' warp the cursor back
	glfwSetCursorPos(window, controller.m_state.winWidth / 2, controller.m_state.winHeight / 2);

	offsetX *= MOUSE_SENSITIVITY;
	offsetY *= MOUSE_SENSITIVITY;
	controller.m_camera.rotate(offsetX, offsetY);
}

static void winRefreshCallback(GLFWwindow * window) {
	glfwGetFramebufferSize(window, &controller.m_state.winWidth, &controller.m_state.winHeight);
	glViewport(0, 0, controller.m_state.winWidth, controller.m_state.winHeight);

	controller.redraw(window);
}

void callbacksInit(GLFWwindow * window) {
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetWindowRefreshCallback(window, winRefreshCallback);
}

void rejfpankInit(GLFWwindow * window) {
	srand((unsigned int) time(NULL));

	callbacksInit(window);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0, (GLsizei) controller.m_state.winWidth, (GLsizei) controller.m_state.winHeight);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_FILL/GL_LINE

	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	controller.init();
	controller.m_state.ctrlMap[CTRL_INIT] = true; // for synchro, now all is initiated and no void pointer conflicts etc should occur
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
	else selectedMonitor = SELECT_MONITOR_DEFAULT;
	cout << "Selected monitor " << selectedMonitor << "." << endl;
	selectedMonitor--; // the monitors display as starting from 1 instead of 0

	const GLFWvidmode * mode = glfwGetVideoMode(monitors[selectedMonitor]);
	controller.m_state.winWidth = mode->width;
	controller.m_state.winHeight = mode->height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	return glfwCreateWindow(controller.m_state.winWidth, controller.m_state.winHeight = mode->height, WIN_TITLE, monitors[selectedMonitor], NULL);
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

	//DevIL
	ilInit();

	// Leftover inits -- controller, models, shaders, callbacks
	rejfpankInit(window);

	// main loop
	while (!glfwWindowShouldClose(window)) {
		controller.redraw(window);
		controller.update();
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}
