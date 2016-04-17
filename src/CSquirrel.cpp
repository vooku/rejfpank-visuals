#include "CSquirrel.hpp"
#include "midicodes.hpp"
#include "pgr/pgr.hpp"
#include <vector>

CSquirrel::CSquirrel(CCamera * camera, TCommonShaderProgram * bannerShaderProgram)
	: CSong(camera),
	m_bannerShaderProgram(bannerShaderProgram) {

	m_innerMap = new bool[SQUIR_COUNT];
	for (int i = 0; i < SQUIR_COUNT; i++) m_innerMap[i] = false;

	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Veverka" << std::endl;
}

CSquirrel::~CSquirrel(void) {
	delete m_squirrel1;
	delete m_squirrel2;

	for (int i = 0; i < 3; i++) delete m_banners[i];
	delete[] m_banners;

	delete[] m_shaderPrograms;

	delete[] m_innerMap;
}

void CSquirrel::shadersInit(void) {
	std::vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[1];


	// Init cubePix shaders


}

void CSquirrel::modelsInit(void) {
	m_squirrel1 = new CObjectPix(IMG_SQUIRREL5, glm::vec3(0.0f), glm::vec3(1.0f), m_bannerShaderProgram);
	m_squirrel2 = new CObjectPix(IMG_SQUIRREL6, glm::vec3(0.0f), glm::vec3(1.0f), m_bannerShaderProgram);

	m_banners = new CBanner * [3];
	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram, false);
	m_banners[1] = new CBanner(m_camera, m_bannerShaderProgram, false);
	m_banners[2] = new CBanner(m_camera, m_bannerShaderProgram, false);
	m_banners[0]->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	m_banners[1]->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	m_banners[2]->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
}

void CSquirrel::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (m_innerMap[SQUIR_SQUIRREL1]) m_squirrel1->draw(PMatrix, VMatrix);
	if (m_innerMap[SQUIR_SQUIRREL2]) m_squirrel2->draw(PMatrix, VMatrix);

	if (m_innerMap[SQUIR_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_innerMap[SQUIR_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);
	if (m_innerMap[SQUIR_BANNER2]) m_banners[2]->draw(PMatrix, VMatrix);
}


void CSquirrel::update(double time) {
	
}

void CSquirrel::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
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
			std::cout << "Unresolved midi note from MPX16:" << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
	//-------------------------------------------------------------------> ALESIS SR16
	else if (status == MIDI_NOTE_ON_CH02) {
		switch (note) {
		case MIDI_DRUM_KICK1:
			m_innerMap[SQUIR_SQUIRREL1] = true;
			m_innerMap[SQUIR_SQUIRREL2] = false;
			break;
		case MIDI_DRUM_SNARE1:
			m_innerMap[SQUIR_SQUIRREL1] = false;
			m_innerMap[SQUIR_SQUIRREL2] = true;
			break;
		case MIDI_DRUM_TOM_LOW1:
			m_innerMap[SQUIR_BANNER0] = true;
			m_innerMap[SQUIR_BANNER1] = false;
			m_innerMap[SQUIR_BANNER2] = false;
			break;
		case MIDI_DRUM_TOM_MID1:
			m_innerMap[SQUIR_BANNER0] = false;
			m_innerMap[SQUIR_BANNER1] = true;
			m_innerMap[SQUIR_BANNER2] = false;
			break;
		case MIDI_DRUM_TOM_HIGH1:
			m_innerMap[SQUIR_BANNER0] = false;
			m_innerMap[SQUIR_BANNER1] = false;
			m_innerMap[SQUIR_BANNER2] = true;
			break;
		default:
			std::cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
}