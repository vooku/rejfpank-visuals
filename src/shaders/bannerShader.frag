#version 140

in vec2 texCoordsTrans;

uniform sampler2D texSampler;

out vec4 color;

void main (void) {
	//color = vec4(texCoordsTrans, 1.0f, 1.0f);
	color = texture (texSampler, texCoordsTrans);
}