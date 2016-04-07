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
	CDrawable(const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram);
	
	virtual void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) = 0;
	
	void rotate(const double & time);
	void switchRotAxis(const double & time);
		
protected:
	virtual void sendUniforms(void) = 0;

	double triggerTime;
	glm::mat4 rotMatrix;
	glm::mat4 pastRotMatrix; // remembers previous rotations around different axes
	glm::vec3 axis;
	glm::vec3 scale;
	TCommonShaderProgram * shaderProgram;

	struct TMeshGeometry {
		GLuint vertexBufferObject;
		GLuint elementBufferObject;
		GLuint vertexArrayObject;
		unsigned int numTriangles;
	} geometry;

	struct TTempMatrices {
		glm::mat4 PVMMatrix;
		glm::mat4 VMatrix;
		glm::mat4 MMatrix;
		glm::mat4 normalMatrix;
	} tempMats;
};

#endif // !_REJFPANK_CDRAWABLE