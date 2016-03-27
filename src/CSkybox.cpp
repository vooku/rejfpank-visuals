#include "CSkybox.hpp"
#include "pgr\Shader.hpp"

using namespace std;

CSkybox::CSkybox(const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram)
	: CDrawable(position, scale, shaderProgram) {

	glGenVertexArrays(1, &geometry.vertexArrayObject);
	glBindVertexArray(geometry.vertexArrayObject);

	glGenBuffers(1, &geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry.elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeTriangles), cubeTriangles, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, cubeNAttribsPerVertex * sizeof(float), (void *)(0));
	glEnableVertexAttribArray(shaderProgram->normalLocation);
	glVertexAttribPointer(shaderProgram->normalLocation, 3, GL_FLOAT, GL_FALSE, cubeNAttribsPerVertex * sizeof(float), (void *)(3 * sizeof(float)));

	glBindVertexArray(0);
}

void CSkybox::sendUniforms(void) {
	glUseProgram(shaderProgram->program);

	glUniformMatrix4fv(shaderProgram->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.PVMmatrix));
}

void CSkybox::draw(const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix) {
	tempMats.Mmatrix = glm::translate(glm::mat4(1.0f), position);
	tempMats.Mmatrix = glm::scale(tempMats.Mmatrix, scale);
	//tempMats.Vmatrix = Vmatrix; // not necessary
	//tempMats.Pmatrix = Pmatrix; // not necessary
	tempMats.PVMmatrix = Pmatrix * Vmatrix * tempMats.Mmatrix;

	this->sendUniforms();

	glBindVertexArray(geometry.vertexArrayObject);
	glDrawElements(GL_TRIANGLES, cubeNTriangles, GL_UNSIGNED_SHORT, 0);
	CHECK_GL_ERROR();
	// reset
	glBindVertexArray(0);
	glFinish();
}