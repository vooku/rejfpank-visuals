/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CBANNER
#define _REJFPANK_CBANNER

#include "CDrawable.hpp"

class CBanner : public CDrawable {
public:
	CBanner(const glm::vec3 position, TCommonShaderProgram * shaderProgram);
	//~CBanner();
	
	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);

protected:
	void sendUniforms(void);
};

#endif // !_REJFPANK_CBANNER
