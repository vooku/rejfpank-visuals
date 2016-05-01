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
	CCamera(void);

	void rotate(const GLdouble offsetX, const GLdouble offsetY);
	void roll(const GLfloat angle);
	void move(const GLfloat stepLen); ///< Move forwards or backwards, the direction is resolved by the + or - sign.
	void sideStep(const GLfloat stepLen); ///< Move left or right, the direction is resolved by the + or - sign.
	void flow(const double time, const int dir); ///< Movement for a period of time
	bool placeOnCircle(const double time, const float r, const glm::vec3 axis); /// Accepts only axes x, y and z for the time being
	
	void reset(void);

	glm::vec3 m_up;
	glm::vec3 m_right;
	bool m_freedom;

	friend void cursorPosCallback(GLFWwindow * window, double x, double y);
protected:
	GLdouble m_lastX;
	GLdouble m_lastY;
	bool m_firstMouse;
};

#endif // !_REJFPAK_CCAMERA