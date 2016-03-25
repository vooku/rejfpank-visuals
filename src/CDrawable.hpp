/**
 * @author	Vadim Petrov
 * @date	2015
 */

#ifndef _REJFPANK_CDRAWABLE
#define _REJFPANK_CDRAWABLE

#include "CObject.hpp"
#include "TCommonShaderProgram.hpp"
#include <vector>

class CDrawable : public CObject {
public:
	CDrawable (const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram);
	
	virtual void draw (const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix) = 0;
	
protected:
	glm::vec3 scale;

	TCommonShaderProgram * shaderProgram;

	struct MeshGeometry {
		GLuint vertexBufferObject;
		GLuint elementBufferObject;
		GLuint vertexArrayObject;

		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<GLushort>  elements;

		// material
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		GLuint texture;
	} geometry;
};

#endif // !_REJFPANK_CDRAWABLE