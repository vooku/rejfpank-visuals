/**
 * @author	Vadim Petrov
 * @date	2015
 */

#ifndef _REJFPANK_CSKYBOX
#define _REJFPANK_CSKYBOX

#include "CDrawable.hpp"

static const unsigned int nCubeVertices = 8;
static const unsigned int nCubeAttribsPerVertex = 6;
static const float cubeVertices[nCubeVertices * nCubeAttribsPerVertex] = {
	// x      y      z       nx     ny    nz
	-1.0f, -1.0f, -1.0f,  -1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,  -1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,  -1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,  -1.0f,  1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,   1.0f,  1.0f,  1.0f
};

static const unsigned int nCubeTriangles = 36;
static const unsigned short cubeTriangles[nCubeTriangles] = {
	0, 2, 4,   4, 2, 6, // back face
	0, 1, 2,   2, 1, 3, // left face
	1, 5, 3,   3, 5, 7, // front face
	5, 4, 7,   7, 4, 6, // right face
	3, 7, 2,   2, 7, 6, // upper face
	0, 4, 1,   1, 4, 5, // lower face
};

class CSkybox : public CDrawable {
public:
	CSkybox(const glm::vec3 & position, const glm::vec3 & scale, TCommonShaderProgram * shaderProgram);

	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void reset(void); ///< Resets rotation and color multiplier

	GLfloat m_colorMultiplier;
protected:
	void sendUniforms(void);
};

#endif // !_REJFPANK_CSKYBOX
