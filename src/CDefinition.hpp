/**
 * @author	Vadim Petrov
 * @date	2016
 */

#ifndef _REJFPANK_CDEFINITION
#define _REJFPANK_CDEFINITION

#include "CSong.hpp"
#include "CLoadedObj.hpp"
#include "CBanner.hpp"

class CDefinition : public CSong {
public:
	CDefinition(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram);
	~CDefinition(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CDefinition()
	void shadersInit(void);
	/// Helper funtion for CDefinition()
	void modelsInit(void);

	void sweepBanners(const double time);

	enum {
		DEF_BANNER0, DEF_BANNER1, DEF_BANNER2, DEF_BANNER4, // banner 3 is for multipass
		DEF_INVERSE, DEF_REDUCE, DEF_SWEEP,
		DEF_BACKGROUND,
		DEF_COUNT
	};

	// own objects
	CLoadedObj ** m_honeyData;
	int m_honeyDataN;
	CLoadedObj ** m_honeycombs;
	CBanner ** m_banners;
	int m_bannersN;

	// ptrs to objects from the controller
	TCommonShaderProgram * m_bannerShaderProgram;

	unsigned int m_kickCount;
	double m_sweepTriggerTime;
	float m_r;
	float m_camOffset;
};

#endif // !_REJFPANK_CDEFINITION
