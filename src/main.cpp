/*
 * Copyright (C) 2016 Vadim Petrov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
/**
 * @author Vadim Petrov
 * @date 2016
 */

#include "CMIDIControl.hpp"

//#define GLFW_INCLUDE_GLU // only if I actually need GLU (i probably won't)
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

static void errorCallback(int error, const char* description) {
	fputs(description, stderr);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

int main (void) {
	if (!cMIDIControl.init()) {
		getchar();
		return -1;
	}
	




	// ----------------------------------------------------- glfw stuff
	GLFWwindow* window;
	glfwSetErrorCallback (errorCallback);

	if (!glfwInit()) return 1;

	window = glfwCreateWindow (640, 480, "Simple example", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent (window);
	glfwSwapInterval (1);
	glfwSetKeyCallback (window, keyCallback);
	

	// main loop
	while (!glfwWindowShouldClose(window)) {
		float ratio;
		int width, height;

		glfwGetFramebufferSize (window, &width, &height);
		ratio = width / (float) height;
		glViewport (0, 0, width, height);
		glClear (GL_COLOR_BUFFER_BIT);
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho (-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glRotatef ((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin (GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow (window);
	glfwTerminate ();

	return 0;
}
