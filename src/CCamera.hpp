/**
 * @file	CCamera.hpp
 * @author	Vadim Petrov
 * @date	2015
*/

#ifndef _REJFPAK_CCAMERA
#define _REJFPAK_CCAMERA

#include "CDrawable.hpp"
#include "data.hpp"

class CCamera : public CObject {
public:
	CCamera ();

	void rotate (const GLfloat offsetX, const GLfloat offsetY);
	void roll (const GLfloat angle);
	void move (const GLfloat stepLen); ///< Move forwards or backwards, the direction is resolved by the + or - sign.
	void sideStep (const GLfloat stepLen); ///< Move left or right, the direction is resolved by the + or - sign.

	glm::vec3 up;
	glm::vec3 right; ///< Last of the vector trio front, up, right.
	bool freedom;

	friend void cursorPosCallback(GLFWwindow * window, double x, double y);
protected:
	GLdouble lastX;
	GLdouble lastY;
	bool firstMouse;
};

extern CCamera camera;

#endif // !_REJFPAK_CCAMERA