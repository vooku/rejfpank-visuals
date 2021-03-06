/**
 * @author	Vadim Petrov
 * @date		2016
 */

#ifndef _REJFPANK_CSONG
#define _REJFPANK_CSONG

#include "data.hpp"
#include "midicodes.hpp"
#include "TControlState.hpp"
#include "CCamera.hpp"
#include "CSkybox.hpp"

/** Parent class for song structure
 */

class CSong {
public:
	CSong(CCamera * camera, TControlState * state, CSkybox * skybox = NULL);
	virtual ~CSong(void);

	virtual void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) = 0;
	virtual void update(double time) = 0;

	virtual void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) = 0;

protected:
	/// Helper funtion for the constructor
	virtual void shadersInit(void) = 0;
	/// Helper funtion for the constructor
	virtual void modelsInit(void) = 0;
	/// Helper funtion for CSong()
	void multipassInit(void);

	/// map of inner controls
	bool * m_innerMap;

	// own objects
	TCommonShaderProgram * m_shaderPrograms;

	// ptrs to objects from the controller
	CSkybox * m_skybox;
	CCamera * m_camera;
	TControlState * m_state;

	GLuint m_frameBufferObject;
	GLuint m_renderBufferObject;
	GLuint m_renderedTex;
	bool m_multipass;
};

#endif // !_REJFPANK_CSONG
