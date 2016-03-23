#version 140

in vec3 position;

uniform mat4 PVMmatrix;

out vec3 positionTrans;

void main (void) {
	gl_Position = PVMmatrix * vec4 (position, 1.0f);
	positionTrans = position;
}