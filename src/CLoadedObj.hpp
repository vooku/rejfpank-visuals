/**
* @author	Vadim Petrov
* @date	2015
*/

#ifndef _REJFPANK_CLOADEDOBJ
#define _REJFPANK_CLOADEDOBJ

#include "CDrawable.hpp"
#include "TCommonShaderProgram.hpp"

class CLoadedObj : public CDrawable {
public:
	CLoadedObj(const char* filename, const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram);

	void sendCLoadedObjUniforms(const glm::mat4 & PVMmatrix, const glm::mat4 & Vmatrix, const glm::mat4 & Mmatrix);
	void draw(const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix);

protected:
	bool loaded;
};

#endif // !_REJFPANK_CLOADEDOBJ