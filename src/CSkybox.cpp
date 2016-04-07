#include "CSkybox.hpp"
#include "pgr\Shader.hpp"

using namespace std;

CSkybox::CSkybox(const glm::vec3 & position, const glm::vec3 & scale, TCommonShaderProgram * shaderProgram)
	: CDrawable(position, scale, shaderProgram),
	  colorMultiplier(0.7f) {
	geometry.numTriangles = nCubeTriangles;

	glGenVertexArrays(1, &geometry.vertexArrayObject);
	glBindVertexArray(geometry.vertexArrayObject);

	glGenBuffers(1, &geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, nCubeVertices * nCubeAttribsPerVertex * sizeof(float), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry.elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nCubeVertices * nCubeAttribsPerVertex * sizeof(float), cubeTriangles, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, nCubeAttribsPerVertex * sizeof(float), (void *) 0);
	
	glBindVertexArray(0);
}

void CSkybox::sendUniforms(void) {
	glUseProgram(shaderProgram->program);

	glUniformMatrix4fv(shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.PVMMatrix));
	glUniform1f(shaderProgram->colorMultiplierLocation, colorMultiplier);
}

void CSkybox::draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	tempMats.MMatrix = glm::translate(glm::mat4(1.0f), position);
	tempMats.MMatrix = glm::scale(tempMats.MMatrix, scale);
	tempMats.MMatrix = glm::rotate(tempMats.MMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//tempMats.VMatrix = VMatrix;
	tempMats.PVMMatrix = PMatrix * VMatrix * tempMats.MMatrix;

	this->sendUniforms();

	glBindVertexArray(geometry.vertexArrayObject);
	glDrawElements(GL_TRIANGLES, geometry.numTriangles, GL_UNSIGNED_SHORT, 0);
	
	CHECK_GL_ERROR();
	// reset
	glBindVertexArray(0);
	glFinish();
}