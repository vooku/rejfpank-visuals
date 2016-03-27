#include "CLoadedObj.hpp"
#include <fstream>
#include <string>
#include <sstream>

#include "pgr\Shader.hpp" // GL_CHECK_ERROR
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

using namespace std;

/** Load mesh using assimp library
 * @param filename [in] file to open/load
 * @param shader [in] vao will connect loaded data to shader
 */
bool CLoadedObj::loadSingleMesh(const string &fileName, TCommonShaderProgram * shaderProgram) {
	Assimp::Importer importer;

	importer.SetPropertyInteger(AI_CONFIG_PP_PTV_NORMALIZE, 1); // Unitize object in size (scale the model to fit into (-1..1)^3)
																// Load asset from the file - you can play with various processing steps
	const aiScene * scn = importer.ReadFile(fileName.c_str(), 0
		| aiProcess_Triangulate             // Triangulate polygons (if any).
		| aiProcess_PreTransformVertices    // Transforms scene hierarchy into one root with geometry-leafs only. For more see Doc.
		| aiProcess_GenSmoothNormals        // Calculate normals per vertex.
		| aiProcess_JoinIdenticalVertices);
	// abort if the loader fails
	if (scn == NULL) {
		cerr << "Error: assimp: " << importer.GetErrorString() << endl;
		return false;
	}
	// some formats store whole scene (multiple meshes and materials, lights, cameras, ...) in one file, we cannot handle that in our simplified example
	if (scn->mNumMeshes != 1) {
		cerr << "Error: This simplified loader can only process files with only one mesh." << endl;
		return false;
	}
	// in this phase we know we have one mesh in our loaded scene, we can directly copy its data to opengl...
	const aiMesh * mesh = scn->mMeshes[0];

	// vertex buffer object, store all vertex positions and normals
	glGenBuffers(1, &(geometry.vertexBufferObject));
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferObject);
	// allocate memory for vertices, normals, and texture coordinates
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * mesh->mNumVertices, 0, GL_STATIC_DRAW);
	// first store all vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(float) * mesh->mNumVertices, mesh->mVertices);
	// then store all normals
	glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float)*mesh->mNumVertices, 3 * sizeof(float) * mesh->mNumVertices, mesh->mNormals);

	// just texture 0 for now
	float *textureCoords = new float[2 * mesh->mNumVertices];  // 2 floats per vertex
	float *currentTextureCoord = textureCoords;

	// copy texture coordinates
	aiVector3D vect;

	if (mesh->HasTextureCoords(0)) {
		// we use 2D textures with 2 coordinates and ignore the third coordinate
		for (unsigned int idx = 0; idx<mesh->mNumVertices; idx++) {
			vect = (mesh->mTextureCoords[0])[idx];
			*currentTextureCoord++ = vect.x;
			*currentTextureCoord++ = vect.y;
		}
	}

	// finally store all texture coordinates
	glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float)*mesh->mNumVertices, 2 * sizeof(float) * mesh->mNumVertices, textureCoords);

	// copy all mesh faces into one big array (assimp supports faces with ordinary number of vertices, we use only 3 -> triangles)
	unsigned int *indices = new unsigned int[mesh->mNumFaces * 3];
	for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
		indices[f * 3 + 0] = mesh->mFaces[f].mIndices[0];
		indices[f * 3 + 1] = mesh->mFaces[f].mIndices[1];
		indices[f * 3 + 2] = mesh->mFaces[f].mIndices[2];
	}
	// copy our temporary index array to OpenGL and free the array
	glGenBuffers(1, &(geometry.elementBufferObject));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned) * mesh->mNumFaces, indices, GL_STATIC_DRAW);

	delete[] indices;

	// copy the material info to MeshGeometry structure
	const aiMaterial *mat = scn->mMaterials[mesh->mMaterialIndex];
	aiColor3D color;
	aiString name;

	// Get returns: aiReturn_SUCCESS 0 | aiReturn_FAILURE -1 | aiReturn_OUTOFMEMORY -3
	mat->Get(AI_MATKEY_NAME, name); // may be "" after the input mesh processing. Must be aiString type!

	mat->Get<aiColor3D>(AI_MATKEY_COLOR_DIFFUSE, color);
	geometry.diffuse = glm::vec3(color.r, color.g, color.b);

	// mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	mat->Get<aiColor3D>(AI_MATKEY_COLOR_AMBIENT, color);
	geometry.ambient = glm::vec3(color.r, color.g, color.b);

	mat->Get<aiColor3D>(AI_MATKEY_COLOR_SPECULAR, color);
	geometry.specular = glm::vec3(color.r, color.g, color.b);

	float shininess;

	mat->Get<float>(AI_MATKEY_SHININESS, shininess);
	geometry.shininess = shininess / 4.0f;  // shininess divisor-not descibed anywhere

	geometry.texture = 0;

	// load texture image
	if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		// get texture name 
		mat->Get<aiString>(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), name);
		string textureName = name.data;

		size_t found = fileName.find_last_of("/\\");
		// insert correct texture file path 
		if (found != string::npos) { // not found
										  //subMesh_p->textureName.insert(0, "/");
			textureName.insert(0, fileName.substr(0, found + 1));
		}

		cout << "Loading texture file: " << textureName << endl;
		//(*geometry)->texture = pgr::createTexture(textureName);
	}
	CHECK_GL_ERROR();

	glGenVertexArrays(1, &(geometry.vertexArrayObject));
	glBindVertexArray(geometry.vertexArrayObject);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.elementBufferObject); // bind our element array buffer (indices) to vao
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBufferObject);

	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
	glEnableVertexAttribArray(shaderProgram->normalLocation);
	glVertexAttribPointer(shaderProgram->normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float) * mesh->mNumVertices));
	
	glEnableVertexAttribArray(shaderProgram->texCoordsLocation);
	glVertexAttribPointer(shaderProgram->texCoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * sizeof(float) * mesh->mNumVertices));
	CHECK_GL_ERROR();

	glBindVertexArray(0);

	geometry.numTriangles = mesh->mNumFaces;

	return true;
}

CLoadedObj::CLoadedObj(const char* filename, const glm::vec3 position, const glm::vec3 scale, TCommonShaderProgram * shaderProgram)
	: loaded (true),
	  CDrawable (position, scale, shaderProgram) {
	if (!this->loadSingleMesh(filename, shaderProgram)) {
		cerr << "Error: Cannot load " << filename << "!" << endl;
		loaded = false;
		return;
	}
	cout << "loaded file: " << filename << endl;
}

void CLoadedObj::sendUniforms(void) {
	glUseProgram(shaderProgram->program);

	glUniformMatrix4fv(shaderProgram->PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(tempMats.PVMmatrix));
}

void CLoadedObj::draw(const glm::mat4 & Pmatrix, const glm::mat4 & Vmatrix) {
	if (!loaded) return;

	tempMats.Mmatrix = glm::translate(glm::mat4(1.0f), position);
	tempMats.Mmatrix = glm::scale(tempMats.Mmatrix, scale);
	////Mmatrix = Mmatrix * glm::toMat4 (rotQuat);
	//tempMats.Vmatrix = Vmatrix; // not necessary
	//tempMats.Pmatrix = Pmatrix; // not necessary
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