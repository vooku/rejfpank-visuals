/**
 * @author	Vadim Petrov
 * @date		2016
 */

#include "CDefinition.hpp"
#include "pgr/pgr.hpp"

CDefinition::CDefinition(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram)
	: CSong(camera, state, skybox),
	  m_bannerShaderProgram(bannerShaderProgram),
	  m_kickCount(0) {

	m_innerMap = new bool[DEF_COUNT];
	for (int i = 0; i < DEF_COUNT; i++) m_innerMap[i] = false;

	this->multipassInit();
	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Definice" << std::endl;
}

CDefinition::~CDefinition(void) {
	m_skybox->rotate(M_PI, glm::vec3(0.0f, 1.0f, 0.0f)); // just turn it back

	for (int i = 0; i < DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N; i++) delete m_honeycombs[i];
	delete[] m_honeycombs;

	for (int i = 0; i < m_honeyDataN; i++) delete m_honeyData[i];
	delete[] m_honeyData;
	
	for (int i = 0; i < m_bannersN; i++) delete m_banners[i];
	delete[] m_banners;

	delete[] m_shaderPrograms;

	delete[] m_innerMap;

	std::cout << "destroyed song: Definice" << std::endl;
}

void CDefinition::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (m_multipass) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// skybox
	m_skybox->draw(PMatrix, VMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// honeycombs
	for (int i = 0; i < DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N; i++) m_honeycombs[i]->draw(PMatrix, VMatrix);

	// banners
	if (m_innerMap[DEF_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_innerMap[DEF_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);
	if (m_innerMap[DEF_BANNER2]) m_banners[2]->draw(PMatrix, VMatrix);
	if (m_innerMap[DEF_BANNER4]) m_banners[4]->draw(PMatrix, VMatrix);

	glDisable(GL_BLEND);

	// multipass
	if (m_multipass) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, m_renderedTex);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_banners[3]->draw(PMatrix, VMatrix, m_innerMap[DEF_INVERSE], m_innerMap[DEF_REDUCE]);
	}
}

void CDefinition::update(double time) {
	float halftime = BEAT_LENGTH(175) / 32.0f;
	float fulltime = BEAT_LENGTH(175) / 16.0f;
	float triplet  = BEAT_LENGTH(175) / 3.0f;

	if (m_innerMap[DEF_BANNER0]) {
		if (time - m_banners[0]->m_triggerTime > halftime) m_innerMap[DEF_INVERSE] = true;
		if (time - m_banners[0]->m_triggerTime > fulltime) m_innerMap[DEF_BANNER0] = m_innerMap[DEF_INVERSE] = false;
	}
	if (m_innerMap[DEF_BANNER1]) {
		if (time - m_banners[1]->m_triggerTime > halftime) m_innerMap[DEF_INVERSE] = true;
		if (time - m_banners[1]->m_triggerTime > fulltime) m_innerMap[DEF_BANNER1] = m_innerMap[DEF_INVERSE] = false;
	}
	if (m_innerMap[DEF_BANNER2]) {
		if (time - m_banners[2]->m_triggerTime > halftime) m_innerMap[DEF_INVERSE] = true;
		if (time - m_banners[2]->m_triggerTime > fulltime) m_innerMap[DEF_BANNER2] = m_innerMap[DEF_INVERSE] = false;
	}

	if (m_innerMap[DEF_SWEEP]) {
		m_innerMap[DEF_BANNER0] = false;
		m_innerMap[DEF_BANNER1] = false;
		m_innerMap[DEF_BANNER2] = false;
		m_innerMap[DEF_INVERSE] = false;

			 if (time - m_sweepTriggerTime <= triplet) m_innerMap[DEF_BANNER0] = true;
		else if (time - m_sweepTriggerTime <= 2 * triplet) {
				 m_innerMap[DEF_BANNER0] = true;
				 m_innerMap[DEF_INVERSE] = true;
			 }
		else if (time - m_sweepTriggerTime <= 3 * triplet) m_innerMap[DEF_BANNER1] = true;
		else if (time - m_sweepTriggerTime <= 4 * triplet) {
			m_innerMap[DEF_BANNER1] = true;
			m_innerMap[DEF_INVERSE] = true;
		}
		else if (time - m_sweepTriggerTime <= 5 * triplet) m_innerMap[DEF_BANNER2] = true;
		else if (time - m_sweepTriggerTime <= 6 * triplet) {
			m_innerMap[DEF_BANNER2] = true;
			m_innerMap[DEF_INVERSE] = true;
		}
		else m_innerMap[DEF_SWEEP] = false;
	}

}


void CDefinition::shadersInit(void) {
	std::vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[1];

	// Init common shaders
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
		m_shaderPrograms[0].alphaLocation =			glGetUniformLocation(m_shaderPrograms[0].program, "alpha");
		m_shaderPrograms[0].fadeToBlackLocation =	glGetUniformLocation(m_shaderPrograms[0].program, "fadeToBlack");
		m_shaderPrograms[0].useTexLocation =		glGetUniformLocation(m_shaderPrograms[0].program, "useTex");
		// Get input locations
		m_shaderPrograms[0].posLocation =		glGetAttribLocation(m_shaderPrograms[0].program, "position");
		m_shaderPrograms[0].normalLocation =	glGetAttribLocation(m_shaderPrograms[0].program, "normal");
		m_shaderPrograms[0].texCoordsLocation = glGetAttribLocation(m_shaderPrograms[0].program, "texCoords");

	shaders.clear();
}

void CDefinition::modelsInit(void) {
	// skybox
	m_skybox->rotate(M_PI, glm::vec3(0.0f, 1.0f, 0.0f));

	// honeycombs
	m_honeyDataN = 4;
	m_honeyData = new CLoadedObj * [m_honeyDataN];
	m_honeyData[0] = new CLoadedObj(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_MOD, true);
	m_honeyData[1] = new CLoadedObj(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_MEANEYE, false);
	m_honeyData[2] = new CLoadedObj(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_IMPACT, true);
	m_honeyData[3] = new CLoadedObj(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_FMOON, true);

	m_honeycombs = new CLoadedObj * [DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N];
	float phi = 0.0f;
	float phiStep = M_PI / (DEF_HONEYCOMBS_N_PER_LINE / 2.0f);
	float r = DEF_HONEYCOMBS_N_PER_LINE / 4.0f + DEF_HONEYCOMBS_N_PER_LINE / 16.0f;
	float zOffset = -0.0f; // move it closer to the camera
	float xOffset = 1.7f; // move it left and right
	float alpha = 0.8f;
	glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N; i++) {
		int xFactor = (i % DEF_HONEYCOMBS_LINES_N) - (int)floor(DEF_HONEYCOMBS_LINES_N / 2.0f);
		int phiFactor = (i % DEF_HONEYCOMBS_LINES_N) % 2;
		float actualXOffset = xOffset * xFactor;
		float phiOffset = (M_PI / DEF_HONEYCOMBS_N_PER_LINE) * phiFactor;
		
		m_honeycombs[i] = new CLoadedObj(MODEL_DEF_HONEY,
										 glm::vec3(actualXOffset, r * glm::sin(phi + phiOffset), r * glm::cos(phi + phiOffset) + zOffset),
										 glm::vec3(1.0f),
										 &m_shaderPrograms[0],
										 NULL,
										 false,
										 m_honeyData[rand() % m_honeyDataN],
										 0,
										 alpha);
		m_honeycombs[i]->rotate(M_PI * 3.0f / 2.0f - phi - phiOffset, axis);

		phi += phiStep;
	}

	m_camera->m_position = glm::vec3(0.0f, 0.0f, - r - 4.0f);

	// banners
	m_bannersN = 5;
	m_banners = new CBanner * [m_bannersN];
	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram, (m_state->ctrlMap[CTRL_4TO3] ? TEX_DEF_1_4TO3 : TEX_DEF_1));
	m_banners[1] = new CBanner(m_camera, m_bannerShaderProgram, (m_state->ctrlMap[CTRL_4TO3] ? TEX_DEF_2_4TO3 : TEX_DEF_2));
	m_banners[2] = new CBanner(m_camera, m_bannerShaderProgram, (m_state->ctrlMap[CTRL_4TO3] ? TEX_DEF_3_4TO3 : TEX_DEF_3));
	m_banners[3] = new CBanner(m_camera, m_bannerShaderProgram, BANNER_PARAM_MULTIPASS, m_renderedTex); // multipass
	m_banners[4] = new CBanner(m_camera, m_bannerShaderProgram, (m_state->ctrlMap[CTRL_4TO3] ? TEX_GEN_FIN_4TO3 : TEX_GEN_FIN)); // fin
}

void CDefinition::sweepBanners(const double time) {
	m_innerMap[DEF_SWEEP] = true;
	m_sweepTriggerTime = time;
}

void CDefinition::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	double time = glfwGetTime();
	//-------------------------------------------------------------------> AKAI MPX16
	if (status == MIDI_NOTE_ON_CH10) {
		switch (note) {
		case MPX16_PAD01: // tech 1
			this->sweepBanners(time);
			break;
		case MPX16_PAD02: // tech 2
			this->sweepBanners(time);
			break;
		case MPX16_PAD03: // tech 3
			this->sweepBanners(time);
			break;
		case MPX16_PAD04: // tech 4
			this->sweepBanners(time);
			break;
		case MPX16_PAD05: // block tech 1
			break;			  
		case MPX16_PAD06: // block tech 2
			break;			  
		case MPX16_PAD07: // block tech 3
			break;			  
		case MPX16_PAD08: // block tech 4
			break;
		case MPX16_PAD09: // laser
			break;
		case MPX16_PAD10: // laser
			break;
		case MPX16_PAD11: // block definition
			break;
		case MPX16_PAD12: // definition
			break;
		case MPX16_PAD13:  // lazer + 2
			break;
		case MPX16_PAD14: // lazer + 4
			break;
		case MPX16_PAD15: // block mlp
			m_innerMap[DEF_BANNER4] = false;
			break;
		case MPX16_PAD16: // mlp
			m_innerMap[DEF_BANNER4] = true;
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
			m_innerMap[DEF_BANNER0] = true;
			m_banners[0]->m_triggerTime = time;
			break;
		//case MIDI_DRUM_KICK2:
		//	break;
		case MIDI_DRUM_SNARE1:
			m_innerMap[DEF_BANNER1] = true;
			m_banners[1]->m_triggerTime = time;
			break;
		case MIDI_DRUM_HIHAT_OPEN:
			m_innerMap[DEF_BANNER2] = true;
			m_banners[2]->m_triggerTime = time;
			break;
		//case MIDI_DRUM_TOM_LOW1:
		//	break;
		//case MIDI_DRUM_TOM_MID1:
		//	break;
		//case MIDI_DRUM_TOM_HIGH1: // laser tom
		//	break;
		//case MIDI_DRUM_CYMBAL_CRASH1:
		//	break;
		//case MIDI_DRUM_CYMBAL_SPLASH:
		//	break;
		//case MIDI_DRUM_CYMBAL_RIDE1:
		//	break;
		default:
			std::cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
}