#ifndef _REJFPANK_TMIDICONTROL
#define _REJFPANK_TMIDICONTROL
#include <windows.h>

struct TMIDIControl {
	TMIDIControl();

	bool listPorts(void);
	void selectPort(void);
	bool openPort(void);

	unsigned int nPorts;
	unsigned int selectedPort;
	HMIDIIN inHandle;
};
extern TMIDIControl tMIDIControl;

#endif // !_REJFPANK_TMIDICONTROL