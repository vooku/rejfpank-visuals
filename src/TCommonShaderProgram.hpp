#ifndef _REJFPANK_TCOMMONSHADERPROGRAM
#define _REJFPANK_TCOMMONSHADERPROGRAM

//#include "data.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct TCommonShaderProgram {
	// identifier for the program
	GLuint program;          // = 0;
							 // vertex attributes
	GLint posLocation;       // = -1;
	GLint normalLocation;    // = -1;
	GLint texCoordsLocation;  // = -1;
							  // matrices
	GLint PVMmatrixLocation;    // = -1;
	GLint MmatrixLocation;
	GLint VmatrixLocation;
	GLint normalmatrixLocation; // = -1;  inverse transposed VMmatrix
	// material 
	GLint diffuseLocation;    // = -1;
	GLint ambientLocation;    // = -1;
	GLint specularLocation;   // = -1;
	GLint shininessLocation;  // = -1;
							  // texture 
	GLint texSamplerLocation; // = -1;
							  // light controls
	GLint fogLocation;
	GLint pointLightLocation;
	GLint directionalLightLocation;
	GLint spotLightLocation;
	// camera parametres
	GLint cameraDirectionLocation;
	// lolstuff
	GLint randVecLocation;
};

#endif // !_REJFPANK_TCOMMONSHADERPROGRAM
