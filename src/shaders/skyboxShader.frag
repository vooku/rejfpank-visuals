#version 140

in vec3 positionTrans;

uniform mat4 Mmatrix;
uniform mat4 Vmatrix;

out vec4 color;

void main (void) {
	color = vec4 (positionTrans, 1.0f);
}