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
	CSquirrel(CCamera * camera, TCommonShaderProgram * bannerShaderProgram);
	~CSquirrel(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for the constructor
	void shadersInit(void);
	/// Helper funtion for the constructor
	void modelsInit(void);

	enum {
		SQUIR_SQUIRREL1, SQUIR_SQUIRREL2,
		SQUIR_BANNER0, SQUIR_BANNER1, SQUIR_BANNER2,
		SQUIR_COUNT
	};

	CObjectPix * m_squirrel1;
	CObjectPix * m_squirrel2;
	CBanner ** m_banners;

	TCommonShaderProgram * m_bannerShaderProgram;
};

#endif // !_REJFPANK_CVEVERKA
