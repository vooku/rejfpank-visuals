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
	virtual void sendUniforms(void) = 0;

	glm::vec3 scale;
	TCommonShaderProgram * shaderProgram;

	struct TMeshGeometry {
		GLuint vertexBufferObject;
		GLuint elementBufferObject;
		GLuint vertexArrayObject;
		unsigned int  numTriangles;

		// material
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		GLuint texture;
	} geometry;

	struct TTempMatrices {
		glm::mat4 PVMmatrix;
		glm::mat4 Vmatrix;
		glm::mat4 Mmatrix;
		glm::mat4 normalmatrix;
	} tempMats;
};

#endif // !_REJFPANK_CDRAWABLE