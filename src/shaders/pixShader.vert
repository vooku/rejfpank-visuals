#version 140

in vec3 position;
in vec3 color;
in vec3 offset;

uniform mat4 PVMMatrix;

out vec3 colorTrans;

void main (void) {
	gl_Position = PVMMatrix * vec4 (position + offset, 1.0f);
	colorTrans = color;
}
