/**
 * @author	Vadim Petrov
 * @date	2015
 */

#ifndef _REJFPANK_CSKYBOX
#define _REJFPANK_CSKYBOX

#include "CDrawable.hpp"

class CSkybox : public CDrawable {
public:
	CSkybox (const glm::vec3 position, const glm::vec3 scale);

	void draw (const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix);
protected:
	void sendSkyboxUniforms (const glm::mat4 & PVMmatrix, const glm::mat4 & Vmatrix, const glm::mat4 & Mmatrix);
};

extern TCommonShaderProgram skyboxShaderProgram;

#endif // !_REJFPANK_CSKYBOX
