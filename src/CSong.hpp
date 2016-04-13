/**
 * @author	Vadim Petrov
 * @date		2016
 */

#ifndef _REJFPANK_CSONG
#define _REJFPANK_CSONG

#include "data.hpp"
#include "TControlState.hpp"
#include "CCamera.hpp"
#include "CSkybox.hpp"
#include "CLoadedObj.hpp"
#include "CBanner.hpp"

/** Parent class for song structure
 */

struct CSong {
public:
	CSong(CCamera * camera, TControlState * state);
	~CSong(void);

	virtual void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) = 0;
	virtual void update(double time) = 0;

	virtual void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) = 0;

protected:
	/// Helper funtion for the constructor
	virtual void shadersInit(void) = 0;
	/// Helper funtion for the constructor
	virtual void modelsInit(void) = 0;

	CCamera * m_camera;
	TControlState * m_state;
	TCommonShaderProgram * m_shaderPrograms;
};

#endif // !_REJFPANK_CSONG
