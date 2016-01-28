# REJFPANK VISUALS

## Introduction
This is my undergraduate thesis at [FIT CTU](https://www.fit.cvut.cz/en). It visualises MIDI data from incoming stream sourced from samplers, drum machines, gameboys etc.

I am a big fan of what I call 'glitch aesthetics,' commonly seen in the context of the chiptune or algorave scenes. It consists of bright colors, glitches and general randomness. Expect this kind of visual turn-on.

The rest of the readme is mostly for myself. 

## TODO
- knihovny:
	- glut vs
		- GLFW ()
		- SFML (Simple and Fast Multimedia Library)
		- Allegro
	- midi 
		- [RtMidi](https://github.com/thestk/rtmidi)
		- Juce
		- SDL

## Ideas
- use cmake to create makefile
- use a function to create VBOs
- create an algorithm to tessellate a rectangle
	- parameter (starting from 1) is the 'tessellation lvl' -> cell count equals 1 / (tessellation lvl + 1)^2
- Voronoi diagram with taxicab geometry
- Ask the good fellows over at Chiptunes = Win for some pixel artist -> Pokémon-Crystal-like sprite of a squirrel and a mutant squirrel
- Transform pixels of BMP to vertex data and display them as points (toy with distance and point size, animate point size)
- Fog color depending on the y coordinate
- Switch dynamically between ortographic and perspective projection
- Toy with glm::frustum
- Create light emiting objects via fog

## Useful links
- [Procedural textures inspiration](http://www.upvector.com/?section=Tutorials&subsection=Intro%20to%20Procedural%20Textures)
- [Windows MIDI reference](https://msdn.microsoft.com/en-us/library/windows/desktop/dd798495(v=vs.85).aspx)
- [openFrameworks](http://openframeworks.cc/)
- [mTroll](http://sourceforge.net/p/mtroll/mTroll/ci/default/tree/midi/) -- windows midi example

## Notes
- Midi yoke proved to be obsolete -- incompatible with 64bit SW such as Ableton Live 9. For testing purposes I am using [loopMIDI](www.tobias-erichsen.de/software/loopmidi.html) instead.
- OSC (Open Sound Control) is (might be) the new MIDI