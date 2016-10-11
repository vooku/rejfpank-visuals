/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CSONGSQUIRREL
#define _REJFPANK_CSONGSQUIRREL

#include "CSong.hpp"
#include "CObjectPix.hpp"
#include "CBanner.hpp"

class CSongSquirrel : public CSong {
public:
	CSongSquirrel(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CSongSquirrel(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSongSquirrel()
	void shadersInit(void);
	/// Helper funtion for CSongSquirrel()
	void modelsInit(void);
	/// Helper function for midiIn()
	void nextBanner(void);

	unsigned int m_kickCount;
	double m_rideTriggerTime;
	double m_snareTriggerTime;
	glm::vec3 m_axis;

	CObjectPix * m_squirrel1;
	CObjectPix * m_squirrel2;
	CBanner ** m_banners;
	unsigned int m_bannersCount;

	TCommonShaderProgram * m_bannerShaderProgram;
};

#endif // !_REJFPANK_CSONGSQUIRREL