#ifndef _REJFPANK_CMIDIControl
#define _REJFPANK_CMIDIControl
#include <windows.h>

void transMidiMsg(unsigned int &midiStatus, unsigned int &midiParam1, unsigned int &midiParam2, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
void CALLBACK midiInCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

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