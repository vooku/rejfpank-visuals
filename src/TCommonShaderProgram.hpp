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
	//lights
	GLint whiteFlagLocation;
	GLint redFlagLocation;
	GLint blueFlagLocation;
	GLint pointFlagLocation;
	GLint cameraPositionLocation;
	// various
	GLint colorMultiplierLocation;
	GLint offsetLocation;
	// multipass effects
	GLint inverseLocation;
	GLint reducePaletteLocation;
	GLint deadPixLocation;
	GLint deadPixPLocation;
	GLint tearFlagLocation;
	GLint tearNLocation;
	GLint tearBordersLocation;
	GLint tearOffsetsLocation;
	GLint colorShiftLocation;
};

#endif // !_REJFPANK_TCOMMONSHADERPROGRAM
