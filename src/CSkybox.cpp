#include "CSkybox.hpp"

CSkybox::CSkybox(const glm::vec3 & position, const glm::vec3 & scale, TCommonShaderProgram * shaderProgram)
	: CDrawable(position, scale, shaderProgram),
	  m_colorMultiplier(0.7f) {
	m_geometry.numTriangles = nCubeTriangles;

	glGenVertexArrays(1, &m_geometry.vertexArrayObject);
	glBindVertexArray(m_geometry.vertexArrayObject);

	glGenBuffers(1, &m_geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, nCubeVertices * nCubeAttribsPerVertex * sizeof(float), cubeVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_geometry.elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_geometry.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nCubeVertices * nCubeAttribsPerVertex * sizeof(float), cubeTriangles, GL_STATIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, nCubeAttribsPerVertex * sizeof(float), (void *) 0);
	
	glBindVertexArray(0);
}

void CSkybox::sendUniforms(void) {
	glUseProgram(m_shaderProgram->program);

	glUniformMatrix4fv(m_shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.PVMMatrix));
	glUniform1f(m_shaderProgram->colorMultiplierLocation, m_colorMultiplier);
}

void CSkybox::draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	m_tempMats.MMatrix = glm::translate(glm::mat4(1.0f), m_position);
	m_tempMats.MMatrix = glm::scale(m_tempMats.MMatrix, m_scale);
	m_tempMats.MMatrix = glm::rotate(m_tempMats.MMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//tempMats.VMatrix = VMatrix;
	m_tempMats.PVMMatrix = PMatrix * VMatrix * m_tempMats.MMatrix;

	this->sendUniforms();

	glBindVertexArray(m_geometry.vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_geometry.numTriangles, GL_UNSIGNED_SHORT, 0);
	
	// reset
	glBindVertexArray(0);
	glFinish();
}