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
	CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram, const char * texName = "NO_TEX", GLint texLoc = 0);
	~CBanner(void);

	bool setColor(const glm::vec3 & color);
	void updateAlpha(const double & time);
	
	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix, bool inverse);

protected:
	void sendUniforms(void);

	bool m_useTex;
	bool m_inverse;

	glm::vec3 m_color;
	float m_alpha;
	const CCamera * m_camera;
};

#endif // !_REJFPANK_CBANNER
