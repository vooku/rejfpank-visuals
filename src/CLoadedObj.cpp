#include "CLoadedObj.hpp"
#include <fstream>
#include <string>
#include <sstream>

#include "pgr\Shader.hpp"

using namespace std;

CLoadedObj::CLoadedObj(const char* filename, const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram)
	: loaded (true),
	  CDrawable (position, scale, shaderProgram) {

	ifstream in(filename, ios::in);
	if (!in) {
		cerr << "Error: Cannot open " << filename << "!" << endl;
		loaded = false;
		return;
	}

	string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ") {
			istringstream s(line.substr(2));
			glm::vec3 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			geometry.vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "f ") {
			istringstream s(line.substr(2));
			GLushort a, b, c;
			s >> a;
			s >> b;
			s >> c;
			a--;
			b--;
			c--;
			geometry.elements.push_back(a);
			geometry.elements.push_back(b);
			geometry.elements.push_back(c);
		}
		else if (line[0] == '#') {
			// ignoring this line
		}
		else {
			// ignoring this line 
		}
	}

	geometry.normals.resize(geometry.vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (unsigned int i = 0; i < geometry.elements.size(); i += 3) {
		GLushort ia = geometry.elements[i];
		GLushort ib = geometry.elements[i + 1];
		GLushort ic = geometry.elements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(geometry.vertices[ib]) - glm::vec3(geometry.vertices[ia]),
			glm::vec3(geometry.vertices[ic]) - glm::vec3(geometry.vertices[ia])));
		geometry.normals[ia] = geometry.normals[ib] = geometry.normals[ic] = normal;
	}

	glGenVertexArrays(1, &geometry.vertexArrayObject);
	glBindVertexArray(geometry.vertexArrayObject);

	glGenBuffers(1, &geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 2 * geometry.vertices.size() * sizeof(glm::vec3), &(geometry.vertices[0]), GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(glm::vec3), geometry.vertices.size() * sizeof(glm::vec3), &(geometry.normals[0]));

	glGenBuffers(1, &geometry.elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.elements.size() * sizeof(GLushort), &(geometry.elements[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)(0));
	glEnableVertexAttribArray(shaderProgram->normalLocation);
	glVertexAttribPointer(shaderProgram->normalLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void *) (geometry.vertices.size() * sizeof(glm::vec3)));

	glBindVertexArray(0);
	
	cout << "loaded file: " << filename << endl;
}

void CLoadedObj::sendCLoadedObjUniforms(const glm::mat4 & PVMmatrix, const glm::mat4 & Vmatrix, const glm::mat4 & Mmatrix) {
	glUseProgram(shaderProgram->program);

	glUniformMatrix4fv(shaderProgram->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVMmatrix));
	glUniformMatrix4fv(shaderProgram->VmatrixLocation, 1, GL_FALSE, glm::value_ptr(Vmatrix));
	glUniformMatrix4fv(shaderProgram->MmatrixLocation, 1, GL_FALSE, glm::value_ptr(Mmatrix));
}

void CLoadedObj::draw(const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix) {
	if (!loaded) return;

	glm::mat4 Mmatrix = glm::translate(glm::mat4(1.0f), position);
	//Mmatrix = Mmatrix * glm::toMat4(rotQuat);
	Mmatrix = glm::scale(Mmatrix, scale);

	glm::mat4 PVMmatrix = Pmatrix * Vmatrix * Mmatrix;
	glm::mat4 normalmatrix = glm::transpose(glm::inverse(Mmatrix));

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, geometry.texture);

	this->sendCLoadedObjUniforms(PVMmatrix, Vmatrix, Mmatrix);
	//this->sendCLoadedObjUniforms(PVMmatrix, Vmatrix, Mmatrix, normalmatrix);

	glBindVertexArray(geometry.vertexArrayObject);
	glDrawElements(GL_TRIANGLES, geometry.vertices.size() * 3, GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
	// reset
	glBindVertexArray(0);
	glFinish();
}