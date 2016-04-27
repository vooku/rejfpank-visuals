#include "CSquirrel.hpp"
#include "pgr/pgr.hpp"
#include <vector>

CSquirrel::CSquirrel(CCamera * camera, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram, TControlState * state)
	: CSong(camera, skybox),
	  m_bannerShaderProgram(bannerShaderProgram),
	  m_state(state),
	  m_kickCount(0),
	  m_rideTriggerTime(0),
	  m_snareTriggerTime(0),
	  m_camOffset(CAMERA_ROTATE_SPEED) {

	m_innerMap = new bool[SQUIR_COUNT];
	for (int i = 0; i < SQUIR_COUNT; i++) m_innerMap[i] = false;

	this->multipassInit();
	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Veverka" << std::endl;
}

CSquirrel::~CSquirrel(void) {
	delete m_squirrel1;
	delete m_squirrel2;

	for (unsigned int i = 0; i < m_bannersCount; i++) delete m_banners[i];
	delete[] m_banners;

	delete[] m_shaderPrograms;

	delete[] m_innerMap;

	glDeleteFramebuffers(1, &m_frameBufferObject);

	std::cout << "destroyed song: Veverka" << std::endl;
}

void CSquirrel::shadersInit(void) {
	std::vector<GLuint> shaders;
	m_shaderPrograms = new TCommonShaderProgram[1];

	// Init pix shaders
	shaders.push_back(pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/pixShader.vert"));
	shaders.push_back(pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/pixShader.frag"));
	m_shaderPrograms[0].program = pgr::createProgram(shaders);

		// Get uniform locations
		m_shaderPrograms[0].PVMMatrixLocation = glGetUniformLocation(m_shaderPrograms[0].program, "PVMMatrix");
		// Get input locations
		m_shaderPrograms[0].posLocation =		glGetAttribLocation(m_shaderPrograms[0].program, "position");
		m_shaderPrograms[0].ambientLocation =	glGetAttribLocation(m_shaderPrograms[0].program, "color");
		m_shaderPrograms[0].offsetLocation =	glGetAttribLocation(m_shaderPrograms[0].program, "offset");

	shaders.clear();
}

void CSquirrel::modelsInit(void) {
	// squirrels
	float scale = m_state->ctrlMap[CTRL_4TO3] ? 1.0f : 1.3f;
	m_squirrel1 = new CObjectPix(IMG_SQUIRREL1, m_camera->m_position + glm::normalize(m_camera->m_direction), glm::vec3(scale), &m_shaderPrograms[0]);
	m_squirrel2 = new CObjectPix(IMG_SQUIRREL2, m_camera->m_position + glm::normalize(m_camera->m_direction), glm::vec3(scale), &m_shaderPrograms[0]);

	// banners
	m_bannersCount = 4;
	m_banners = new CBanner * [m_bannersCount];
	
	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram); // red
	m_banners[0]->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	
	m_banners[1] = new CBanner(m_camera, m_bannerShaderProgram); // green
	m_banners[1]->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	
	m_banners[2] = new CBanner(m_camera, m_bannerShaderProgram); // blue
	m_banners[2]->setColor(glm::vec3(0.0f, 0.0f, 1.0f));

	m_banners[3] = new CBanner(m_camera, m_bannerShaderProgram, BANNER_PARAM_MULTIPASS, m_renderedTex); // multipass
}

void CSquirrel::multipassInit(void) {
	glGenFramebuffers(1, &m_frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);

	glGenTextures(1, &m_renderedTex);
	glBindTexture(GL_TEXTURE_2D, m_renderedTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_state->winWidth, m_state->winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); // empty image

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderedTex, 0);

	glGenRenderbuffers(1, &m_renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_state->winWidth, m_state->winHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferObject);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: Cannot init frame buffer." << std::endl;
	else m_innerMap[SQUIR_FBUFF] = true;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CSquirrel::nextBanner(void) {
	if (m_innerMap[SQUIR_BANNER0]) {
		m_innerMap[SQUIR_BANNER0] = false;
		m_innerMap[SQUIR_BANNER1] = true;
	}
	else if (m_innerMap[SQUIR_BANNER1]) {
		m_innerMap[SQUIR_BANNER1] = false;
		m_innerMap[SQUIR_BANNER2] = true;
	}
	else if (m_innerMap[SQUIR_BANNER2]) {
		m_innerMap[SQUIR_BANNER2] = false;
		m_innerMap[SQUIR_BANNER0] = true;
	}
	else m_innerMap[SQUIR_BANNER0] = true;
}

void CSquirrel::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (m_innerMap[SQUIR_FBUFF]) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// skybox
	m_skybox->draw(PMatrix, VMatrix);

	// squirrels
	if (m_innerMap[SQUIR_SQUIRREL1]) m_squirrel1->draw(PMatrix, VMatrix);
	if (m_innerMap[SQUIR_SQUIRREL2]) m_squirrel2->draw(PMatrix, VMatrix);

	// banners
	if (m_innerMap[SQUIR_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_innerMap[SQUIR_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);
	if (m_innerMap[SQUIR_BANNER2]) m_banners[2]->draw(PMatrix, VMatrix);
	
	// multipass
	if (m_innerMap[SQUIR_FBUFF]) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, m_renderedTex);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_banners[3]->draw(PMatrix, VMatrix, m_innerMap[SQUIR_INVERSE], m_innerMap[SQUIR_REDUCE]);
	}
}


void CSquirrel::update(double time) {
	m_squirrel1->m_position = m_camera->m_position + glm::normalize(m_camera->m_direction);
	m_squirrel2->m_position = m_camera->m_position + glm::normalize(m_camera->m_direction);

	m_squirrel1->updatePtSize(time);
	m_squirrel2->updatePtSize(time);

	if (m_innerMap[SQUIR_ROTATE]) m_camera->rotate(m_camOffset.x, m_camOffset.y);

	if (m_innerMap[SQUIR_REDUCE] && time - m_rideTriggerTime > BEAT_LENGTH(175)) m_innerMap[SQUIR_REDUCE] = false;

	if (m_innerMap[SQUIR_TEAR] && time - m_snareTriggerTime > BEAT_LENGTH(175) / 2.0f) {
		m_innerMap[SQUIR_TEAR] = false;
		m_banners[3]->untear();
	}
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
		case MIDI_DRUM_KICK1: // ghost kick
			if (m_kickCount % 4 == 0) {
				double time = glfwGetTime();
				m_squirrel1->m_triggerTime = time;
				m_squirrel2->m_triggerTime = time;
			}
			if (m_kickCount >= 16) m_innerMap[SQUIR_ROTATE] = true;
			if (m_innerMap[SQUIR_ROTATE] && m_kickCount % 16 == 0) {
				if (rand() > RAND_MAX / 2.0f) m_camOffset.x *= -1;
				else m_camOffset.y *= -1;
			}
			m_kickCount++;
			break;
		case MIDI_DRUM_KICK2:
			if (!m_innerMap[SQUIR_SQUIRREL1] && !m_innerMap[SQUIR_SQUIRREL2])
				m_innerMap[SQUIR_SQUIRREL1] = true;
			else {
				m_innerMap[SQUIR_SQUIRREL1] = !m_innerMap[SQUIR_SQUIRREL1];
				m_innerMap[SQUIR_SQUIRREL2] = !m_innerMap[SQUIR_SQUIRREL2];
			}
			break;
		case MIDI_DRUM_SNARE1:
			m_innerMap[SQUIR_TEAR] = true;
			m_banners[3]->tear();
			m_snareTriggerTime = glfwGetTime();
			break;
		case MIDI_DRUM_TOM_LOW1:
			this->nextBanner();
			break;
		case MIDI_DRUM_TOM_MID1:
			this->nextBanner();
			break;
		case MIDI_DRUM_TOM_HIGH1:
			this->nextBanner();
			break;
		case MIDI_DRUM_CYMBAL_CRASH1:
			m_innerMap[SQUIR_INVERSE] = true;
			break;
		case MIDI_DRUM_CYMBAL_SPLASH:
			m_innerMap[SQUIR_INVERSE] = false;
			break;
		case MIDI_DRUM_CYMBAL_RIDE1:
			m_innerMap[SQUIR_REDUCE] = true;
			m_rideTriggerTime = glfwGetTime();
			break;
		default:
			std::cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
}