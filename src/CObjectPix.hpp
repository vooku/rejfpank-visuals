/**
 * @author	Vadim Petrov
 * @date	2016
 */

#ifndef _REJFPANK_COBJECTPIX
#define _REJFPANK_COBJECTPIX

#include "CDrawable.hpp"
#include <IL/il.h>
#include <vector>

class CObjectPix : public CDrawable {
public:
	CObjectPix(const char * filename,
			   const glm::vec3 & position,
			   const glm::vec3 & scale,
			   TCommonShaderProgram * shaderProgram);
	//~CObjectPix(void);

	void draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);

protected:
	bool loadImg(const char * filename);
	void sendUniforms(void);

	bool m_enableDraw;

	struct TPixel {
		unsigned char r, g, b, a;
	};

	struct TBlock {
		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 scale;
	};
	std::vector<TBlock> m_blocks;
};

#endif // !_REJFPANK_COBJECTPIX
