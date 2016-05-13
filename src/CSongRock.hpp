/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CSONGROCK
#define _REJFPANK_CSONGROCK

#include "CSong.hpp"
#include "CBanner.hpp"
#include "CModel.hpp"

class CSongRock : public CSong {
public:
	CSongRock(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CSongRock(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSongRock()
	void shadersInit(void);
	/// Helper funtion for CSongRock()
	void modelsInit(void);

	/// Helper function for midiIn()
	void setCamFlow(const bool flowForward, const bool flowBackward, const bool rollQ, const bool rollE, const bool trigger);
	/// Helper function for update()
	void replaceLoop(const int dir);

	// own objects
	CModel ** m_legoData;
	CModel ** m_lego;
	CBanner ** m_banners;

	// ptrs to objects from the controller
	TCommonShaderProgram * m_bannerShaderProgram;

	int m_loopCtr;
};

#endif // !_REJFPANK_CSONGROCK
