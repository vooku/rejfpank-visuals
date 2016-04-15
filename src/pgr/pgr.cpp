#include <stdio.h>
#include <iostream>

#include "pgr.hpp"

namespace pgr {

void checkGLError(const char *where, int line) {
  GLenum err = glGetError();
  if(err == GL_NONE)
    return;

  std::string errString = "<unknown>";
  switch(err) {
    case GL_INVALID_ENUM:
      errString = "GL_INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      errString = "GL_INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      errString = "GL_INVALID_OPERATION";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      errString = "GL_INVALID_FRAMEBUFFER_OPERATION";
      break;
    case GL_OUT_OF_MEMORY:
      errString = "GL_OUT_OF_MEMORY";
      break;
    default:;
  }
  if(where == 0 || *where == 0)
    std::cerr << "GL error occurred: " << errString << std::endl;
  else
    std::cerr << "GL error occurred in " << where << ":" << line << ": " << errString << std::endl;
}

GLuint createShaderFromSource(GLenum eShaderType, const std::string &strShaderText) {
  GLuint shader = glCreateShader(eShaderType);
  const char *strFileData = strShaderText.c_str();
  glShaderSource(shader, 1, &strFileData, NULL);

  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

    const char *strShaderType = NULL;
    switch(eShaderType) {
      case GL_VERTEX_SHADER: strShaderType   = "vertex";   break;
      case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
      case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
    }

    std::cerr << "Compile failure in " << strShaderType << " shader:" << std::endl;
    std::cerr << strInfoLog;

    delete[] strInfoLog;
    glDeleteShader(shader);
    return 0;
  }

  CHECK_GL_ERROR();
  return shader;
}

GLuint createShaderFromFile(GLenum eShaderType, const std::string &filename) {
  FILE *f = fopen(("../" + filename).c_str(), "rb");
  if (!f) {
    std::cerr << "Unable to open file " << filename << " for reading" << std::endl;
    return 0;
  }

  fseek(f, 0, SEEK_END);
  int length = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buffer = new char[length + 1];
  int length2 = fread(buffer, 1, length, f);
  fclose(f);
  buffer[length] = '\0';

  GLuint sh = createShaderFromSource(eShaderType, buffer);
  delete [] buffer;

  std::cout << "loaded shader: " << filename << std::endl;
  return sh;
}

static bool linkProgram(GLuint program) {
  glLinkProgram(program);

  GLint status = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  CHECK_GL_ERROR();

  if(status == GL_FALSE) {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
    fprintf(stderr, "Linker failure: %s\n", strInfoLog);
    delete[] strInfoLog;
    return false;
  }

  return true;
}

GLuint createProgram(const GLuint * shaders) {
  GLuint program = glCreateProgram();

  while(shaders && *shaders)
    glAttachShader(program, *shaders++);
  CHECK_GL_ERROR();

  if(!linkProgram(program)) {
    deleteProgramAndShaders(program);
    return 0;
  }

  return program;
}

GLuint createProgram(const std::vector<GLuint> &shaderList) {
  GLuint program = glCreateProgram();

  for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    glAttachShader(program, shaderList[iLoop]);
  CHECK_GL_ERROR();

  if(!linkProgram(program)) {
    deleteProgramAndShaders(program);
    return 0;
  }

  return program;
}

void deleteProgramAndShaders(GLuint program) {
  const GLsizei MAX_COUNT = 10;  // Pisvejc's constant
  GLsizei count = 0;
  GLuint shaders[MAX_COUNT];

  if(program != 0) {
    glGetAttachedShaders( program, MAX_COUNT, &count, shaders );
    for( int i=0; i<count; i++)
      glDeleteShader( shaders[i] );

    glDeleteProgram( program );
  }
}

bool loadTexImage2D(const std::string & fileName, GLenum target) {
	// DevIL library has to be initialized (ilInit() must be called)

	// DevIL uses mechanism similar to OpenGL, each image has its ID (name)
	ILuint img_id;
	ilGenImages(1, &img_id); // generate one image ID (name)
	ilBindImage(img_id); // bind that generated id

	// set origin to LOWER LEFT corner (the orientation which OpenGL uses)
	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

	// this will load image data to the currently bound image
	if (ilLoadImage(fileName.c_str()) == IL_FALSE) {
		ilDeleteImages(1, &img_id);
		std::cerr << __FUNCTION__ << " cannot load image " << fileName << std::endl;
		return false;
	}

	// if the image was correctly loaded, we can obtain some informatins about our image
	ILint width = ilGetInteger(IL_IMAGE_WIDTH);
	ILint height = ilGetInteger(IL_IMAGE_HEIGHT);
	ILenum format = ilGetInteger(IL_IMAGE_FORMAT);
	// there are many possible image formats and data types
	// we will convert all image types to RGB or RGBA format, with one byte per channel
	unsigned Bpp = ((format == IL_RGBA || format == IL_BGRA) ? 4 : 3);
	char * data = new char[width * height * Bpp];
	// this will convert image to RGB or RGBA, one byte per channel and store data to our array
	ilCopyPixels(0, 0, 0, width, height, 1, Bpp == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);
	// image data has been copied, we dont need the DevIL object anymore
	ilDeleteImages(1, &img_id);

	// bogus ATI drivers may require this call to work with mipmaps
	//glEnable(GL_TEXTURE_2D);

	glTexImage2D(target, 0, Bpp == 4 ? GL_RGBA : GL_RGB, width, height, 0, Bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

	// free our data (they were copied to OpenGL)
	delete[] data;
	return true;
}

GLuint createTexture(const std::string &fileName, bool mipmap) {
	// generate and bind one texture
	GLuint tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// set linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// upload our image data to OpenGL
	if (!loadTexImage2D(fileName, GL_TEXTURE_2D)) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &tex);
		return 0;
	}
	// create mipmaps
	if (mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
	// unbind the texture (just in case someone will mess up with texture calls later)
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();
	return tex;
}

} // end namespace pgr
