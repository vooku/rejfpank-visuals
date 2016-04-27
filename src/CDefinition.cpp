/**
 * @author	Vadim Petrov
 * @date		2016
 */

#include "CDefinition.hpp"
#include "pgr/pgr.hpp"

CDefinition::CDefinition(CCamera * camera)
	: CSong(camera),
	  m_kickCount(0) {
	glClearColor(0.3f, 0.0f, 0.0f, 1.0f);

	m_innerMap = new bool[DEF_COUNT];
	for (int i = 0; i < DEF_COUNT; i++) m_innerMap[i] = false;

	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Definice" << std::endl;
}

CDefinition::~CDefinition(void) {
	delete m_honeyData;
	for (int i = 0; i < 8; i++) delete m_honeycombs[i];
	delete[] m_honeycombs;

	delete m_eye;

	delete[] m_shaderPrograms;

	delete[] m_innerMap;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	std::cout << "destroyed song: Definice" << std::endl;
}

void CDefinition::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	for (int i = 0; i < 8; i++) m_honeycombs[i]->draw(PMatrix, VMatrix);
	m_eye->draw(PMatrix, VMatrix);
}

void CDefinition::update(double time) {

}


void CDefinition::shadersInit(void) {
	std::vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[2];

	// Init lego shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/commonShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/commonShader.frag"));
	m_shaderPrograms[0].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[0].PVMMatrixLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "PVMMatrix");
		m_shaderPrograms[0].VMatrixLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "VMatrix");
		m_shaderPrograms[0].MMatrixLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "MMatrix");
		m_shaderPrograms[0].normalMatrixLocation =	glGetUniformLocation(m_shaderPrograms[0].program, "normalMatrix");
		m_shaderPrograms[0].diffuseLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "material.diffuse");
		m_shaderPrograms[0].ambientLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "material.ambient");
		m_shaderPrograms[0].specularLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "material.specular");
		m_shaderPrograms[0].shininessLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "material.shininess");
		m_shaderPrograms[0].fadeToBlackLocation =	glGetUniformLocation(m_shaderPrograms[0].program, "fadeToBlack");
		// Get input locations
		m_shaderPrograms[0].posLocation =		glGetAttribLocation(m_shaderPrograms[0].program, "position");
		m_shaderPrograms[0].normalLocation =	glGetAttribLocation(m_shaderPrograms[0].program, "normal");

	shaders.clear();

	// Init pix shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/pixShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/pixShader.frag"));
	m_shaderPrograms[1].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[1].PVMMatrixLocation = glGetUniformLocation(m_shaderPrograms[1].program, "PVMMatrix");
		// Get input locations
		m_shaderPrograms[1].posLocation =		glGetAttribLocation(m_shaderPrograms[1].program, "position");
		m_shaderPrograms[1].ambientLocation =	glGetAttribLocation(m_shaderPrograms[1].program, "color");
		m_shaderPrograms[1].offsetLocation =	glGetAttribLocation(m_shaderPrograms[1].program, "offset");

	shaders.clear();
}

void CDefinition::modelsInit(void) {
	m_honeyData = new CLoadedObj(MODEL_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0]);

	m_honeycombs = new CLoadedObj * [8];
	float phi = 0.0f;
	float r = 2.5f;
	for (int i = 0; i < 8; i++) {
		m_honeycombs[i] = new CLoadedObj(MODEL_HONEY, glm::vec3(0.0f, r * glm::sin(phi), r * glm::cos(phi)), glm::vec3(1.0f), &m_shaderPrograms[0], m_honeyData);
		m_honeycombs[i]->rotate(M_PI / 2.0f - phi, glm::vec3(1.0f, 0.0f, 0.0f));

		phi += M_PI / 4.0f;
	}

	m_eye = new CObjectPix(IMG_EYE_BLACK, m_camera->m_position + glm::normalize(m_camera->m_direction), glm::vec3(2.5f), &m_shaderPrograms[1], 12.0f);
}


void CDefinition::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
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
		case MPX16_PAD14:
			break;
		case MPX16_PAD15:
			break;
		case MPX16_PAD16:
			break;
		default:
			std::cout << "Unresolved midi note from MPX16:" << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
	//-------------------------------------------------------------------> ALESIS SR16
	// sends velocity 0 instead of note off
	else if (status == MIDI_NOTE_ON_CH02) {
		if (velocity != 0) switch (note) {
		case MIDI_DRUM_KICK1:
			break;
		case MIDI_DRUM_KICK2:
			break;
		case MIDI_DRUM_SNARE1:
			break;
		case MIDI_DRUM_TOM_LOW1:
			break;
		case MIDI_DRUM_TOM_MID1:
			break;
		case MIDI_DRUM_TOM_HIGH1: // laser tom
			break;
		case MIDI_DRUM_CYMBAL_CRASH1:
			break;
		case MIDI_DRUM_CYMBAL_SPLASH:
			break;
		case MIDI_DRUM_CYMBAL_RIDE1:
			break;
		default:
			std::cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
}