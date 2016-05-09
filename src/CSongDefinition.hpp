/**
 * @author	Vadim Petrov
 * @date	2016
 */

#ifndef _REJFPANK_CSONGDEFINITION
#define _REJFPANK_CSONGDEFINITION

#include "CSong.hpp"
#include "CModel.hpp"
#include "CBanner.hpp"

class CSongDefinition : public CSong {
public:
	CSongDefinition(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CSongDefinition(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSongDefinition()
	void shadersInit(void);
	/// Helper funtion for CSongDefinition()
	void modelsInit(void);

	void sweepBanners(const double time);

	enum {
		DEF_BANNER0, DEF_BANNER1, DEF_BANNER2,
		DEF_BLUE, DEF_RED, DEF_POINT,
		DEF_INVERSE, DEF_REDUCE, DEF_SWEEP,
		DEF_BACKGROUND,
		DEF_COUNT
	};

	// own objects
	CModel ** m_honeyData;
	int m_honeyDataN;
	CModel ** m_honeycombs;
	CBanner ** m_banners;
	int m_bannersN;

	// ptrs to objects from the controller
	TCommonShaderProgram * m_bannerShaderProgram;

	double m_triggerTimes[4]; // 0 sweep, 1 point light, 2 red light, 3 blue light
	int m_colorShift;
	float m_r;
	float m_camOffset;
};

#endif // !_REJFPANK_CSONGDEFINITION
