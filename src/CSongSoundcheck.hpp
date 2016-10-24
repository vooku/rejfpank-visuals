/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CSONGSOUNDCHECK
#define _REJFPANK_CSONGSOUNDCHECK

#include "CSong.hpp"
#include "CBanner.hpp"

class CSongSoundcheck : public CSong {
public:
	CSongSoundcheck(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CSongSoundcheck(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSongSquirrel()
	void shadersInit(void);
	/// Helper funtion for CSongSquirrel()
	void modelsInit(void);
	
	int m_colorShift;
	CBanner ** m_banners;
	unsigned int m_bannersCount;

	TCommonShaderProgram * m_bannerShaderProgram;
};

#endif // !_REJFPANK_CSONGSOUNDCHECK