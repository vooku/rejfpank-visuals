#version 140

in vec3 colorTrans;

out vec4 color;

void main (void) {
	color = vec4 (colorTrans, 1.0f);
}
