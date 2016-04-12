#version 140

in vec2 texCoordsTrans;

uniform sampler2D texSampler;
uniform vec3 color;
uniform float alpha;
uniform int useTex;

out vec4 colorOut;

void main (void) {
	if (useTex == 1) {
		colorOut = texture (texSampler, texCoordsTrans);
		colorOut.w = alpha;
	}
	else colorOut = vec4(color, alpha);
	
}