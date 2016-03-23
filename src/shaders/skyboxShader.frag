#version 140

in vec4 positionTrans;

out vec4 color;

void main (void) {
	color = positionTrans;
}