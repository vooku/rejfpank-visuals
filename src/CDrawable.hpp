/**
 * @author	Vadim Petrov
 * @date	2015
 */

#ifndef _REJFPANK_CDRAWABLE
#define _REJFPANK_CDRAWABLE

#include "CObject.hpp"
#include "TCommonShaderProgram.hpp"

class CDrawable : public CObject {
public:
	CDrawable (const glm::vec3 position, const glm::vec3 scale, const unsigned int nTriangles);
	
	virtual void draw (const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix) = 0;
	
	struct MeshGeometry {
		GLuint vertexBufferObject; 
		GLuint elementBufferObject;
		GLuint vertexArrayObject;
		unsigned int numTriangles;

		// material
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		GLuint texture;
	} geometry;

protected:
	glm::vec3 scale;
};

#endif // !_REJFPANK_CDRAWABLE