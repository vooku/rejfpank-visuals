#include "CBanner.hpp"
#include "data.hpp"
#include <algorithm>
#include <functional>

#include "pgr\pgr.hpp"

float bannerVertices[nBannerVertices * nBannerAttribsPerVertex] = {
	// x      y           z     u     v
	-1.0f, -9.0f / 16.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -9.0f / 16.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  9.0f / 16.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  9.0f / 16.0f, 0.0f, 1.0f, 1.0f
};

CBanner::CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram, const char * param, GLint multipassTexLoc)
	: CDrawable(camera->m_position + glm::normalize(camera->m_direction), glm::vec3(BANNER_SIZE), shaderProgram),
	  m_camera(camera),
	  m_inverse(false),
	  m_reducePalette(false),
	  m_deadPix(false),
	  m_colorShift(0),
	  m_tear(false),
	  m_tearN(0) {

	if (strcmp(param, BANNER_PARAM_NO_TEX) == 0) {
		m_useTex = false;
		m_alpha = 0.4f;
	}
	else if (strcmp(param, BANNER_PARAM_NO_TEX_BLACK) == 0) {
		m_useTex = false;
		m_alpha = 1.0f;
	}
	else if (strcmp(param, BANNER_PARAM_MULTIPASS) == 0) {
		m_useTex = true;
		m_geometry.texture = multipassTexLoc;
		m_alpha = 1.0f;
	}
	else {
		m_useTex = true;
		m_geometry.texture = pgr::createTexture(param, false);
		std::cout << "loaded texture: " << param << std::endl;
		if (strcmp(param, TEX_GEN_NOISE) == 0 || strcmp(param, TEX_GEN_NOISE_4TO3) == 0) m_alpha = 0.2f;
		else m_alpha = 1.0f;
	}

	glGenVertexArrays(1, &m_geometry.vertexArrayObject);
	glBindVertexArray(m_geometry.vertexArrayObject);

	glGenBuffers(1, &m_geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, nBannerVertices * nBannerAttribsPerVertex * sizeof(float), bannerVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(m_shaderProgram->posLocation);
	glVertexAttribPointer(m_shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, nBannerAttribsPerVertex * sizeof(float), (void *) 0);

	glEnableVertexAttribArray(m_shaderProgram->texCoordsLocation);
	glVertexAttribPointer(m_shaderProgram->texCoordsLocation, 2, GL_FLOAT, GL_FALSE, nBannerAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
	
	glBindVertexArray(0);
}

CBanner::~CBanner(void) {
	glDeleteTextures(1, &m_geometry.texture);
}

bool CBanner::setColor(const glm::vec3 & color) {
	if (m_useTex) return false;
	m_color = color;
	return true;
}

void CBanner::setInverse(const bool inverse) {
	m_inverse = inverse;
}

void CBanner::setReducePalette(const bool reducePalette) {
	m_reducePalette = reducePalette;
}

void CBanner::setDeadPix(const bool deadPix, const float & deadPixP) {
	m_deadPix = deadPix;
	m_deadPixP = deadPixP;
}

void CBanner::setColorShift(const int & colorShift) {
	m_colorShift = colorShift;
}

void CBanner::tear(void) {
	m_tear = true;
	m_tearN = rand() % SCREEN_TEARS + 1;

	for (int i = 0; i < m_tearN; i++) m_tearBorders[i] = rand() / (float)RAND_MAX;
	std::sort(m_tearBorders, m_tearBorders + m_tearN, std::less<float>());

	for (int i = 0; i < m_tearN + 1; i++) m_tearOffsets[i] = rand() / (float)RAND_MAX;
}

void CBanner::untear(void) {
	m_tear = false;
	m_tearN = 0;
}

void CBanner::draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	glDisable(GL_DEPTH_TEST);
	
	// just take rotation part of the view transform
	glm::mat4 billboardRotationMatrix = glm::mat4(
		VMatrix[0],
		VMatrix[1],
		VMatrix[2],
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	// inverse view rotation
	billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

	m_position = m_camera->m_position + glm::normalize(m_camera->m_direction);

	m_tempMats.MMatrix = glm::translate(glm::mat4(1.0f), m_position);
	m_tempMats.MMatrix = glm::scale(m_tempMats.MMatrix, m_scale);
	m_tempMats.MMatrix = m_tempMats.MMatrix * billboardRotationMatrix; // make billboard face the camera

	m_tempMats.PVMMatrix = PMatrix * VMatrix * m_tempMats.MMatrix;
	
	this->sendUniforms();
	
	glBindVertexArray(m_geometry.vertexArrayObject);
	if (m_useTex) glBindTexture(GL_TEXTURE_2D, m_geometry.texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, nBannerVertices);
	
	CHECK_GL_ERROR();
	glBindVertexArray(0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
}

void CBanner::sendUniforms(void) {
	glUseProgram(m_shaderProgram->program);

	glUniformMatrix4fv(m_shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.PVMMatrix));
	glUniform1i(m_shaderProgram->texSamplerLocation, 0);
	glUniform3fv(m_shaderProgram->ambientLocation, 1, glm::value_ptr(m_color));
	glUniform1f(m_shaderProgram->alphaLocation, m_alpha);
	glUniform1i(m_shaderProgram->useTexLocation, m_useTex);
	glUniform1i(m_shaderProgram->inverseLocation, m_inverse);
	glUniform1i(m_shaderProgram->reducePaletteLocation, m_reducePalette);
	glUniform1i(m_shaderProgram->deadPixLocation, m_deadPix);
	glUniform1f(m_shaderProgram->deadPixPLocation, m_deadPixP);
	glUniform1i(m_shaderProgram->tearFlagLocation, m_tear);
	glUniform1i(m_shaderProgram->tearNLocation, m_tearN);
	glUniform1fv(m_shaderProgram->tearBordersLocation, m_tearN, m_tearBorders);
	glUniform1fv(m_shaderProgram->tearOffsetsLocation, m_tearN + 1, m_tearOffsets);
	glUniform1i(m_shaderProgram->colorShiftLocation, m_colorShift);
}
