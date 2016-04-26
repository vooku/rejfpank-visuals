/**
* @author	Vadim Petrov
* @date	2015
*/

#ifndef _REJFPANK_TCOMMONSHADERPROGRAM
#define _REJFPANK_TCOMMONSHADERPROGRAM

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
	GLint normalMatrixLocation; // inverse transposed VMatrix
	// material 
	GLint ambientLocation;
	GLint diffuseLocation;
	GLint specularLocation;
	GLint shininessLocation;
	GLint alphaLocation;
	GLint useTexLocation;
	GLint texSamplerLocation;
	// various
	GLint colorMultiplierLocation;
	GLint offsetLocation;
	GLint fadeToBlackLocation;
	// multipass effects
	GLint inverseLocation;
	GLint reducePaletteLocation;
	GLint tearFlagLocation;
	GLint tearNLocation;
	GLint tearBordersLocation;
	GLint tearOffsetsLocation;
};

#endif // !_REJFPANK_TCOMMONSHADERPROGRAM
