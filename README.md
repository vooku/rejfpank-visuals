# REJFPANK VISUALS

## Introduction
This is my undergraduate thesis at [FIT CTU](https://www.fit.cvut.cz/en). It visualises MIDI data from incoming stream sourced from samplers, drum machines, gameboys etc.

I am a big fan of what I call _glitch aesthetics_, commonly seen in the context of the chiptune or algorave scenes. It consists of bright colors, glitches and general randomness. Expect this kind of visual turn-on.

The rest of the readme is mostly for myself. 

## Don't forget
- unify sr16 and mpx16 midi notes with the standard
- GLFW 3.2 should be released soon and should support toggling fullscreen
- create a GLFW_ICON when illustrations are ready
- don't load models for each instance

## Ideas
- create an algorithm to tessellate a rectangle
	- parameter (starting from 1) is the 'tessellation lvl' -> cell count equals 1 / (tessellation lvl + 1)^2
- Voronoi diagram with taxicab geometry
- Fog color depending on the y coordinate
- Switch dynamically between ortographic and perspective projection
- Toy with glm::frustum
- Create light emiting objects via fog
- Generate random points and draw tham as triangle strip 
- change directional light on beat
- King Midas: White (yellow) over everything, aplha slowly fading
- trailing lego bricks in fifo

## Known Issues
- When connecting to AKAI MPX16, WinMM throws the "not enough memory" error. The MPX16 MIDI implementation sucks pretty hard though and in the end I won't be using it anyway.

## Useful links
- [Procedural textures inspiration](http://www.upvector.com/?section=Tutorials&subsection=Intro%20to%20Procedural%20Textures)
- [Windows MIDI reference](https://msdn.microsoft.com/en-us/library/windows/desktop/dd798495(v=vs.85).aspx)
- [MIDI notes reference](http://www.midimountain.com/midi/midi_status.htm)
- [openFrameworks](http://openframeworks.cc/)

## Notes
- Midi yoke proved to be obsolete — incompatible with 64bit SW such as Ableton Live 9. For testing purposes I am using [loopMIDI](www.tobias-erichsen.de/software/loopmidi.html) instead.
- OSC (Open Sound Control) is (might be) the new MIDI

## Similar stuff
- [OpenTZT](http://opentzt.sourceforge.net/) — VJ tool based on DirectX, also receives MIDI
- [midi2opengl](http://chancesend.com/2004/12/midi-2-opengl-visualization/) — Similar concept developed for the Oxygen8 keyboard controller. The site is cluttered with dead links and the project seems to be dead since it's publication in 2004.
- [MIDI.live](http://www.zaposerver.net/projects/midilive/files/midilive.pdf) — abandoned in 2009 pre-beta

## Thanks
- Thanks to [Floris 'floriuszzz' Smit](http://tf3dm.com/user/floriuszzz) whose Lego Brick 3D model I'm using