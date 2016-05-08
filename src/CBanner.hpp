/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CBANNER
#define _REJFPANK_CBANNER

#include "CDrawable.hpp"
#include "CCamera.hpp"

const static unsigned int nBannerVertices = 4;
const static unsigned int nBannerAttribsPerVertex = 5;
extern float bannerVertices[];

class CBanner : public CDrawable {
public:
	CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram, const char * param = BANNER_PARAM_NO_TEX, GLint multipassTexLoc = 0);
	~CBanner(void);

	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix, bool inverse, bool reducePalette, const int colorShift = 0);

	bool setColor(const glm::vec3 & color);
	void tear(void);
	void untear(void);

protected:
	void sendUniforms(void);

	bool m_useTex;
	bool m_inverse;
	bool m_reducePalette;
	bool m_tear;

	int m_colorShift;

	int m_tearN;
	float m_tearBorders[SCREEN_TEARS];
	GLfloat m_tearOffsets[SCREEN_TEARS + 1];

	glm::vec3 m_color;
	const CCamera * m_camera;
};

#endif // !_REJFPANK_CBANNER
