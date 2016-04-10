#include "CCamera.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;

CCamera::CCamera (void)
	: CObject (CAMERA_INIT_POS),
	  up (glm::normalize(CAMERA_INIT_UP)),
	  freedom (CAMERA_INIT_FREE),
	  firstMouse(true) {

	direction = glm::normalize(CAMERA_INIT_DIR);
	right = glm::normalize (glm::cross (direction, up));
}

void CCamera::rotate (const GLfloat offsetX, const GLfloat offsetY) {
	if (!freedom) return;
	
	glm::quat pitchQuat = glm::angleAxis (offsetY, right);
	direction = glm::normalize (glm::rotate (pitchQuat, direction));
	up = glm::normalize (glm::cross (right, direction));

	glm::quat yawQuat = glm::angleAxis (offsetX, up);
	direction = glm::normalize (glm::rotate (yawQuat, direction));
	right = glm::normalize (glm::cross (direction, up));
}

void CCamera::roll (const GLfloat angle) {
	if (!freedom) return;
	glm::quat rollQuat = glm::angleAxis (angle, direction);
	up = glm::normalize (glm::rotate (rollQuat, up));
	right = glm::normalize (glm::cross (direction, up));
}

void CCamera::move (const GLfloat stepLen) {
	if (!freedom) return;
	
	glm::vec3 newPos = position + stepLen * direction;
	if (abs (newPos.x) >= 100.f || abs (newPos.y) >= 100.f || abs (newPos.z) >= 100.f) return;
	position = newPos;
}

void CCamera::sideStep (const GLfloat stepLen) {
	if (!freedom) return;
	
	glm::vec3 newPos = position + stepLen * right;
	if (abs (newPos.x) >= 100.f || abs (newPos.y) >= 100.f || abs (newPos.z) >= 100.f) return;
	position = newPos;
}

void CCamera::reset(void) {
	position = CAMERA_INIT_POS;
	direction = CAMERA_INIT_DIR;
	up = CAMERA_INIT_UP;
	right = glm::normalize(glm::cross(direction, up));
}

void CCamera::flow(const GLfloat t) {
	cout << "cam::flow" << endl;
/*	int i = (int)floor(t);
	this->setFlowPts(i);

	glm::vec3 relPos = this->evaluateCurveSegment(t - i);

	position = initPos + relPos;
	direction = glm::normalize(this->evaluateCurveSegment1stDer(t - i));
	//up = CAMERA_INIT_UP;
	right = glm::normalize(glm::cross(direction, up));
	*/
}

void CCamera::setFlowPts(const int i) {
	flowPoints.p0 = curveData[(i - 1) % CURVE_SIZE];
	flowPoints.p1 = curveData[(i + 0) % CURVE_SIZE];
	flowPoints.p2 = curveData[(i + 1) % CURVE_SIZE];
	flowPoints.p3 = curveData[(i + 2) % CURVE_SIZE];
}

glm::vec3 CCamera::evaluateCurveSegment(const GLfloat t) {
	const float t1 = t;
	const float t2 = t1 * t1;
	const float t3 = t1 * t2;

	return 0.5f * (flowPoints.p0 * (-t3 + 2.0f * t2 - t1)
				 + flowPoints.p1 * (3.0f * t3 - 5.0f * t2 + 2.0f)
				 + flowPoints.p2 * (-3.0f * t3 + 4.0f * t2 + t1)
				 + flowPoints.p3 * (t3 - t2));
}

glm::vec3 CCamera::evaluateCurveSegment1stDer(const GLfloat t) {
	const float t1 = t;
	const float t2 = t1 * t1;

	return 0.5f * (flowPoints.p0 * (-3.0f * t2 + 4.0f * t1 - 1.0f)
				 + flowPoints.p1 * (9.0f * t2 - 10.0f * t1)
				 + flowPoints.p2 * (-9.0f * t2 + 8.0f * t1 + 1.0f)
				 + flowPoints.p3 * (3.0f * t2 - 2.0f * t1));
}