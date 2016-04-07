#version 140

in vec3 positionTrans;
uniform float colorMultiplier;
out vec4 color;

void main (void) {
	color = colorMultiplier * vec4 (positionTrans, 1.0f);
}