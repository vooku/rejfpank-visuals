#include "CBanner.hpp"
#include "data.hpp"

#include "pgr\pgr.hpp"

using namespace std;

CBanner::CBanner(CCamera * camera, TCommonShaderProgram * shaderProgram)
	: CDrawable(camera->position + glm::normalize(camera->direction), glm::vec3(BANNER_SIZE), shaderProgram),
	  camera (camera) {
	alpha = 0.2f;

	geometry.texture = pgr::createTexture(TEX_NOISE, false);

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

	position = camera->position + glm::normalize(camera->direction);

	tempMats.MMatrix = glm::translate(glm::mat4(1.0f), position);
	tempMats.MMatrix = glm::scale(tempMats.MMatrix, scale);
	tempMats.MMatrix = tempMats.MMatrix * billboardRotationMatrix; // make billboard face the camera

	tempMats.PVMMatrix = PMatrix * VMatrix * tempMats.MMatrix;

	this->sendUniforms();

	glBindVertexArray(geometry.vertexArrayObject);
	glBindTexture(GL_TEXTURE_2D, geometry.texture);
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
	glUniform1f(shaderProgram->alphaLocation, alpha);
}
