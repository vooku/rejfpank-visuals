/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CVEVERKA
#define _REJFPANK_CVEVERKA

#include "CSong.hpp"
#include "CObjectPix.hpp"
#include "CBanner.hpp"

class CSquirrel : public CSong {
public:
	CSquirrel(CCamera * camera, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram, TControlState * state);
	~CSquirrel(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSquirrel()
	void shadersInit(void);
	/// Helper funtion for CSquirrel()
	void modelsInit(void);
	/// Helper funtion for CSquirrel()
	void multipassInit(void);
	/// Helper function for midiIn()
	void nextBanner(void);

	enum {
		SQUIR_FBUFF,
		SQUIR_SQUIRREL1, SQUIR_SQUIRREL2,
		SQUIR_BANNER0, SQUIR_BANNER1, SQUIR_BANNER2,
		SQUIR_COUNT
	};

	unsigned int m_kickCount;
	glm::vec2 m_camOffset;

	CObjectPix * m_squirrel1;
	CObjectPix * m_squirrel2;
	CBanner ** m_banners;
	unsigned int m_bannersCount;

	TCommonShaderProgram * m_bannerShaderProgram;
	TControlState * m_state;

	GLuint m_frameBufferObject;
	GLuint m_renderBufferObject;
	GLuint m_renderedTex;
};

#endif // !_REJFPANK_CVEVERKA
