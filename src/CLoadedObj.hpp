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
	bool loadObj(const char * filename);
	void draw(const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix);

protected:
	void sendUniforms();

	bool loaded;
};

#endif // !_REJFPANK_CLOADEDOBJ