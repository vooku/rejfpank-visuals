#include "CBanner.hpp"
#include "data.hpp"

#include "pgr\pgr.hpp"

using namespace std;

CBanner::CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram, const bool useTex, const char * tex)
	: CDrawable(camera->position + glm::normalize(camera->direction), glm::vec3(BANNER_SIZE), shaderProgram),
	  m_camera(camera),
	  m_useTex(useTex)  {
	m_alpha = 1.0f;

	if (m_useTex) geometry.texture = pgr::createTexture(tex, false);
	if (strcmp(tex, TEX_NOISE) == 0) m_alpha = 0.2f;

	glGenVertexArrays(1, &geometry.vertexArrayObject);
	glBindVertexArray(geometry.vertexArrayObject);

	glGenBuffers(1, &geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, nBannerVertices * nBannerAttribsPerVertex * sizeof(float), bannerVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, nBannerAttribsPerVertex * sizeof(float), (void *) 0);

	glEnableVertexAttribArray(shaderProgram->texCoordsLocation);
	glVertexAttribPointer(shaderProgram->texCoordsLocation, 2, GL_FLOAT, GL_FALSE, nBannerAttribsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
	
	glBindVertexArray(0);
}

bool CBanner::setColor(const glm::vec3 & color) {
	if (m_useTex) return false;
	m_color = color;
	return true;
}

void CBanner::updateAlpha(const double & time) {
	const double elapsedTime = time - triggerTime;
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

	position = m_camera->position + glm::normalize(m_camera->direction);

	tempMats.MMatrix = glm::translate(glm::mat4(1.0f), position);
	tempMats.MMatrix = glm::scale(tempMats.MMatrix, scale);
	tempMats.MMatrix = tempMats.MMatrix * billboardRotationMatrix; // make billboard face the camera

	tempMats.PVMMatrix = PMatrix * VMatrix * tempMats.MMatrix;
	
	this->sendUniforms();
	
	glBindVertexArray(geometry.vertexArrayObject);
	if (m_useTex) glBindTexture(GL_TEXTURE_2D, geometry.texture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, nBannerVertices);
	
	CHECK_GL_ERROR();
	glBindVertexArray(0);
	glUseProgram(0);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void CBanner::sendUniforms(void) {
	glUseProgram(shaderProgram->program);

	glUniformMatrix4fv(shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.PVMMatrix));
	glUniform1i(shaderProgram->texSamplerLocation, 0);
	glUniform3fv(shaderProgram->ambientLocation, 1, glm::value_ptr(m_color));
	glUniform1f(shaderProgram->alphaLocation, m_alpha);
	CHECK_GL_ERROR();
	if (m_useTex) glUniform1i(shaderProgram->booleanFlagLocation, 1);
	else glUniform1i(shaderProgram->booleanFlagLocation, 0);

}
