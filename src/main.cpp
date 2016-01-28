/*
 * Copyright (C) 2016 Vadim Petrov
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */
/**
 * @author Vadim Petrov
 * @date 2016
 */
 
#include <conio.h>

#include <iostream>
#include <stdio.h>

#include <windows.h>   /* required before including mmsystem.h */
#include <mmsystem.h>

using namespace std;

bool PrintMidiDevices() {
	unsigned int nMidiDeviceNum;
	MIDIINCAPS caps;

	nMidiDeviceNum = midiInGetNumDevs();
	if (nMidiDeviceNum == 0) {
		cerr << "midiInGetNumDevs() returns 0..." << endl;
		return false;
	}

	cout << "== PrintMidiDevices() ==" << endl;
	for (unsigned int i = 0; i < nMidiDeviceNum; ++i) {
		midiInGetDevCaps(i, &caps, sizeof(MIDIINCAPS));
		//cout << "\t" << i << ": name = " << caps.szPname << endl;
		cout << "\t" << i << ":\t" << caps.wMid << ", " << caps.wPid  << ", " << caps.vDriverVersion << ", " << caps.szPname << ", " << caps.dwSupport << endl;
	}
	cout << endl;
	return true;
}

void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
	switch(wMsg) {
	case MIM_OPEN:
		printf("wMsg=MIM_OPEN\n");
		break;
	case MIM_CLOSE:
		printf("wMsg=MIM_CLOSE\n");
		break;
	case MIM_DATA:
		printf("wMsg=MIM_DATA, dwInstance=%08x, dwParam1=%08x, dwParam2=%08x\n", (unsigned int) dwInstance, (unsigned int) dwParam1, (unsigned int) dwParam2);
		break;
	case MIM_LONGDATA:
		printf("wMsg=MIM_LONGDATA\n"); 
		break;
	case MIM_ERROR:
		printf("wMsg=MIM_ERROR\n");
		break;
	case MIM_LONGERROR:
		printf("wMsg=MIM_LONGERROR\n");
		break;
	case MIM_MOREDATA:
		printf("wMsg=MIM_MOREDATA\n");
		break;
	default:
		printf("wMsg = unknown\n");
		break;
	}
	return;
}

int main(int argc, char** argv) {
	HMIDIIN hMidiDevice = NULL;;
	DWORD nMidiPort = -1;
	
	if (midiInGetNumDevs() == 0) {
		cerr << "midiInGetNumDevs() returns 0, quitting." << endl;
		return -1;
	}

	if (!PrintMidiDevices()) {
		cerr << "Cannot acces MIDI devices, quitting." << endl;
		return -1;
	}
	while (nMidiPort < 0 || nMidiPort >= midiInGetNumDevs()) {
		cout << "Enter MIDI Input device number:" << endl;
		scanf ("%lu", &nMidiPort);
	}

	if (midiInOpen(&hMidiDevice, nMidiPort, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
		cout << "midiInOpen() failed, quitting." << endl;
		return -1;
	}

	midiInStart(hMidiDevice);



	midiInStop(hMidiDevice);
	midiInClose(hMidiDevice);
	hMidiDevice = NULL;

	return 0;
}

