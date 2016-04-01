#include "CDrawable.hpp"

using namespace std;

CDrawable::CDrawable (const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram)
	: CObject (position),
	scale (scale),
	shaderProgram (shaderProgram),
	triggerTime(0),
	rotMatrix(glm::mat4(1.0f)),
	axis(glm::normalize(glm::vec3(rand(), rand(), rand()))) { }

void CDrawable::rotate(const double & time) {
	const double elapsedTime = time - triggerTime;
	const float angle = elapsedTime;
	rotMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
}
