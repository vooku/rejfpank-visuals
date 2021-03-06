#version 140

in vec3 position;

uniform mat4 PVMMatrix;

out vec3 positionTrans;

void main (void) {
	gl_Position = PVMMatrix * vec4 (position, 1.0f);
	positionTrans = position;
}