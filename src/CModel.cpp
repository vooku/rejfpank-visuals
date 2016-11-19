#include "CModel.hpp"
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

#include "pgr\pgr.hpp" // CHECK_GL_ERROR
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

CModel::CModel(const char * filename,
			   const glm::vec3 & position,
			   const glm::vec3 & scale,
			   TCommonShaderProgram * shaderProgram,
			   const char * texname,
			   const bool randomizeUV,
			   const CModel * dataObj,
			   const unsigned int materialIdx,
			   const float & alpha)
	: CDrawable(position, scale, shaderProgram, alpha),
	  m_dataObj(dataObj) {

	m_material.index = materialIdx;

	if (dataObj == NULL) {
		m_enableDraw = this->loadObj(filename, randomizeUV);
		if (!m_enableDraw) {
			std::cerr << "Error: Cannot load model: " << filename << std::endl;
			return;
		}

		if (texname != NULL) {
			m_geometry.texture = pgr::createTexture(texname, false);
			m_useTex = m_geometry.texture != 0;
			if (m_useTex) std::cout << "loaded texture: " << texname << std::endl;
			else std::cerr << "Error: Cannot load texture: " << texname << std::endl;
		}
		else m_useTex = false;

		m_dataObj = this;
		m_containsData = true;
		std::cout << "loaded model: " << filename << std::endl;
	}
	else {
		m_useTex = dataObj->m_useTex;
		if (m_useTex) m_geometry.texture = dataObj->m_geometry.texture;
		m_containsData = false;
		m_enableDraw = true;
	}

	this->setMaterials(filename);
}

/** Load mesh using assimp library
 * @param filename [in] file to open/load
 */
bool CModel::loadObj(const char * filename, const bool randomizeUV) {
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1); // normalize the model, makes scaling easier

	const aiScene * scene = importer.ReadFile(filename,
		aiProcess_Triangulate | // obvious
		aiProcess_GenSmoothNormals | // generate normals in vertices
		aiProcess_JoinIdenticalVertices | // obvious
		aiProcess_PreTransformVertices); // simplifies scene graph
	if (scene == NULL) {
		std::cerr << "Error: assimp: " << importer.GetErrorString() << std::endl;
		return false;
	}
	
	if (scene->mNumMeshes != 1) {
		std::cerr << "Error: This simplified loader can only process files with only one mesh." << std::endl;
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
		if (randomizeUV) {
			vertices[8 * i + 6] = rand() / (float)RAND_MAX;
			vertices[8 * i + 7] = rand() / (float)RAND_MAX;
		}
		else {
			vertices[8 * i + 6] = loadedMesh->HasTextureCoords(0) ? loadedMesh->mTextureCoords[0][i].x : 0;
			vertices[8 * i + 7] = loadedMesh->HasTextureCoords(0) ? loadedMesh->mTextureCoords[0][i].y : 0;
		}
	}
	// the indices are not stored in one array in the mesh but in their own containers
	unsigned int * indices = new unsigned int[loadedMesh->mNumFaces * 3];
	for (unsigned int i = 0; i < loadedMesh->mNumFaces; i++) {
		indices[3 * i + 0] = loadedMesh->mFaces[i].mIndices[0];
		indices[3 * i + 1] = loadedMesh->mFaces[i].mIndices[1];
		indices[3 * i + 2] = loadedMesh->mFaces[i].mIndices[2];
	}

	glGenVertexArrays(1, &m_geometry.vertexArrayObject);
	glBindVertexArray(m_geometry.vertexArrayObject);

	glGenBuffers(1, &m_geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, loadedMesh->mNumVertices * 8 * sizeof(float), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_geometry.elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_geometry.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedMesh->mNumFaces * 3 * sizeof (unsigned int), indices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(m_shaderProgram->posLocation);
	glVertexAttribPointer(m_shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(m_shaderProgram->normalLocation);
	glVertexAttribPointer(m_shaderProgram->normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(m_shaderProgram->texCoordsLocation);
	glVertexAttribPointer(m_shaderProgram->texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

	delete[] vertices;
	delete[] indices;

	m_geometry.numTriangles = loadedMesh->mNumFaces;

	CHECK_GL_ERROR();
	glBindVertexArray(0);
	return true;
}

void CModel::setMaterials(const char * filename) {
	glm::vec3 color;
	if (strstr(filename, "lego") != NULL)
		color = legoBrickColors[m_material.index % LEGO_BRICK_COLORS_COUNT];
	else if (m_useTex)
		color = glm::vec3(0.0f);
	else
		color = glm::vec3(0.7f);

	m_material.m_ambient = color * MATERIAL_GEN_AMBIENT_MULTI;
	m_material.m_diffuse = color;
	m_material.m_specular = MATERIAL_LEGO_SPECULAR;
	m_material.m_shininess = MATERIAL_LEGO_SHININES;
}

void CModel::setMaterial(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, const float & shininess, const float & alpha) {
	m_material.m_ambient = ambient;
	m_material.m_diffuse = diffuse;
	m_material.m_specular = specular;
	m_material.m_shininess = shininess;
	m_alpha = alpha;
}

void CModel::sendUniforms(void) {
	glUseProgram(m_shaderProgram->program);

	glUniformMatrix4fv(m_shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.PVMMatrix));
	glUniformMatrix4fv(m_shaderProgram->VMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.VMatrix));
	glUniformMatrix4fv(m_shaderProgram->MMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.MMatrix));
	glUniformMatrix4fv(m_shaderProgram->normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.normalMatrix));
	glUniform3fv(m_shaderProgram->ambientLocation, 1, glm::value_ptr(m_material.m_ambient));
	glUniform3fv(m_shaderProgram->diffuseLocation, 1, glm::value_ptr(m_material.m_diffuse));
	glUniform3fv(m_shaderProgram->specularLocation, 1, glm::value_ptr(m_material.m_specular));
	glUniform1f(m_shaderProgram->shininessLocation, m_material.m_shininess);
	glUniform1f(m_shaderProgram->alphaLocation, m_alpha);
	glUniform1f(m_shaderProgram->useTexLocation, m_useTex);
}

void CModel::draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (!m_enableDraw) return;

	m_tempMats.MMatrix = glm::translate(glm::mat4(1.0f), m_position);
	m_tempMats.MMatrix = glm::scale(m_tempMats.MMatrix, m_scale);
	m_tempMats.MMatrix = m_tempMats.MMatrix * m_pastRotMatrix * m_rotMatrix;
	m_tempMats.VMatrix = VMatrix;
	m_tempMats.PVMMatrix = PMatrix * VMatrix * m_tempMats.MMatrix;
	m_tempMats.normalMatrix = glm::transpose(glm::inverse(m_tempMats.MMatrix));

	if (m_useTex) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_geometry.texture);
	}
	
	this->sendUniforms();
	
	glBindVertexArray(m_dataObj->m_geometry.vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_dataObj->m_geometry.numTriangles * 3, GL_UNSIGNED_INT, 0);

	CHECK_GL_ERROR();
	
	// reset
	glBindVertexArray(0);
	glFinish();
}