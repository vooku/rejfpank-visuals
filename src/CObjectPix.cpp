#include "CObjectPix.hpp"

CObjectPix::CObjectPix(const char * filename,
					   const glm::vec3 & position,
					   const glm::vec3 & scale,
					   TCommonShaderProgram * shaderProgram)
	: CDrawable(position, scale, shaderProgram) {

	m_enableDraw = this->loadImg(filename);
	if (!m_enableDraw) {
		std::cerr << "Error: Cannot load image: " << filename << std::endl;
		return;
	}
	std::cout << "loaded image: " << filename << std::endl;	

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
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, nCubeAttribsPerVertex * sizeof(float), (void *)0);

	glBindVertexArray(0);
}

bool CObjectPix::loadImg(const char * filename) {
	ILuint img_id;
	ilGenImages(1, &img_id);
	ilBindImage(img_id);

	if (ilLoadImage(filename) == IL_FALSE) {
		ilDeleteImages(1, &img_id);
		return false;
	}

	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT);

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	ILenum format = ilGetInteger(IL_IMAGE_FORMAT);

	unsigned int bytesPerPixel = ((format == IL_RGBA || format == IL_BGRA) ? 4 : 3);
	unsigned char * data = new unsigned char[width * height * bytesPerPixel];
	ilCopyPixels(0, 0, 0, width, height, 1, bytesPerPixel == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);

	// move the blocks so the center is in the middle (the - 0.5f is just magic)
	glm::vec3 offset = glm::vec3(-width / 2.0f - 0.5f, -height / 2.0f - 0.5f, 0.0f);

	std::vector<TPixel> pixels;
	for (int i = 0; i < width * height * bytesPerPixel; i += bytesPerPixel) {
		TPixel newPixel;
		newPixel.r = data[i + 0];
		newPixel.g = data[i + 1];
		newPixel.b = data[i + 2];
		if (bytesPerPixel == 4) newPixel.a = data[i + 3];
		pixels.push_back(newPixel);
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// if alpha == 0
			if (bytesPerPixel == 4 && pixels[i * width + j].a == 0) continue;

			TBlock newBlock;
			newBlock.color = (1 / 255.0f) * glm::vec3(pixels[i * width + j].r, pixels[i * width + j].g, pixels[i * width + j].b);
			newBlock.position = glm::vec3(width - j, height - i, 0.0f) + offset;
			newBlock.scale = glm::vec3(0.25f);
			m_blocks.push_back(newBlock);
		}

	}

	delete[] data;
	ilDeleteImages(1, &img_id);
	return true;
}

void CObjectPix::draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (!m_enableDraw) return;

	glBindVertexArray(m_geometry.vertexArrayObject);
	for (auto it : m_blocks) {
		m_tempMats.MMatrix = glm::translate(glm::mat4(1.0f), m_position);
		m_tempMats.MMatrix = glm::translate(m_tempMats.MMatrix, it.position);
		m_tempMats.MMatrix = glm::scale(m_tempMats.MMatrix, it.scale);
		//m_tempMats.MMatrix = glm::rotate(m_tempMats.MMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//tempMats.VMatrix = VMatrix;
		m_tempMats.PVMMatrix = PMatrix * VMatrix * m_tempMats.MMatrix;

		glUseProgram(m_shaderProgram->program);

		glUniformMatrix4fv(m_shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.PVMMatrix));
		glUniform1i(m_shaderProgram->texSamplerLocation, 0);
		glUniform3fv(m_shaderProgram->ambientLocation, 1, glm::value_ptr(it.color));
		glUniform1f(m_shaderProgram->alphaLocation, 1.0f);
		glUniform1i(m_shaderProgram->booleanFlagLocation, 0);

		glDrawElements(GL_TRIANGLES, m_geometry.numTriangles, GL_UNSIGNED_SHORT, 0);
	}

	// reset
	glBindVertexArray(0);
	glFinish();
}

void CObjectPix::sendUniforms(void) {
	
}
