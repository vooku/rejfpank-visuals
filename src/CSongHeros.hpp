/**
* @author	Vadim Petrov
* @date	2016
*/

#ifndef _REJFPANK_CSONGHEROS
#define _REJFPANK_CSONGHEROS

#include "CSong.hpp"
#include "CModel.hpp"
#include "CBanner.hpp"

class CSongHeros : public CSong {
public:
	CSongHeros(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CSongHeros(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSongDefinition()
	void shadersInit(void);
	/// Helper funtion for CSongDefinition()
	void modelsInit(void);

	// own objects
	CModel * m_sphereData;
	int m_spheresN;
	CModel ** m_spheres; // kick1 kick2 snr1 snr2 tom3 tom1 tom2
	glm::vec3 * m_scales;
	CBanner ** m_banners;
	int m_bannersN;

	// ptrs to objects from the controller
	TCommonShaderProgram * m_bannerShaderProgram;

	double * m_triggerTimes;
	double m_reduceTime, m_strobeTime;
	int m_colorShift;
	float m_r;
	float m_camOffset;
};

#endif // !_REJFPANK_CSONGHEROS
