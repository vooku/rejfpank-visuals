#version 140

in vec3 position;
in vec2 texCoords;

uniform mat4 PVMMatrix;

out vec2 texCoordsTrans;

void main (void) {
	gl_Position = PVMMatrix * vec4 (position, 1.0f);
	texCoordsTrans = texCoords;
}
