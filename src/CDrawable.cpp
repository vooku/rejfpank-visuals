#include "CDrawable.hpp"

using namespace std;

CDrawable::CDrawable (const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram)
	: CObject (position),
	scale (scale),
	shaderProgram (shaderProgram) { }
