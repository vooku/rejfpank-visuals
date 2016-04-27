/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CROCK
#define _REJFPANK_CROCK

#include "CSong.hpp"
#include "CBanner.hpp"
#include "CLoadedObj.hpp"

class CRock : public CSong {
public:
	CRock(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CRock(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CRock()
	void shadersInit(void);
	/// Helper funtion for CRock()
	void modelsInit(void);

	/// Helper function for midiIn()
	void setCamFlow(const bool flowForward, const bool flowBackward, const bool rollQ, const bool rollE, const bool trigger);
	/// Helper function for update()
	void replaceLoop(const int dir);

	enum {
		ROCK_CAM_FORWARD, ROCK_CAM_BACKWARD, ROCK_CAM_Q, ROCK_CAM_E,
		ROCK_BANNER0, ROCK_BANNER1, ROCK_BANNER2,
		ROCK_BLACK,
		ROCK_KICK1, ROCK_HIHAT_CLOSED, ROCK_PLUCK,
		ROCK_COUNT
	};

	// own objects
	CLoadedObj ** m_legoData;
	CLoadedObj ** m_lego;
	CBanner ** m_banners;

	// ptrs to objects from the controller
	TCommonShaderProgram * m_bannerShaderProgram;

	int m_loopCtr;
};

#endif // !_REJFPANK_CROCK
