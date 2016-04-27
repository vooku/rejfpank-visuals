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
	virtual ~CDrawable(void);

	virtual void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) = 0;
	
	void rotate(const float angle, const glm::vec3 & axis);
	void rotate(const double & time);
	void switchRotAxis(const double & time);
		
protected:
	virtual void sendUniforms(void) = 0;

	glm::mat4 m_rotMatrix;
	glm::mat4 m_pastRotMatrix; // remembers previous rotations around different axes
	glm::vec3 m_axis;
	glm::vec3 m_scale;
	TCommonShaderProgram * m_shaderProgram;

	struct TMeshGeometry {
		GLuint vertexBufferObject;
		GLuint elementBufferObject;
		GLuint vertexArrayObject;
		unsigned int numTriangles;
		GLuint texture;
	} m_geometry;

	struct TTempMatrices {
		glm::mat4 PVMMatrix;
		glm::mat4 VMatrix;
		glm::mat4 MMatrix;
		glm::mat4 normalMatrix;
	} m_tempMats;
};

#endif // !_REJFPANK_CDRAWABLE