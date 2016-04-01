/**
 * @author	Vadim Petrov
 * @date	2015
 */

#ifndef _REJFPANK_CSKYBOX
#define _REJFPANK_CSKYBOX

#include "CDrawable.hpp"

class CSkybox : public CDrawable {
public:
	CSkybox (const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram);

	void draw (const glm::mat4 & Pmatrix, const glm::mat4 & VMatrix);
protected:
	void sendUniforms (void);
};

#endif // !_REJFPANK_CSKYBOX
