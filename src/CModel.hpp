/**
 * @author	Vadim Petrov
 * @date	2016
 */

#ifndef _REJFPANK_CMODEL
#define _REJFPANK_CMODEL

#include "CDrawable.hpp"
#include "TCommonShaderProgram.hpp"

class CModel : public CDrawable {
public:
	CModel(const char * filename,
		   const glm::vec3 & position,
		   const glm::vec3 & scale,
		   TCommonShaderProgram * shaderProgram,
		   const char * texname = NULL,
		   const bool randomizeUV = false,
		   const CModel * dataObj = NULL,
		   const unsigned int materialIdx = 0,
		   const float & alpha = 1.0f);

	void setMaterial(const glm::vec3 & ambient,
					 const glm::vec3 & diffuse,
					 const glm::vec3 & specular,
					 const float & shininess,
					 const float & alpha = 1.0f);

	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);

protected:
	bool loadObj(const char * filename, const bool randomizeUV); ///< Helper method for the constructor
	void setMaterials(const char * filename); ///< Helper method for the constructor

	void sendUniforms(void); ///< Helper method for draw()

	bool m_enableDraw; ///< whether the object was properly initialized (loaded)
	bool m_containsData;
	bool m_useTex;
	const CModel * m_dataObj;

	struct TMaterial {
		unsigned int index; ///< Material hinted from outside

		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;
	} m_material;
};

#endif // !_REJFPANK_CMODEL