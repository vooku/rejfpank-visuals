/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CSKALA
#define _REJFPANK_CSKALA

#include "CSong.hpp"
#include "CSkybox.hpp"
#include "CBanner.hpp"

/** The class name is chosen according to the song it represents, which is in Czech, sorry.
 */

class CSkala : public CSong {
public:
	CSkala(CCamera * camera, TControlState * state, TCommonShaderProgram * bannerShaderProgram, CSkybox * skybox);
	~CSkala(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for the constructor
	void shadersInit(void);
	/// Helper funtion for the constructor
	void modelsInit(void);

	/// Helper function for midiIn()
	void setCamFlow(const bool flowForward, const bool flowBackward, const bool rollQ, const bool rollE, const bool trigger);
	/// Helper function for update()
	void replaceLoop(const int dir);

	// own objects
	CLoadedObj ** m_legoData;
	CLoadedObj ** m_lego;
	CBanner ** m_banners;

	// ptrs to objects from the controller
	CSkybox * m_skybox;
	TCommonShaderProgram * m_bannerShaderProgram;

	int m_loopCtr;
};

#endif // !_REJFPANK_CSKALA
