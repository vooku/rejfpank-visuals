/**
* @author	Vadim Petrov
* @date	2016
*/

#include "CSongHeros.hpp"
#include "pgr/pgr.hpp"

CSongHeros::CSongHeros(CCamera * camera, TControlState * state, CSkybox * skybox, TCommonShaderProgram * bannerShaderProgram)
	: CSong(camera, state, skybox),
	m_bannerShaderProgram(bannerShaderProgram),
	m_colorShift(0),
	m_reduceTime(0.0) {

	m_innerMap = new bool[HER_COUNT];
	for (int i = 0; i < HER_COUNT; i++) m_innerMap[i] = false;

	this->multipassInit();
	this->shadersInit();
	this->modelsInit();

	std::cout << "loaded song: Heros" << std::endl;
}

CSongHeros::~CSongHeros(void) {
	for (int i = 0; i < m_spheresN; i++) delete m_spheres[i];
	delete[] m_spheres;

	delete m_sphereData;

	delete[] m_triggerTimes;
	delete[] m_scales;

	for (int i = 0; i < m_bannersN; i++) delete m_banners[i];
	delete[] m_banners;

	delete[] m_shaderPrograms;

	delete[] m_innerMap;

	m_skybox->reset();

	std::cout << "destroyed song: Heros" << std::endl;
}

void CSongHeros::redraw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (m_multipass) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// skybox
	m_skybox->draw(PMatrix, VMatrix);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// spheres
	glUseProgram(m_shaderPrograms[0].program);
	glUniform1f(m_shaderPrograms[0].whiteFlagLocation, true);
	if (m_innerMap[HER_KICK1]) m_spheres[0]->draw(PMatrix, VMatrix);
	if (m_innerMap[HER_KICK2]) m_spheres[1]->draw(PMatrix, VMatrix);
	if (m_innerMap[HER_SNR]) {
		m_spheres[2]->draw(PMatrix, VMatrix);
		m_spheres[3]->draw(PMatrix, VMatrix);
	}
	if (m_innerMap[HER_TOM1]) m_spheres[5]->draw(PMatrix, VMatrix);
	if (m_innerMap[HER_TOM2]) m_spheres[6]->draw(PMatrix, VMatrix);
	if (m_innerMap[HER_TOM3]) m_spheres[4]->draw(PMatrix, VMatrix);
	

	// banners
	if (m_innerMap[HER_BANNER0]) m_banners[0]->draw(PMatrix, VMatrix);
	if (m_innerMap[HER_BANNER1]) m_banners[1]->draw(PMatrix, VMatrix);

	glDisable(GL_BLEND);

	// multipass
	if (m_multipass) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, m_renderedTex);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_banners[2]->draw(PMatrix, VMatrix, m_innerMap[HER_INVERSE], m_innerMap[HER_REDUCE], m_colorShift);
	}
}

void CSongHeros::update(double time) {
	m_skybox->rotate(0.1f * ROTATION_ANGLE_DELTA, glm::vec3(1.0f, 0.0f, 0.0f));

	if (time - m_reduceTime > 2 * BEAT_LENGTH(175)) m_innerMap[HER_REDUCE] = false;

	for (int i = 0; i < m_spheresN; i++) {
		m_spheres[i]->rotate(ROTATION_ANGLE_DELTA, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glm::vec3 scale = m_scales[i] * static_cast<float>(exp(-EXP_DECAY * (time - m_triggerTimes[i])));
		m_spheres[i]->scale(scale);
	}
}


void CSongHeros::shadersInit(void) {
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

void CSongHeros::modelsInit(void) {
	// skybox
	m_skybox->m_colorMultiplier = 0.3f;

	// spheres
	m_sphereData = new CModel(MODEL_HER_SPHERE, glm::vec3(0.0f), glm::vec3(1.0f), &m_shaderPrograms[0]);

	m_spheresN = 7;
	m_spheres = new CModel *[m_spheresN];

	m_scales = new glm::vec3[m_spheresN];
	m_scales[0] = glm::vec3(5.5f);
	m_scales[1] = glm::vec3(5.5f);
	m_scales[2] = glm::vec3(3.0f);
	m_scales[3] = glm::vec3(3.0f);
	m_scales[4] = glm::vec3(10.0f);
	m_scales[5] = glm::vec3(6.0f);
	m_scales[6] = glm::vec3(5.0f);

	const float shininess = 2.0f;
	const float alpha = 0.4f;

	// kick1
	m_spheres[0] = new CModel(MODEL_HER_SPHERE, glm::vec3(8.0f, 0.0f, 3.0f), m_scales[0], &m_shaderPrograms[0], NULL, false, m_sphereData);
	m_spheres[0]->setMaterial(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), shininess, alpha);
	// kick2
	m_spheres[1] = new CModel(MODEL_HER_SPHERE, glm::vec3(-8.0f, 0.0f, 3.0f), m_scales[1], &m_shaderPrograms[0], NULL, false, m_sphereData);
	m_spheres[1]->setMaterial(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), shininess, alpha);
	// snr1
	m_spheres[2] = new CModel(MODEL_HER_SPHERE, glm::vec3(-5.0f, 3.0f, 3.0f), m_scales[2], &m_shaderPrograms[0], NULL, false, m_sphereData);
	m_spheres[2]->setMaterial(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), shininess, alpha);
	// snr2
	m_spheres[3] = new CModel(MODEL_HER_SPHERE, glm::vec3( 5.0f, 3.0f, 3.0f), m_scales[3], &m_shaderPrograms[0], NULL, false, m_sphereData);
	m_spheres[3]->setMaterial(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), shininess, alpha);
	// tom3
	m_spheres[4] = new CModel(MODEL_HER_SPHERE, glm::vec3(0.0f, 0.0f, 3.0f), m_scales[4], &m_shaderPrograms[0], NULL, false, m_sphereData);
	m_spheres[4]->setMaterial(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.5f), shininess, 0.9f);
	// tom1
	m_spheres[5] = new CModel(MODEL_HER_SPHERE, glm::vec3( 0.0f, -1.0f, 3.0f), m_scales[5], &m_shaderPrograms[0], NULL, false, m_sphereData);
	m_spheres[5]->setMaterial(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f), shininess, alpha);
	// tom2
	m_spheres[6] = new CModel(MODEL_HER_SPHERE, glm::vec3( 0.0f, 2.0f, 3.0f), m_scales[6], &m_shaderPrograms[0], NULL, false, m_sphereData);
	m_spheres[6]->setMaterial(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), shininess, alpha);

	m_triggerTimes = new double[m_spheresN];
	for (int i = 0; i < m_spheresN; i++) m_triggerTimes[i] = 0.0;

	// banners
	m_bannersN = 3;
	m_banners = new CBanner *[m_bannersN];
	m_banners[0] = new CBanner(m_camera, m_bannerShaderProgram, TEX_HER_1);
	m_banners[1] = new CBanner(m_camera, m_bannerShaderProgram, TEX_HER_2);
	m_banners[2] = new CBanner(m_camera, m_bannerShaderProgram, BANNER_PARAM_MULTIPASS, m_renderedTex); // multipass
}

void CSongHeros::midiIn(const unsigned int status, const unsigned int note, const unsigned int velocity) {
	double time = glfwGetTime();
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
			m_innerMap[HER_KICK1] = true;
			m_triggerTimes[0] = time;
			break;
		case MIDI_DRUM_KICK2:
			m_innerMap[HER_KICK2] = true;
			m_triggerTimes[1] = time;
			break;
		case MIDI_DRUM_SNARE1:
			m_innerMap[HER_SNR] = true;
			m_triggerTimes[2] = time;
			m_triggerTimes[3] = time;
			break;
		case MIDI_DRUM_SNARE2:
			break;
		case MIDI_DRUM_HIHAT_OPEN:
			m_colorShift = (m_colorShift + 1) % 3;
			break;
		case MIDI_DRUM_HIHAT_CLOSED:
			m_colorShift = (m_colorShift + 1) % 3;
			break;
		case MIDI_DRUM_TOM_LOW1:
			m_innerMap[HER_TOM3] = true;
			m_triggerTimes[4] = time;
			break;
		case MIDI_DRUM_TOM_MID1:
			m_innerMap[HER_TOM1] = true;
			m_triggerTimes[5] = time;
			break;
		case MIDI_DRUM_TOM_HIGH1:
			m_innerMap[HER_TOM2] = true;
			m_triggerTimes[6] = time;
			break;
		case MIDI_DRUM_CYMBAL_CRASH1:
			m_colorShift = (m_colorShift + 1) % 3;
			break;
		case MIDI_DRUM_CYMBAL_SPLASH:
			m_innerMap[HER_REDUCE] = true;
			m_reduceTime = time;
			break;
		case MIDI_DRUM_CYMBAL_RIDE1: // snare lol
			break;
		default:
			std::cout << "Unresolved midi note from SR16: " << status << " " << note << " " << velocity << std::endl;
			break;
		}
	}
}