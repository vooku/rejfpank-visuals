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

#include <iostream>
#include <vector>
#include <cstdlib>
#include "RtMidi.h"

using namespace std;

void mycallback (double deltatime, std::vector <unsigned char> *message, void */*userData*/) {
	unsigned int nBytes = message->size();
	for (auto it : *message) cout << (int) it << "\t";
	if (nBytes > 0) cout << "\ttimestamp = " << deltatime << endl;
}

// This function should be embedded in a try/catch block in case of
// an exception.  It offers the user a choice of MIDI ports to open.
// It returns false if there are no ports available.
bool chooseMidiPort (RtMidiIn *rtmidi) {
	unsigned int port;
	unsigned int nPorts = rtmidi->getPortCount();
	if (nPorts == 0) {
		cout << "No input ports available!" << endl;
		return false;
	}

	if (nPorts == 1) cout << "\nOpening " << rtmidi->getPortName() << endl;
	else {
		cout << "\tMIDI iput ports\n\tnumber\tname" << endl;
		for (unsigned int i=0; i<nPorts; i++) {
			cout << "\t" << i << "\t" << rtmidi->getPortName (i) << endl;
		}

		do {
			cout << "\nChoose a port number: ";
			cin >> port;
		} while (port >= nPorts);
		string keyHit;
		getline (cin, keyHit);  // used to clear out stdin
	}

	rtmidi->openPort (port);
	return true;
}

int main (void) {
	RtMidiIn *midiin = 0;
  
	try {
		// RtMidiIn constructor
		midiin = new RtMidiIn();

		// Call function to select port.
		if (chooseMidiPort (midiin) == false) {
			delete midiin;
			return 1;
		}

		// Set our callback function.  This should be done immediately after
		// opening the port to avoid having incoming messages written to the
		// queue instead of sent to the callback function.
		midiin->setCallback (&mycallback);

		// Don't ignore sysex, timing, or active sensing messages.
		midiin->ignoreTypes (false, false, false);

		cout << "\nReading MIDI input ... press <enter> to quit.\n";
		char input;
		cin.get (input);

	} catch (RtMidiError &error) {
		error.printMessage ();
	}

  return 0;
}
