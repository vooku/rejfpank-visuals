#version 140

in vec3 positionTrans;

out vec4 color;

void main (void) {
	color = 0.2 * vec4 (positionTrans, 1.0f);
}