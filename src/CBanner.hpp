/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CBANNER
#define _REJFPANK_CBANNER

#include "CDrawable.hpp"
#include "CCamera.hpp"

class CBanner : public CDrawable {
public:
	CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram, const char * param = BANNER_PARAM_NO_TEX, GLint texLoc = 0);
	~CBanner(void);

	bool setColor(const glm::vec3 & color);
	void updateAlpha(const double & time);
	void tear(void);
	void untear(void);

	
	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix, bool inverse, bool reducePalette);

protected:
	void sendUniforms(void);

	bool m_useTex;
	bool m_inverse;
	bool m_reducePalette;
	bool m_tear;

	int m_tearN;
	float m_tearBorders[SCREEN_TEARS];
	GLfloat m_tearOffsets[SCREEN_TEARS + 1];

	glm::vec3 m_color;
	float m_alpha;
	const CCamera * m_camera;
};

#endif // !_REJFPANK_CBANNER
