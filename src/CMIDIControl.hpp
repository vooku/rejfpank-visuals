#ifndef _REJFPANK_CMIDIControl
#define _REJFPANK_CMIDIControl
#include <windows.h>

class CMIDIControl {
public:
	CMIDIControl();
	~CMIDIControl();
	
	bool init(void);

private:
	bool listPorts(void);
	void selectPort(void);
	bool openPort(void);
	void manageMMError(const char * comment);

	unsigned int nPorts;
	unsigned int selectedPort;
	HMIDIIN inHandle;
	MIDIHDR midiHdr;
	char * midiLongBuffer;
	MMRESULT res;
};
extern CMIDIControl cMIDIControl;

#endif // !_REJFPANK_CMIDIControl