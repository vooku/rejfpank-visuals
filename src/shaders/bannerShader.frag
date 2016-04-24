#version 140

in vec2 texCoordsTrans;

uniform sampler2D texSampler;
uniform vec3 color;
uniform float alpha;
uniform bool useTex;
uniform bool inverse;

out vec4 colorOut;

void main (void) {
	vec4 colorTemp;

	if (useTex) colorTemp = texture (texSampler, texCoordsTrans);
	else colorTemp = vec4(color, 1.0f);
	
	if (inverse) colorTemp = vec4(1.0f) - colorTemp;

	colorTemp.w = alpha;
	colorOut = colorTemp;
}