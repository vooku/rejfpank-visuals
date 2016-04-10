#version 140

in vec2 texCoordsTrans;

uniform sampler2D texSampler;
uniform float alpha;

out vec4 color;

void main (void) {
	color = vec4(texCoordsTrans, 1.0f, alpha);
	//color = texture (texSampler, texCoordsTrans);
}