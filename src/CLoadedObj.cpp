#include "CLoadedObj.hpp"
#include <fstream>
#include <string>
#include <sstream>

#include "pgr\Shader.hpp" // CHECK_GL_ERROR
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

using namespace std;

CLoadedObj::CLoadedObj(const char* filename, const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram)
	: loaded (true),
	  CDrawable (position, scale, shaderProgram) {
	if (!this->loadObj(filename)) {
		cerr << "Error: Cannot load " << filename << "!" << endl;
		loaded = false;
		return;
	}
	cout << "loaded file: " << filename << endl;

	if (filename == MODEL_LEGO) {
		glm::vec3 color = legoBrickColors[rand() % 4];
		
		material.ambient = color * 0.1f;
		material.diffuse = color;
		material.specular = glm::vec3 (0.3f);
		material.shininess = 3.0f;
	}
}

/** Load mesh using assimp library
 * @param filename [in] file to open/load
 */
bool CLoadedObj::loadObj(const char * filename) {
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1); // normalize the model, makes scaling easier

	const aiScene * scene = importer.ReadFile(filename,
		aiProcess_Triangulate | // obvious
		aiProcess_GenSmoothNormals | // generate normals in vertices
		aiProcess_JoinIdenticalVertices | // obvious
		aiProcess_PreTransformVertices); // simplifies scene graph
	if (scene == NULL) {
		cerr << "Error: assimp: " << importer.GetErrorString() << endl;
		return false;
	}
	
	if (scene->mNumMeshes != 1) {
		cerr << "Error: This simplified loader can only process files with only one mesh." << endl;
		return false;
	}

	// process the data
	const aiMesh * loadedMesh = scene->mMeshes[0];

	float * vertices;
	vertices = new float[loadedMesh->mNumVertices * 8];
	for (unsigned int i = 0; i < loadedMesh->mNumVertices; i++) {
		vertices[8 * i + 0] = loadedMesh->mVertices[i].x;
		vertices[8 * i + 1] = loadedMesh->mVertices[i].y;
		vertices[8 * i + 2] = loadedMesh->mVertices[i].z;
		vertices[8 * i + 3] = loadedMesh->mNormals[i].x;
		vertices[8 * i + 4] = loadedMesh->mNormals[i].y;
		vertices[8 * i + 5] = loadedMesh->mNormals[i].z;
		vertices[8 * i + 6] = loadedMesh->HasTextureCoords(0) ? loadedMesh->mTextureCoords[0][i].x : 0;
		vertices[8 * i + 7] = loadedMesh->HasTextureCoords(0) ? loadedMesh->mTextureCoords[0][i].y : 0;
	}
	// the indices are not stored in one array in the mesh but in their own containers
	unsigned int * indices = new unsigned int[loadedMesh->mNumFaces * 3];
	for (unsigned int i = 0; i < loadedMesh->mNumFaces; i++) {
		indices[3 * i + 0] = loadedMesh->mFaces[i].mIndices[0];
		indices[3 * i + 1] = loadedMesh->mFaces[i].mIndices[1];
		indices[3 * i + 2] = loadedMesh->mFaces[i].mIndices[2];
	}

	glGenVertexArrays(1, &geometry.vertexArrayObject);
	glBindVertexArray(geometry.vertexArrayObject);

	glGenBuffers(1, &geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, loadedMesh->mNumVertices * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &geometry.elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedMesh->mNumFaces * 3 * sizeof (unsigned int), indices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(shaderProgram->normalLocation);
	glVertexAttribPointer(shaderProgram->normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));

	delete[] vertices;
	delete[] indices;

	geometry.numTriangles = loadedMesh->mNumFaces;

	CHECK_GL_ERROR();
	glBindVertexArray(0);
	return true;
}

void CLoadedObj::sendUniforms(void) {
	glUseProgram(shaderProgram->program);

	glUniformMatrix4fv(shaderProgram->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.PVMmatrix));
	glUniformMatrix4fv(shaderProgram->VmatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.Vmatrix));
	glUniformMatrix4fv(shaderProgram->MmatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.Mmatrix));
	glUniformMatrix4fv(shaderProgram->normalmatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.normalmatrix));
	glUniform3fv(shaderProgram->ambientLocation, 1, glm::value_ptr(material.ambient));
	glUniform3fv(shaderProgram->diffuseLocation, 1, glm::value_ptr(material.diffuse));
	glUniform3fv(shaderProgram->specularLocation, 1, glm::value_ptr(material.specular));
	glUniform1f(shaderProgram->shininessLocation, material.shininess);
}

void CLoadedObj::draw(const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix) {
	if (!loaded) return;

	tempMats.Mmatrix = glm::translate(glm::mat4(1.0f), position);
	tempMats.Mmatrix = glm::scale(tempMats.Mmatrix, scale);
	//Mmatrix = Mmatrix * glm::toMat4 (rotQuat);
	tempMats.Vmatrix = Vmatrix;
	tempMats.PVMmatrix = Pmatrix * Vmatrix * tempMats.Mmatrix;
	tempMats.normalmatrix = glm::transpose(glm::inverse(tempMats.Mmatrix));

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, geometry.texture);

	this->sendUniforms();

	glBindVertexArray(geometry.vertexArrayObject);
	glDrawElements(GL_TRIANGLES, geometry.numTriangles * 3, GL_UNSIGNED_INT, 0);

	CHECK_GL_ERROR();
	
	// reset
	glBindVertexArray(0);
	glFinish();
}