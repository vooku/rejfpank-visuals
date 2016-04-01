#ifndef _REJFPANK_TCOMMONSHADERPROGRAM
#define _REJFPANK_TCOMMONSHADERPROGRAM

//#include "data.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct TCommonShaderProgram {
	GLuint program;
	// vertex attributes
	GLint posLocation;
	GLint normalLocation;
	GLint texCoordsLocation;
	// matrices
	GLint PVMMatrixLocation;
	GLint MMatrixLocation;
	GLint VMatrixLocation;
	GLint normalMatrixLocation; //  inverse transposed VMatrix
	// material 
	GLint diffuseLocation;
	GLint ambientLocation;
	GLint specularLocation;
	GLint shininessLocation;

	GLint texSamplerLocation;
};

#endif // !_REJFPANK_TCOMMONSHADERPROGRAM
