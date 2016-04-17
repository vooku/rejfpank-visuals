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

void CCamera::rotate (const GLfloat offsetX, const GLfloat offsetY) {
	if (!m_freedom) return;
	
	glm::quat pitchQuat = glm::angleAxis (offsetY, m_right);
	m_direction = glm::normalize (glm::rotate (pitchQuat, m_direction));
	m_up = glm::normalize (glm::cross (m_right, m_direction));

	glm::quat yawQuat = glm::angleAxis (offsetX, m_up);
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

void CCamera::reset(void) {
	m_position = CAMERA_INIT_POS;
	m_direction = CAMERA_INIT_DIR;
	m_up = CAMERA_INIT_UP;
	m_right = glm::normalize(glm::cross(m_direction, m_up));
}