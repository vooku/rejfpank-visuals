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
	CLoadedObj(const char * filename,
			   const glm::vec3 position,
			   const glm::vec3 scale,
			   TCommonShaderProgram * shaderProgram);
	CLoadedObj(const char * filename,
		const glm::vec3 position,
		const glm::vec3 scale,
		TCommonShaderProgram * shaderProgram,
		const CLoadedObj * dataObj);

	bool loadObj(const char * filename);
	void setMaterials(const char * filename);

	void draw(const glm::mat4 & Pmatrix, const glm::mat4 & VMatrix);

protected:
	void sendUniforms(void);

	bool enableDraw; // whether the object was properly initialized (loaded)
	const bool containsData;
	const CLoadedObj * dataObj;

	struct TMaterial {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;

		GLuint texture;
	} material;
};

#endif // !_REJFPANK_CLOADEDOBJ