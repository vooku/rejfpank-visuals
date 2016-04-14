#include "CVeverka.hpp"
#include "midicodes.hpp"
#include "pgr/pgr.hpp"
#include <vector>

using namespace std;

CVeverka::CVeverka(CCamera * camera, TControlState * state)
	: CSong(camera, state) {

	this->shadersInit();
	this->modelsInit();
	cout << "loaded song Veverka!" << endl;
}

void CVeverka::shadersInit(void) {
	vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[3];


	// Init cubePix shaders


}

void CVeverka::modelsInit(void) {
	// TODO cubePix
}

CVeverka::~CVeverka(void) {
	delete m_shaderPrograms;

	// TODO cubePix
}

void CVeverka::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {

	// TODO cubePix
}


void CVeverka::update(double time) {
	
}

void CVeverka::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	//-------------------------------------------------------------------> AKAI MPX16
	if (status == MIDI_NOTE_ON_CH10) {
		switch (note) {
		case MPX16_PAD01:
			
			break;
		case MPX16_PAD02:
			
			break;
		case MPX16_PAD03:
			
			break;
		case MPX16_PAD04:
			
			break;
		case MPX16_PAD05:
			
			break;
		case MPX16_PAD06:
			
			break;
		case MPX16_PAD07:
			
			break;
		case MPX16_PAD08:
			
			break;
		case MPX16_PAD09:
			
			break;
		case MPX16_PAD10:
			
			break;
		case MPX16_PAD11:
			
			break;
		case MPX16_PAD12:
			
			break;
		case MPX16_PAD13:

			break;
		case MPX16_PAD14: // block bass

			break;
		case MPX16_PAD15:  // block midas

			break;
		case MPX16_PAD16:

			break;
		default:
			cout << "Unresolved midi note from MPX16:" << status << " " << note << " " << velocity << endl;
			break;
		}
	}
	//-------------------------------------------------------------------> ALESIS SR16
	else if (status == MIDI_NOTE_ON_CH02) {
		switch (note) {
		case MIDI_DRUM_KICK1:

			break;
		case MIDI_DRUM_HIHAT_CLOSED:

			break;
		case MIDI_DRUM_SNARE2:

			break;
		default:
			cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << endl;
			break;
		}
	}
}
