#include "CCamera.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

CCamera::CCamera (void)
	: CObject (CAMERA_INIT_POS),
	  m_up (glm::normalize(CAMERA_INIT_UP)),
	  m_freedom (CAMERA_INIT_FREE),
	  m_firstMouse(true) {

	m_direction = glm::normalize(CAMERA_INIT_DIR);
	m_right = glm::normalize (glm::cross (m_direction, m_up));
}

void CCamera::rotate (const GLdouble offsetX, const GLdouble offsetY) {
	if (!m_freedom) return;
	
	glm::quat pitchQuat = glm::angleAxis ((float)offsetY, m_right);
	m_direction = glm::normalize (glm::rotate (pitchQuat, m_direction));
	m_up = glm::normalize (glm::cross (m_right, m_direction));

	glm::quat yawQuat = glm::angleAxis ((float)offsetX, m_up);
	m_direction = glm::normalize (glm::rotate (yawQuat, m_direction));
	m_right = glm::normalize (glm::cross (m_direction, m_up));
}

void CCamera::roll (const GLfloat angle) {
	glm::quat rollQuat = glm::angleAxis (angle, m_direction);
	m_up = glm::normalize (glm::rotate (rollQuat, m_up));
	m_right = glm::normalize (glm::cross (m_direction, m_up));
}

void CCamera::move (const GLfloat stepLen) {
	m_position += stepLen * m_direction;
}

void CCamera::sideStep (const GLfloat stepLen) {
	m_position += stepLen * m_right;
}

void CCamera::flow(const double time, const int dir) {
	const double elapsedTime = time - m_triggerTime;
	if (elapsedTime > FLOW_MAX_TIME) return;

	this->move(dir * STEP_LENGTH);
}

bool CCamera::placeOnCircle(const double time, const float r, const glm::vec3 axis) {
	if (axis == glm::vec3(1.0f, 0.0f, 0.0f)) m_position = glm::vec3(0.0f, r * glm::sin(time), r * glm::cos(time));
	else if (axis == glm::vec3(0.0f, 1.0f, 0.0f)) m_position = glm::vec3(r * glm::sin(time), 0.0f, r * glm::cos(time));
	else if (axis == glm::vec3(0.0f, 0.0f, 1.0f)) m_position = glm::vec3(r * glm::sin(time), r * glm::cos(time), 0.0f);
	else return false;

	m_direction = glm::normalize(-m_position);
	m_right = axis;
	m_up = glm::normalize(glm::cross(m_right, m_direction));

	return true;
}

void CCamera::reset(void) {
	m_position = CAMERA_INIT_POS;
	m_direction = CAMERA_INIT_DIR;
	m_up = CAMERA_INIT_UP;
	m_right = glm::normalize(glm::cross(m_direction, m_up));
}