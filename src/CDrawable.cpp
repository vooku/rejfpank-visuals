#include "CDrawable.hpp"

using namespace std;

CDrawable::CDrawable (const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram)
	: CObject (position),
	  scale (scale),
	  shaderProgram (shaderProgram),
	  rotMatrix(glm::mat4(1.0f)),
	  pastRotMatrix(glm::mat4(1.0f)) {
	axis = glm::vec3(0.0f);
	axis[rand() % 3] = 1.0f; // those two lines make it one of the base axes x, y or z
}

void CDrawable::rotate(const double & time) {
	const double elapsedTime = time - triggerTime;
	if (elapsedTime >= ROTATION_TIME) return;
	const float angle = ROTATION_SPEED * glm::sin(glm::radians(90.0f) / ROTATION_TIME * elapsedTime); // the first quarter of sinus makes the angle rise slower and slower
	rotMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void CDrawable::switchRotAxis(const double & time) {
	glm::vec3 newAxis = glm::vec3(0.0f);
	newAxis[rand() % 3] = 1.0f; // those two lines make it one of the base axes x, y or z
	axis = newAxis;
	triggerTime = time;
	pastRotMatrix *= rotMatrix;
	this->rotate(time);
}
