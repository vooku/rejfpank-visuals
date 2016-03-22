/**
 * @author	Vadim Petrov
 * @date	2016
 */

#ifndef _REJFPANK_CMIDICONTROL
#define _REJFPANK_CMIDICONTROL
#include <windows.h>

void transMidiMsg(unsigned int &midiStatus, unsigned int &midiParam1, unsigned int &midiParam2, const DWORD_PTR dwParam1, const DWORD_PTR dwParam2);
void CALLBACK midiInCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

/**
 * @class CMIDICotrol
 * @brief Class to control the MIDI input.
 *
 * CMIDIControl makes use of the WinMM api and therefore is platform dependent. The class doesn't include the callback for received messages.
 */
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

#endif // !_REJFPANK_CMIDICONTROL