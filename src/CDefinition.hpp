/**
 * @author	Vadim Petrov
 * @date		2016
 */

#ifndef _REJFPANK_CDEFINITION
#define _REJFPANK_CDEFINITION

#include "CSong.hpp"
#include "CLoadedObj.hpp"

class CDefinition : public CSong {
public:
	CDefinition(CCamera * camera);
	~CDefinition(void);

	void redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix);
	void update(double time);

	void midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity);

protected:
	/// Helper funtion for CSquirrel()
	void shadersInit(void);
	/// Helper funtion for CSquirrel()
	void modelsInit(void);

	enum {
		DEF_COUNT
	};

	CLoadedObj ** m_honeycombs;

	unsigned int m_kickCount;
};

#endif // !_REJFPANK_CDEFINITION
