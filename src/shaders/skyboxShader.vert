#version 140

in vec3 position;

uniform mat4 PVMmatrix;

out vec4 positionTrans;

void main (void) {
	positionTrans = PVMmatrix * vec4(position, 1.0f);
	gl_Position = positionTrans;
}