#include "TMIDIControl.h"
//#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

TMIDIControl tMIDIControl;

TMIDIControl::TMIDIControl(void) 
	: nPorts (midiInGetNumDevs()),
	  selectedPort (0) { }

bool TMIDIControl::listPorts(void) {
	cout << "\nAvailable MIDI ports:\n" << endl;
	if (nPorts == 0) {
		cerr << "Error: No MIDI ports available!" << endl;
		return false;
	}

	MIDIINCAPS deviceCaps;
	string portName;
	for (unsigned int i = 0; i < nPorts; i++) {
		midiInGetDevCaps(i, &deviceCaps, sizeof(MIDIINCAPS));

		#if defined (UNICODE) || defined (_UNICODE)
			int length = WideCharToMultiByte(CP_UTF8, 0, deviceCaps.szPname, -1, NULL, 0, NULL, NULL) - 1;
			portName.assign(length, 0);
			length = WideCharToMultiByte(CP_UTF8, 0, deviceCaps.szPname, static_cast<int>(wcslen(deviceCaps.szPname)), &portName[0], length, NULL, NULL);
		#else
			portName = string(deviceCaps.szPname);
		#endif
		cout << "\t" << i + 1 << ":\t" << portName << endl;
	}
	return true;
}

void TMIDIControl::selectPort(void) {
	string data;
	do {
		cout << "\nPlease select a MIDI port:" << endl;
		getline(cin, data);
		if (data.length() > 3) continue;
		selectedPort = (unsigned int) atoi(data.c_str());
	} while (selectedPort < 1 || selectedPort > nPorts);
	
	cout << "Selected port " << tMIDIControl.selectedPort << "." << endl;
	selectedPort--; // the ports display as starting from 1 instead of 0 
}

void CALLBACK midiInCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	cout << "ahoj!" << endl;
}

bool TMIDIControl::openPort(void) {
	MMRESULT res = midiInOpen(&inHandle, selectedPort, (DWORD_PTR) &midiInCallback, NULL, CALLBACK_FUNCTION);
	if (res == MMSYSERR_NOERROR) cout << "hura" << endl;
		
	return false;
}

