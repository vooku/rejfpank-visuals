/**
 * @author	Vadim Petrov
 * @date	2016
 */

#include "CMIDIControl.hpp"
#include "data.hpp"
#include "CController.hpp"

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

CMIDIControl cMIDIControl;

CMIDIControl::CMIDIControl(void) 
	: m_nPorts (midiInGetNumDevs()),
	  m_selectedPort (0) {
	m_midiLongBuffer = new char[MIDI_LONG_BUFFER_SIZE];
}

CMIDIControl::~CMIDIControl() {
	midiInUnprepareHeader(m_inHandle, &m_midiHdr, sizeof(m_midiHdr));
	midiInReset(m_inHandle);
	midiInClose(m_inHandle);
	delete[] m_midiLongBuffer;
}

bool CMIDIControl::init(void) {
	if (!this->listPorts()) return false;
	this->selectPort();
	return this->openPort();
}

bool CMIDIControl::listPorts(void) {
	cout << "\nAvailable MIDI ports:" << endl;
	if (m_nPorts == 0) {
		cerr << "Error: No MIDI ports available!" << endl;
		return false;
	}

	MIDIINCAPS deviceCaps;
	string portName;
	for (unsigned int i = 0; i < m_nPorts; i++) {
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
	if (SELECT_MIDI_PORT_MAN) {
		string data;
		do {
			cout << "\nPlease select a MIDI port:" << endl;
			getline(cin, data);
			if (data.length() > 3) continue;
			m_selectedPort = (unsigned int)atoi(data.c_str());
		} while (m_selectedPort < 1 || m_selectedPort > m_nPorts);
	}
	else m_selectedPort = SELECT_MIDI_PORT_DEFAULT;
	cout << "Selected port " << m_selectedPort << "." << endl;
	m_selectedPort--; // the ports display as starting from 1 instead of 0 
}

void evalMidiMsg(unsigned int &midiStatus, unsigned int &midiParam1, unsigned int &midiParam2,
				 const DWORD_PTR dwParam1, const DWORD_PTR dwParam2) {
	midiStatus = (unsigned int)((dwParam1 & 0x000000ff) >> 0);
	midiParam1 = (unsigned int)((dwParam1 & 0x0000ff00) >> 8);
	midiParam2 = (unsigned int)((dwParam1 & 0x00ff0000) >> 16);

	if (!controller.m_state.ctrlMap[CTRL_INIT]) return; // MIDI received before everything's initialized
	controller.midiIn(midiStatus, midiParam1, midiParam2);
}

void CALLBACK midiInCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	unsigned int midiStatus;
	unsigned int midiParam1;
	unsigned int midiParam2;
		
	switch (wMsg) {
		case MIM_OPEN:
			cout << "MIDI input ready!" << endl;
			break;
		case MIM_ERROR:
			evalMidiMsg(midiStatus, midiParam1, midiParam2, dwParam1, dwParam2);
			cerr << "Error: Invalid MIDI message received! Message: " << midiStatus << " " << midiParam1 << " " << midiParam2 << endl;
			break;
		case MIM_DATA:
			evalMidiMsg(midiStatus, midiParam1, midiParam2, dwParam1, dwParam2);
			break;		
		case MIM_LONGDATA:
			cerr << "Received SysEx message!" << endl;
			break;		
		case MIM_LONGERROR:
			cerr << "Received an invalid SysEx message!" << endl;
			break;
		case MIM_CLOSE:
			cout << "MIDI input finished!" << endl;
			break;
		case MIM_MOREDATA:
			cerr << "Error: Too much MIDI data incoming!" << endl;
			break;
		default:
			cerr << "Received unknown MIDI message!" << endl;
			break;
	}
}

void CMIDIControl::manageMMError(const char * comment) {
	TCHAR errorText[256];
	midiInGetErrorText(m_res, errorText, 255);
	#if defined (UNICODE) || defined (_UNICODE)
		wcerr << "Error: " << comment << " " << errorText << endl;
	#else
		cerr << "Error: " << comment << " " << errorText << endl;
	#endif
}

bool CMIDIControl::openPort(void) {
	m_res = midiInOpen(&m_inHandle, m_selectedPort, (DWORD_PTR) &midiInCallback, NULL, CALLBACK_FUNCTION);
	if (m_res != MMSYSERR_NOERROR) {
		manageMMError("Cannot open port!");
		return false;
	}
	
	m_midiHdr.lpData = m_midiLongBuffer;
	m_midiHdr.dwBufferLength = MIDI_LONG_BUFFER_SIZE;
	m_midiHdr.dwFlags = 0;

	m_res = midiInPrepareHeader(m_inHandle, &m_midiHdr, sizeof(m_midiHdr));
	if (MMSYSERR_NOERROR != m_res) {
		midiInReset(m_inHandle);
		midiInClose(m_inHandle);
		manageMMError("Failed to prepare MIDI header!");
		return false;
	}
	
	m_res = midiInAddBuffer(m_inHandle, &m_midiHdr, sizeof(m_midiHdr));
	if (MMSYSERR_NOERROR != m_res) {
		midiInUnprepareHeader(m_inHandle, &m_midiHdr, sizeof(m_midiHdr));
		midiInReset(m_inHandle);
		midiInClose(m_inHandle);
		manageMMError ("Failed to add MIDI buffer!");
		return false;
	}
	
	m_res = midiInStart(m_inHandle);
	if (MMSYSERR_NOERROR != m_res) {
		midiInUnprepareHeader(m_inHandle, &m_midiHdr, sizeof(m_midiHdr));
		midiInReset(m_inHandle);
		midiInClose(m_inHandle);
		manageMMError ("Failed to start MIDI input device!");
		return false;
	}

	return true;
}

