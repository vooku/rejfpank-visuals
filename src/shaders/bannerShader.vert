#version 140

in vec3 position;
in vec2 texCoords;

uniform mat4 PVMmatrix;

out vec2 texCoordsTrans;

void main (void) {
	gl_Position = PVMmatrix * vec4 (position, 1.0f);
	texCoordsTrans = texCoords;
}
