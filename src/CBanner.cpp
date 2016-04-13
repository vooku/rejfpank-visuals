#include "CBanner.hpp"
#include "data.hpp"

#include "pgr\pgr.hpp"

using namespace std;

CBanner::CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram, const bool useTex, const char * tex)
	: CDrawable(camera->m_position + glm::normalize(camera->m_direction), glm::vec3(BANNER_SIZE), shaderProgram),
	  m_camera(camera),
	  m_useTex(useTex)  {

	if (m_useTex) m_geometry.texture = pgr::createTexture(tex, false);
	/*if (strcmp(tex, TEX_NOISE) == 0)*/ m_alpha = 0.2f;

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

bool CBanner::setColor(const glm::vec3 & color) {
	if (m_useTex) return false;
	m_color = color;
	return true;
}

void CBanner::updateAlpha(const double & time) {
	const double elapsedTime = time - m_triggerTime;
	m_alpha = (-1.0f / 3.0f) * elapsedTime + 1.0f;
}

void CBanner::draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void CBanner::sendUniforms(void) {
	glUseProgram(m_shaderProgram->program);

	glUniformMatrix4fv(m_shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.PVMMatrix));
	glUniform1i(m_shaderProgram->texSamplerLocation, 0);
	glUniform3fv(m_shaderProgram->ambientLocation, 1, glm::value_ptr(m_color));
	glUniform1f(m_shaderProgram->alphaLocation, m_alpha);
	CHECK_GL_ERROR();
	if (m_useTex) glUniform1i(m_shaderProgram->booleanFlagLocation, 1);
	else glUniform1i(m_shaderProgram->booleanFlagLocation, 0);

}
