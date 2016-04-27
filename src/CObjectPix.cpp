#include "CObjectPix.hpp"

CObjectPix::CObjectPix(const char * filename,
					   const glm::vec3 & position,
					   const glm::vec3 & scale,
					   TCommonShaderProgram * shaderProgram,
					   const float & ptSize)
	: CDrawable(position, scale, shaderProgram),
	  m_ptSize(ptSize) {

	m_enableDraw = this->loadImg(filename);
	if (!m_enableDraw) {
		std::cerr << "Error: Cannot load image: " << filename << std::endl;
		return;
	}
	std::cout << "loaded image: " << filename << std::endl;	

	float * pixelData = new float[m_blocks.size() * 9];
	for (size_t i = 0; i < m_blocks.size(); i++) {
		pixelData[i * 9 + 0] = m_blocks[i].position.x;
		pixelData[i * 9 + 1] = m_blocks[i].position.y;
		pixelData[i * 9 + 2] = m_blocks[i].position.z;
		pixelData[i * 9 + 3] = m_blocks[i].color.x;
		pixelData[i * 9 + 4] = m_blocks[i].color.y;
		pixelData[i * 9 + 5] = m_blocks[i].color.z;
		pixelData[i * 9 + 6] = 0.0f;
		pixelData[i * 9 + 7] = 0.0f;
		pixelData[i * 9 + 8] = 0.0f;
	}

	glGenVertexArrays(1, &m_geometry.vertexArrayObject);
	glBindVertexArray(m_geometry.vertexArrayObject);

	glGenBuffers(1, &m_geometry.vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_geometry.vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, m_blocks.size() * 9 * sizeof(float), pixelData, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(shaderProgram->posLocation);
	glVertexAttribPointer(shaderProgram->posLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(shaderProgram->ambientLocation);
	glVertexAttribPointer(shaderProgram->ambientLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(shaderProgram->offsetLocation);
	glVertexAttribPointer(shaderProgram->offsetLocation, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));

	glBindVertexArray(0);
	delete[] pixelData;
}

void CObjectPix::updatePtSize(const double time) {
	double elapsedTime = time - m_triggerTime;
	float newSize = -(1.0f / (float)BEAT_LENGTH(175)) * (float)elapsedTime + 7.0f;
	m_ptSize = newSize > 0 ? newSize : 1.0f;
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

	ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
	ILenum format = ilGetInteger(IL_IMAGE_FORMAT);
	float larger = (float)(width >= height ? width : height); // float bc of precise division

	unsigned int bytesPerPixel = ((format == IL_RGBA || format == IL_BGRA) ? 4 : 3);
	unsigned char * data = new unsigned char[width * height * bytesPerPixel];
	ilCopyPixels(0, 0, 0, width, height, 1, bytesPerPixel == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);

	// move the blocks so the center is in the middle (the - 0.5f is just magic)
	glm::vec3 offset = glm::vec3((-width / 2.0f - 0.5f) / larger, (-height / 2.0f - 0.5f) / larger, 0.0f);

	std::vector<TPixel> pixels;
	for (unsigned int i = 0; i < width * height * bytesPerPixel; i += bytesPerPixel) {
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
			newBlock.position = glm::vec3((width - j) / larger, (height - i) / larger, 0.0f) + offset;
			m_blocks.push_back(newBlock);
		}
	}
	
	delete[] data;
	ilDeleteImages(1, &img_id);
	return true;
}

void CObjectPix::draw(const glm::mat4 & PMatrix, const glm::mat4 & VMatrix) {
	if (!m_enableDraw) return;

	glPointSize(m_ptSize);
	glBindVertexArray(m_geometry.vertexArrayObject);

	m_tempMats.MMatrix = glm::translate(glm::mat4(1.0f), m_position);
	m_tempMats.MMatrix = glm::scale(m_tempMats.MMatrix, m_scale);
	//m_tempMats.MMatrix = glm::rotate(m_tempMats.MMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//tempMats.VMatrix = VMatrix;
	m_tempMats.PVMMatrix = PMatrix * VMatrix * m_tempMats.MMatrix;

	this->sendUniforms();

	glDrawArrays(GL_POINTS, 0, m_blocks.size());

	// reset
	glBindVertexArray(0);
	glFinish();
}
/*
void CObjectPix::offsetPix(void) {
	if (!m_offsets.empty() || !m_offsetsIndices.empty()) this->deoffsetPix();
	
	float newOffset [3];
	
	glBindVertexArray(m_geometry.vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_geometry.vertexBufferObject);
	
	for (int i = 0; i < rand() % 100; i++) {
		m_offsetsIndices.push_back(rand() % m_blocks.size());
	
		newOffset[0] = ((rand() - (RAND_MAX / 2)) % RAND_MAX);
		newOffset[1] = ((rand() - (RAND_MAX / 2)) % RAND_MAX);
		newOffset[2] = ((rand() - (RAND_MAX / 2)) % RAND_MAX);
		
		glBufferSubData(GL_ARRAY_BUFFER, (m_offsetsIndices.back() * 9 + 6) * sizeof(float), 3 * sizeof(float), newOffset);
		m_offsets.push_back(glm::vec3(newOffset[0], newOffset[1], newOffset[2]));
	}
	
	glBindVertexArray(0);
}

void CObjectPix::deoffsetPix(void) {
	if (m_offsets.empty() && m_offsetsIndices.empty()) return;

	float newOffset[3];
	newOffset[0] = newOffset[1] = newOffset[2] = 0.0f;

	glBindVertexArray(m_geometry.vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_geometry.vertexBufferObject);

	for (auto it : m_offsetsIndices)
		glBufferSubData(GL_ARRAY_BUFFER, (it * 9 + 6) * sizeof(float), 3 * sizeof(float), newOffset);
	
	glBindVertexArray(0);

	m_offsets.clear();
	m_offsetsIndices.clear();
}
*/
void CObjectPix::sendUniforms(void) {
	glUseProgram(m_shaderProgram->program);

	glUniformMatrix4fv(m_shaderProgram->PVMMatrixLocation, 1, GL_FALSE, glm::value_ptr(m_tempMats.PVMMatrix));
}
