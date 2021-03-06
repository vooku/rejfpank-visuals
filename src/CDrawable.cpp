#include "CDrawable.hpp"

CDrawable::CDrawable (const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram, const float & alpha)
	: CObject (position),
	  m_scale (scale),
	  m_shaderProgram (shaderProgram),
	  m_rotMatrix(glm::mat4(1.0f)),
	  m_pastRotMatrix(glm::mat4(1.0f)),
	  m_alpha(alpha){
	m_axis = glm::vec3(0.0f);
	m_axis[rand() % 3] = 1.0f; // those two lines make it one of the base axes x, y or z
}

CDrawable::~CDrawable(void) {
	glDeleteBuffers(1, &m_geometry.elementBufferObject);
	glDeleteBuffers(1, &m_geometry.elementBufferObject);
	glDeleteBuffers(1, &m_geometry.vertexArrayObject);
	glDeleteTextures(1, &m_geometry.texture);
}

void CDrawable::updateAlpha(const double & time) {
	const double elapsedTime = time - m_triggerTime;
	m_alpha = (-1.0f / 3.0f) * (float)elapsedTime + 1.0f;
}

void CDrawable::rotate(const float angle, const glm::vec3 & axis) {
	m_rotMatrix = glm::rotate(m_rotMatrix, angle, axis);
}

void CDrawable::scale(const glm::vec3 & scale) {
	m_scale = scale;
}

void CDrawable::rotate(const double & time) {
	const double elapsedTime = time - m_triggerTime;
	if (elapsedTime >= ROTATION_TIME) return;
	float angle = ROTATION_SPEED * glm::sin(glm::radians(90.0f) / ROTATION_TIME * (float)elapsedTime); // the first quarter of sinus makes the angle rise slower and slower
	m_rotMatrix = glm::rotate(glm::mat4(1.0f), angle, m_axis);
}

void CDrawable::switchRotAxis(const double & time) {
	glm::vec3 newAxis = glm::vec3(0.0f);
	newAxis[rand() % 3] = 1.0f; // those two lines make it one of the base axes x, y or z
	m_axis = newAxis;
	m_triggerTime = time;
	m_pastRotMatrix *= m_rotMatrix;
	this->rotate(time);
}
