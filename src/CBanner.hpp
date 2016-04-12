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
	CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram, const bool useTex = true, const char * = TEX_TEST);
	//~CBanner();
	bool setColor(const glm::vec3 & color);
	void updateAlpha(const double & time);
	
	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);

protected:
	void sendUniforms(void);

	const bool m_useTex;
	glm::vec3 m_color;
	float m_alpha;
	const CCamera * m_camera;
};

#endif // !_REJFPANK_CBANNER
