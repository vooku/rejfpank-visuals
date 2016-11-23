/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CSONGSTROBE
#define _REJFPANK_CSONGSTROBE

#include "CSong.hpp"
#include "CBanner.hpp"

class CSongStrobe : public CSong {
public:
	CSongStrobe(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CSongStrobe(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSongSquirrel()
	void shadersInit(void);
	/// Helper funtion for CSongSquirrel()
	void modelsInit(void);

	CBanner ** m_banners;
	unsigned int m_bannersCount;

	int m_toDraw;
	double m_strobeTime;

	TCommonShaderProgram * m_bannerShaderProgram;
};

#endif // !_REJFPANK_CSONGSTROBE