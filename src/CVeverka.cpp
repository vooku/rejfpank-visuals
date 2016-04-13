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

	// Init skybox shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/skyboxShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skyboxShader.frag"));
	m_shaderPrograms[0].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[0].PVMMatrixLocation = glGetUniformLocation(m_shaderPrograms[0].program, "PVMMatrix");
		m_shaderPrograms[0].colorMultiplierLocation = glGetUniformLocation(m_shaderPrograms[0].program, "colorMultiplier");
		// Get input locations
		m_shaderPrograms[0].posLocation = glGetAttribLocation(m_shaderPrograms[0].program, "position");

	shaders.clear();

	// Init cubePix shaders

	// Init banner shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/bannerShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/bannerShader.frag"));
	m_shaderPrograms[2].program = pgr::createProgram(shaders);

	// Get uniform locations
	m_shaderPrograms[2].PVMMatrixLocation = glGetUniformLocation(m_shaderPrograms[2].program, "PVMMatrix");
	m_shaderPrograms[2].texSamplerLocation = glGetUniformLocation(m_shaderPrograms[2].program, "texSampler");
	m_shaderPrograms[2].ambientLocation = glGetUniformLocation(m_shaderPrograms[2].program, "color");
	m_shaderPrograms[2].alphaLocation = glGetUniformLocation(m_shaderPrograms[2].program, "alpha");
	m_shaderPrograms[2].booleanFlagLocation = glGetUniformLocation(m_shaderPrograms[2].program, "useTex");
	// Get input locations
	m_shaderPrograms[2].posLocation = glGetAttribLocation(m_shaderPrograms[2].program, "position");
	m_shaderPrograms[2].texCoordsLocation = glGetAttribLocation(m_shaderPrograms[2].program, "texCoords");

	shaders.clear();
}

void CVeverka::modelsInit(void) {
	// skybox
	m_skybox = new CSkybox(glm::vec3(0.0f), glm::vec3(100.0f), &m_shaderPrograms[0]);

	// TODO cubePix

	// banners
	m_banners = new CDrawable *[3];
	m_banners[0] = new CBanner(m_camera, &m_shaderPrograms[2], false);
	m_banners[1] = new CBanner(m_camera, &m_shaderPrograms[2], false);
	m_banners[2] = new CBanner(m_camera, &m_shaderPrograms[2], true, TEX_NOISE);
	((CBanner *)m_banners[0])->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	((CBanner *)m_banners[1])->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
}

CVeverka::~CVeverka(void) {
	delete m_shaderPrograms;

	delete m_skybox;

	// TODO cubePix

	for (int i = 0; i < 3; i++) delete m_banners[i];
	delete[] m_banners;
}

void CVeverka::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	// skybox
	m_skybox->draw(PMatrix, VMatrix);

	// TODO cubePix

	// banners
	if (m_state->ctrlMap[CTRL_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_state->ctrlMap[CTRL_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);
	m_banners[2]->draw(PMatrix, VMatrix);
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
