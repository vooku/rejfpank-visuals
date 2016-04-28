/**
 * @author	Vadim Petrov
 * @date	2016
 */

#ifndef _REJFPANK_CLOADEDOBJ
#define _REJFPANK_CLOADEDOBJ

#include "CDrawable.hpp"
#include "TCommonShaderProgram.hpp"

class CLoadedObj : public CDrawable {
public:
	CLoadedObj(const char * filename,
			   const glm::vec3 & position,
			   const glm::vec3 & scale,
			   TCommonShaderProgram * shaderProgram,
			   const char * texname = NULL,
			   const bool randomizeUV = false,
			   const CLoadedObj * dataObj = NULL,
			   const unsigned int materialIdx = 0,
			   const float & alpha = 1.0f);

	bool loadObj(const char * filename, const bool randomizeUV);
	void setMaterials(const char * filename);

	void fadeToBlack(void);

	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);

protected:
	void sendUniforms(void);

	bool m_enableDraw; ///< whether the object was properly initialized (loaded)
	bool m_containsData;
	bool m_useTex;
	const CLoadedObj * m_dataObj;

	struct TMaterial {
		unsigned int index; ///< Material hinted from outside

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;

		GLuint texture;
	} m_material;
};

#endif // !_REJFPANK_CLOADEDOBJ