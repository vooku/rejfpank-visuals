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

	void rotate(const GLfloat offsetX, const GLfloat offsetY);
	void roll(const GLfloat angle);
	void move(const GLfloat stepLen); ///< Move forwards or backwards, the direction is resolved by the + or - sign.
	void sideStep(const GLfloat stepLen); ///< Move left or right, the direction is resolved by the + or - sign.
	void reset(void);

	void flow(const GLfloat t); ///< Move on spline.
	void setFlowPts(const int i);
	glm::vec3 evaluateCurveSegment(const GLfloat t);
	glm::vec3 evaluateCurveSegment1stDer(const GLfloat t);

	glm::vec3 up;
	glm::vec3 right;
	bool freedom;

	friend void cursorPosCallback(GLFWwindow * window, double x, double y);
protected:
	GLdouble lastX;
	GLdouble lastY;
	bool firstMouse;

	glm::vec3 * curveData;
	struct CurveControlPoints {
		glm::vec3 p0, p1, p2, p3;
	} flowPoints;
};

#endif // !_REJFPAK_CCAMERA