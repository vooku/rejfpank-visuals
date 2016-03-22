#include "CSkybox.hpp"

using namespace std;

TCommonShaderProgram skyboxShaderProgram;

CSkybox::CSkybox (const glm::vec3 position, const glm::vec3 scale) :
	CDrawable (position, scale, nCubeTriangles) {
	
	glGenVertexArrays (1, &geometry.vertexArrayObject);
	glBindVertexArray (geometry.vertexArrayObject);
	
	glGenBuffers (1, &geometry.vertexBufferObject);
	glBindBuffer (GL_ARRAY_BUFFER, geometry.vertexBufferObject);
	glBufferData (GL_ARRAY_BUFFER, sizeof (cubeVertices), cubeVertices, GL_STATIC_DRAW);
	
	glGenBuffers (1, &geometry.elementBufferObject);
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, geometry.elementBufferObject);
	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (cubeTriangles), cubeTriangles, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray (skyboxShaderProgram.posLocation);
	glVertexAttribPointer (skyboxShaderProgram.posLocation, 3, GL_FLOAT, GL_FALSE, nCubeAttribsPerVertex * sizeof (float), (void *)(0));
	glEnableVertexAttribArray (skyboxShaderProgram.normalLocation);
	glVertexAttribPointer (skyboxShaderProgram.normalLocation, 3, GL_FLOAT, GL_FALSE, nCubeAttribsPerVertex * sizeof (float), (void *) (3 * sizeof (float)));
	
	glBindVertexArray (0);
}

void CSkybox::sendSkyboxUniforms (const glm::mat4 & PVMmatrix, const glm::mat4 & Vmatrix, const glm::mat4 & Mmatrix) {
	glUseProgram (skyboxShaderProgram.program);
	
	glUniformMatrix4fv (skyboxShaderProgram.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr (PVMmatrix));
	glUniformMatrix4fv (skyboxShaderProgram.VmatrixLocation, 1, GL_FALSE, glm::value_ptr (Vmatrix));
	glUniformMatrix4fv (skyboxShaderProgram.MmatrixLocation, 1, GL_FALSE, glm::value_ptr (Mmatrix));
}

void CSkybox::draw (const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix) {
	glm::mat4 Mmatrix = glm::translate (glm::mat4 (1.0f), position);
	//Mmatrix = Mmatrix * glm::toMat4 (rotQuat);
	Mmatrix = glm::scale (Mmatrix, scale);

	glm::mat4 PVMmatrix = Pmatrix * Vmatrix * Mmatrix;
	glm::mat4 normalmatrix = glm::inverse (glm::transpose (Mmatrix));

	this->sendSkyboxUniforms (PVMmatrix, Vmatrix, Mmatrix);
			
	glBindVertexArray (geometry.vertexArrayObject);
	glDrawElements (GL_TRIANGLES, geometry.numTriangles, GL_UNSIGNED_SHORT, 0);
		
	// reset
	glBindVertexArray (0);
	glFinish ();
}