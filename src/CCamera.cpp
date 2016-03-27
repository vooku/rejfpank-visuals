#include "CCamera.hpp"
#include "TControlState.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;

CCamera camera;

CCamera::CCamera (void)
	: CObject (CAMERA_INIT_POS),
	  up (glm::normalize(CAMERA_INIT_UP)),
	  freedom (false),
	  lastX (controlState.winWidth / 2.0f),
	  lastY (controlState.winHeight / 2.0f) {

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
