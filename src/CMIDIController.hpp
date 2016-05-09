/**
 * @author	Vadim Petrov
 * @date	2016
 */

#ifndef _REJFPANK_CMIDICONTROLLER
#define _REJFPANK_CMIDICONTROLLER
#include <windows.h>

void CALLBACK midiInCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

/**
 * @class CMIDICotrol
 * @brief Class to control the MIDI input.
 *
 * CMIDIController makes use of the WinMM api and therefore is platform dependent. The class doesn't include the callback for received messages.
 */
class CMIDIController {
public:
	CMIDIController(void);
	~CMIDIController(void);
	
	bool init(void);

private:
	bool listPorts(void);
	void selectPort(void);
	bool openPort(void);
	void manageMMError(const char * comment);

	unsigned int m_nPorts;
	unsigned int m_selectedPort;
	HMIDIIN m_inHandle;
	MIDIHDR m_midiHdr;
	char * m_midiLongBuffer;
	MMRESULT m_res;
};
extern CMIDIController MIDIController;

#endif // !_REJFPANK_CMIDICONTROLLER