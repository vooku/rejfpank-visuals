#include "CVeverka.hpp"
#include "midicodes.hpp"
#include "pgr/pgr.hpp"
#include <vector>

using namespace std;

CVeverka::CVeverka(CCamera * camera, TControlState * state, TCommonShaderProgram * bannerShaderProgram)
	: CSong(camera, state),
	m_bannerShaderProgram(bannerShaderProgram) {

	this->shadersInit();
	this->modelsInit();

	for (int i = 0; i < INNER_COUNT; i++) innerMap[i] = false;

	cout << "loaded song: Veverka" << endl;
}

CVeverka::~CVeverka(void) {
	delete[] m_shaderPrograms;

	delete m_squirrel1;
	delete m_squirrel2;
}

void CVeverka::shadersInit(void) {
	vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[1];


	// Init cubePix shaders


}

void CVeverka::modelsInit(void) {
	m_squirrel1 = new CObjectPix(IMG_SQUIRREL5, glm::vec3(0.0f), glm::vec3(1.0f), m_bannerShaderProgram);
	m_squirrel2 = new CObjectPix(IMG_SQUIRREL6, glm::vec3(0.0f), glm::vec3(1.0f), m_bannerShaderProgram);
}

void CVeverka::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (innerMap[INNER_SQUIRREL1]) m_squirrel1->draw(PMatrix, VMatrix);
	if (innerMap[INNER_SQUIRREL2]) m_squirrel2->draw(PMatrix, VMatrix);
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
			innerMap[INNER_SQUIRREL1] = true;
			innerMap[INNER_SQUIRREL2] = false;
			break;
		case MIDI_DRUM_SNARE1:
			innerMap[INNER_SQUIRREL1] = false;
			innerMap[INNER_SQUIRREL2] = true;
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
