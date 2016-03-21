#include "CMIDIControl.hpp"
#include "data.hpp"

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

CMIDIControl cMIDIControl;

CMIDIControl::CMIDIControl(void) 
	: nPorts (midiInGetNumDevs()),
	  selectedPort (0) {
	midiLongBuffer = new char[MIDI_LONG_BUFFER_SIZE];
}

CMIDIControl::~CMIDIControl() {
	midiInUnprepareHeader(inHandle, &midiHdr, sizeof(midiHdr));
	midiInReset(inHandle);
	midiInClose(inHandle);
	delete[] midiLongBuffer;
}

bool CMIDIControl::init(void) {
	if (!this->listPorts()) return false;
	this->selectPort();
	return this->openPort();
}

bool CMIDIControl::listPorts(void) {
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

void CMIDIControl::selectPort(void) {
	string data;
	do {
		cout << "\nPlease select a MIDI port:" << endl;
		getline(cin, data);
		if (data.length() > 3) continue;
		selectedPort = (unsigned int) atoi(data.c_str());
	} while (selectedPort < 1 || selectedPort > nPorts);
	
	cout << "Selected port " << selectedPort << "." << endl;
	selectedPort--; // the ports display as starting from 1 instead of 0 
}

void CALLBACK midiInCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	cout << "ahoj!" << endl;
}

void CMIDIControl::manageMMError(const char * comment) {
	TCHAR errorText[256];
	midiInGetErrorText(res, errorText, 255);
	#if defined (UNICODE) || defined (_UNICODE)
		wcerr << "Error: " << comment << " " << errorText << endl;
	#else
		cerr << "Error: " << comment << " " << errorText << endl;
	#endif
}

bool CMIDIControl::openPort(void) {
	res = midiInOpen(&inHandle, selectedPort, (DWORD_PTR) &midiInCallback, NULL, CALLBACK_FUNCTION);
	if (res != MMSYSERR_NOERROR) {
		manageMMError("Cannot open port!");
		return false;
	}
	
	midiHdr.lpData = midiLongBuffer;
	midiHdr.dwBufferLength = MIDI_LONG_BUFFER_SIZE;
	midiHdr.dwFlags = 0;

	res = midiInPrepareHeader(inHandle, &midiHdr, sizeof(midiHdr));
	if (MMSYSERR_NOERROR != res) {
		midiInReset(inHandle);
		midiInClose(inHandle);
		manageMMError("Failed to prepare MIDI header!");
		return false;
	}
	
	res = midiInAddBuffer(inHandle, &midiHdr, sizeof(midiHdr));
	if (MMSYSERR_NOERROR != res) {
		midiInUnprepareHeader(inHandle, &midiHdr, sizeof(midiHdr));
		midiInReset(inHandle);
		midiInClose(inHandle);
		manageMMError ("Failed to add MIDI buffer!");
		return false;
	}
	
	res = midiInStart(inHandle);
	if (MMSYSERR_NOERROR != res) {
		midiInUnprepareHeader(inHandle, &midiHdr, sizeof(midiHdr));
		midiInReset(inHandle);
		midiInClose(inHandle);
		manageMMError ("Failed to start MIDI input device!");
		return false;
	}

	return true;
}

