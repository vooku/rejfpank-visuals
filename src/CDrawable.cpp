#include "CDrawable.hpp"

using namespace std;

CDrawable::CDrawable (const glm::vec3 position, const glm::vec3 scale, const unsigned int nTriangles) : 
	CObject (position),
	scale (scale) {
	
	geometry.numTriangles = nTriangles;
/*
	geometry.ambient = glm::vec3 (0.1f);
	geometry.diffuse = glm::normalize (randomVec3 (5.0f) + glm::vec3 (1.0f)); // the parameter 5.0f is not very important, it should just be > 1
	geometry.specular = glm::vec3 (0.5f);
	geometry.shininess = 3.0f;
*/
}
