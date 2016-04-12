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
	glm::quat rollQuat = glm::angleAxis (angle, direction);
	up = glm::normalize (glm::rotate (rollQuat, up));
	right = glm::normalize (glm::cross (direction, up));
}

void CCamera::move (const GLfloat stepLen) {
	position += stepLen * direction;
}

void CCamera::sideStep (const GLfloat stepLen) {
	position += stepLen * right;
}

void CCamera::flow(const double time, const int dir) {
	const double elapsedTime = time - triggerTime;
	if (elapsedTime > FLOW_MAX_TIME) return;

	this->move(dir * STEP_LENGTH);
}

void CCamera::reset(void) {
	position = CAMERA_INIT_POS;
	direction = CAMERA_INIT_DIR;
	up = CAMERA_INIT_UP;
	right = glm::normalize(glm::cross(direction, up));
}