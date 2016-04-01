#version 140

in vec3 position;
in vec3 normal;
//in vec2 texCoords;

uniform mat4 PVMMatrix;

out vec3 positionTrans;
out vec3 normalTrans;
//out vec2 texCoordsTrans;

void main (void) {
	gl_Position = PVMMatrix * vec4 (position, 1.0f);
	positionTrans = position;
	normalTrans = normalize (normal);
	//texCoordsTrans = texCoords;
}