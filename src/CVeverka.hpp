/**
* @author	Vadim Petrov
* @date		2016
*/

#ifndef _REJFPANK_CVEVERKA
#define _REJFPANK_CVEVERKA

#include "CSong.hpp"

/** The class name is chosen according to the song it represents, which is in Czech, sorry.
*/

class CVeverka : public CSong {
public:
	CVeverka(CCamera * camera, TControlState * state);
	~CVeverka(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for the constructor
	void shadersInit(void);
	/// Helper funtion for the constructor
	void modelsInit(void);

	//CDrawable * data;
};

#endif // !_REJFPANK_CVEVERKA
