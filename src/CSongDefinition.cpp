/**
 * @author	Vadim Petrov
 * @date	2016
 */

#include "CSongDefinition.hpp"
#include "pgr/pgr.hpp"

CSongDefinition::CSongDefinition(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram)
	: CSong(camera, state, skybox),
	  m_bannerShaderProgram(bannerShaderProgram),
	  m_colorShift(0) {

	m_innerMap = new bool[DEF_COUNT];
	for (int i = 0; i < DEF_COUNT; i++) m_innerMap[i] = false;

	for (int i = 0; i < 4; i++) m_triggerTimes[i] = 0.0;

	this->multipassInit();
	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Definice" << std::endl;
}

CSongDefinition::~CSongDefinition(void) {
	for (int i = 0; i < DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N; i++) delete m_honeycombs[i];
	delete[] m_honeycombs;

	for (int i = 0; i < m_honeyDataN; i++) delete m_honeyData[i];
	delete[] m_honeyData;
	
	for (int i = 0; i < m_bannersN; i++) delete m_banners[i];
	delete[] m_banners;

	delete[] m_shaderPrograms;

	delete[] m_innerMap;

	m_skybox->m_colorMultiplier = 1.0f;

	std::cout << "destroyed song: Definice" << std::endl;
}

void CSongDefinition::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (m_multipass) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// skybox
	if (m_innerMap[DEF_BACKGROUND]) m_skybox->draw(PMatrix, VMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// honeycombs
	if (m_innerMap[DEF_BACKGROUND])
		for (int i = 0; i < DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N; i++) {
			glUseProgram(m_shaderPrograms[0].program);
			glUniform1f(m_shaderPrograms[0].whiteFlagLocation, false);
			glUniform1f(m_shaderPrograms[0].redFlagLocation, m_innerMap[DEF_RED]);
			glUniform1f(m_shaderPrograms[0].blueFlagLocation, m_innerMap[DEF_BLUE]);
			glUniform1f(m_shaderPrograms[0].pointFlagLocation, m_innerMap[DEF_POINT]);
			glUniform3fv(m_shaderPrograms[0].cameraPositionLocation, 1, glm::value_ptr(m_camera->m_position));
			m_honeycombs[i]->draw(PMatrix, VMatrix);
		}

	// banners
	if (m_innerMap[DEF_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_innerMap[DEF_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);
	if (m_innerMap[DEF_BANNER2]) m_banners[2]->draw(PMatrix, VMatrix);

	glDisable(GL_BLEND);

	// multipass
	if (m_multipass) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, m_renderedTex);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_banners[3]->draw(PMatrix, VMatrix, m_innerMap[DEF_INVERSE], m_innerMap[DEF_REDUCE], m_colorShift);
	}
}

void CSongDefinition::update(double time) {
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

			 if (time - m_triggerTimes[0] <= triplet) m_innerMap[DEF_BANNER0] = true;
		else if (time - m_triggerTimes[0] <= 2 * triplet) {
				 m_innerMap[DEF_BANNER0] = true;
				 m_innerMap[DEF_INVERSE] = true;
			 }
		else if (time - m_triggerTimes[0] <= 3 * triplet) m_innerMap[DEF_BANNER1] = true;
		else if (time - m_triggerTimes[0] <= 4 * triplet) {
			m_innerMap[DEF_BANNER1] = true;
			m_innerMap[DEF_INVERSE] = true;
		}
		else if (time - m_triggerTimes[0] <= 5 * triplet) m_innerMap[DEF_BANNER2] = true;
		else if (time - m_triggerTimes[0] <= 6 * triplet) {
			m_innerMap[DEF_BANNER2] = true;
			m_innerMap[DEF_INVERSE] = true;
		}
		else m_innerMap[DEF_SWEEP] = false;
	}

	if (m_innerMap[DEF_POINT]) if (time - m_triggerTimes[1] > fulltime * 4) m_innerMap[DEF_POINT] = false;
	if (m_innerMap[DEF_RED]) if (time - m_triggerTimes[2] > fulltime * 4) m_innerMap[DEF_RED] = false;
	if (m_innerMap[DEF_BLUE]) if (time - m_triggerTimes[3] > fulltime * 4) m_innerMap[DEF_BLUE] = false;

	m_camera->placeOnCircle(CAMERA_ENCIRCLE_SPEED * time, m_r + m_camOffset, glm::vec3(1.0f, 0.0f, 0.0f));
}


void CSongDefinition::shadersInit(void) {
	std::vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[1];

	// Init common shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/commonShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/commonShader.frag"));
	m_shaderPrograms[0].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[0].PVMMatrixLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "PVMMatrix");
		m_shaderPrograms[0].VMatrixLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "VMatrix");
		m_shaderPrograms[0].MMatrixLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "MMatrix");
		m_shaderPrograms[0].normalMatrixLocation	= glGetUniformLocation(m_shaderPrograms[0].program, "normalMatrix");
		m_shaderPrograms[0].diffuseLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "material.diffuse");
		m_shaderPrograms[0].ambientLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "material.ambient");
		m_shaderPrograms[0].specularLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "material.specular");
		m_shaderPrograms[0].shininessLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "material.shininess");
		m_shaderPrograms[0].alphaLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "alpha");
		m_shaderPrograms[0].whiteFlagLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "whiteFlag");
		m_shaderPrograms[0].redFlagLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "redFlag");
		m_shaderPrograms[0].blueFlagLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "blueFlag");
		m_shaderPrograms[0].pointFlagLocation		= glGetUniformLocation(m_shaderPrograms[0].program, "pointFlag");
		m_shaderPrograms[0].cameraPositionLocation	= glGetUniformLocation(m_shaderPrograms[0].program, "cameraPosition");
		m_shaderPrograms[0].useTexLocation			= glGetUniformLocation(m_shaderPrograms[0].program, "useTex");
		// Get input locations
		m_shaderPrograms[0].posLocation			= glGetAttribLocation(m_shaderPrograms[0].program, "position");
		m_shaderPrograms[0].normalLocation		= glGetAttribLocation(m_shaderPrograms[0].program, "normal");
		m_shaderPrograms[0].texCoordsLocation	= glGetAttribLocation(m_shaderPrograms[0].program, "texCoords");

	shaders.clear();
}

void CSongDefinition::modelsInit(void) {
	// skybox
	m_skybox->m_colorMultiplier = 0.2f;

	// honeycombs
	m_honeyDataN = 4;
	m_honeyData = new CModel * [m_honeyDataN];
	m_honeyData[0] = new CModel(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_MOD, true);
	m_honeyData[1] = new CModel(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_MEANEYE, false);
	m_honeyData[2] = new CModel(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_IMPACT, true);
	m_honeyData[3] = new CModel(MODEL_DEF_HONEY, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0], TEX_DEF_FMOON, true);

	m_honeycombs = new CModel * [DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N];
	float phi = 0.0f;
	float phiStep = M_PI / (DEF_HONEYCOMBS_N_PER_LINE / 2.0f);
	m_r = DEF_HONEYCOMBS_N_PER_LINE / 4.0f + DEF_HONEYCOMBS_N_PER_LINE / 16.0f;
	m_camOffset = DEF_HONEYCOMBS_LINES_N * 0.9f;
	float zOffset = -0.0f; // move it closer to the camera
	float xOffset = 1.7f; // move it left and right
	float alpha = 0.8f;
	glm::vec3 axis = glm::vec3(1.0f, 0.0f, 0.0f);
	for (int i = 0; i < DEF_HONEYCOMBS_N_PER_LINE * DEF_HONEYCOMBS_LINES_N; i++) {
		int xFactor = (i % DEF_HONEYCOMBS_LINES_N) - (int)floor(DEF_HONEYCOMBS_LINES_N / 2.0f);
		int phiFactor = (i % DEF_HONEYCOMBS_LINES_N) % 2;
		float actualXOffset = xOffset * xFactor;
		float phiOffset = (M_PI / DEF_HONEYCOMBS_N_PER_LINE) * phiFactor;
		
		m_honeycombs[i] = new CModel(MODEL_DEF_HONEY,
										 glm::vec3(actualXOffset, m_r * glm::sin(phi + phiOffset), m_r * glm::cos(phi + phiOffset) + zOffset),
										 glm::vec3(1.0f),
										 &m_shaderPrograms[0],
										 NULL,
										 false,
										 m_honeyData[rand() % m_honeyDataN],
										 0,
										 alpha);
		m_honeycombs[i]->rotate(M_PI / 2.0f - phi - phiOffset, axis);

		phi += phiStep;
	}

	// banners
	m_bannersN = 4;
	m_banners = new CBanner * [m_bannersN];
	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram, (m_state->ctrlMap[CTRL_4TO3] ? TEX_DEF_1_4TO3 : TEX_DEF_1));
	m_banners[1] = new CBanner(m_camera, m_bannerShaderProgram, (m_state->ctrlMap[CTRL_4TO3] ? TEX_DEF_2_4TO3 : TEX_DEF_2));
	m_banners[2] = new CBanner(m_camera, m_bannerShaderProgram, (m_state->ctrlMap[CTRL_4TO3] ? TEX_DEF_3_4TO3 : TEX_DEF_3));
	m_banners[3] = new CBanner(m_camera, m_bannerShaderProgram, BANNER_PARAM_MULTIPASS, m_renderedTex); // multipass
}

void CSongDefinition::sweepBanners(const double time) {
	m_innerMap[DEF_SWEEP] = true;
	m_triggerTimes[0] = time;
}

void CSongDefinition::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
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
			m_triggerTimes[1] = time;
			m_innerMap[DEF_POINT] = true;
			break;
		case MPX16_PAD10: // laser
			m_triggerTimes[1] = time;
			m_innerMap[DEF_POINT] = true;
			break;
		case MPX16_PAD11: // block definition
			break;
		case MPX16_PAD12: // definition
			break;
		case MPX16_PAD13:  // lazer + 2
			m_triggerTimes[1] = time;
			m_innerMap[DEF_POINT] = true;
			break;
		case MPX16_PAD14: // lazer + 4
			m_triggerTimes[1] = time;
			m_innerMap[DEF_POINT] = true;
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
			m_innerMap[DEF_BANNER0] = true;
			m_banners[0]->m_triggerTime = time;
			break;
		case MIDI_DRUM_KICK2:
			m_innerMap[DEF_BACKGROUND] = true;
			break;
		case MIDI_DRUM_SNARE1:
			m_innerMap[DEF_BANNER1] = true;
			m_banners[1]->m_triggerTime = time;
			break;
		case MIDI_DRUM_SNARE2: // nothing, used just for stronger transient
			break;
		case MIDI_DRUM_HIHAT_OPEN:
			m_innerMap[DEF_BANNER2] = true;
			m_banners[2]->m_triggerTime = time;
			break;
		case MIDI_DRUM_HIHAT_CLOSED:
			m_colorShift = (m_colorShift + 1) % 3;
			break;
		case MIDI_DRUM_TOM_LOW1:
			m_triggerTimes[2] = time;
			m_innerMap[DEF_RED] = true;
			break;
		case MIDI_DRUM_TOM_MID1:
			m_triggerTimes[3] = time;
			m_innerMap[DEF_BLUE] = true;
			break;
		//case MIDI_DRUM_TOM_HIGH1: // laser tom
		//	break;
		case MIDI_DRUM_CYMBAL_CRASH1:
			m_colorShift = (m_colorShift + 1) % 3;
			break;
		case MIDI_DRUM_CYMBAL_SPLASH:
			m_colorShift = (m_colorShift + 1) % 3;
			break;
		case MIDI_DRUM_CYMBAL_RIDE1:
			m_colorShift = (m_colorShift + 1) % 3;
			break;
		default:
			std::cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
}