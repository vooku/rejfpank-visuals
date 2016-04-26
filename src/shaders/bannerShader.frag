#version 140

in vec2 texCoordsTrans;

uniform sampler2D texSampler;
uniform vec3 color;
uniform float alpha;
uniform bool useTex;
uniform bool inverse;
uniform bool reducePalette;
uniform bool tearFlag;
uniform int tearN; // actual number of tear borders
uniform float tearBorders[10]; // enough space for max 10
uniform float tearOffsets[11]; // enough space for max 11

out vec4 colorOut;

vec2 getTear(void) {
	vec2 texCoordsOut = texCoordsTrans;

	if (texCoordsOut.y <= tearBorders[0]) texCoordsOut.x += tearOffsets[0];
	else if (texCoordsOut.y > tearBorders[tearN]) texCoordsOut.x += tearOffsets[tearN + 1];
	else for (int i = 0; i < tearN - 1; i++)
		if (texCoordsOut.y > tearBorders[i] && texCoordsOut.y <= tearBorders[i + 1])
			texCoordsOut.x += tearOffsets[i];
	if (texCoordsOut.x > 1.0f) texCoordsOut.x -= 1.0f;
	
	return texCoordsOut;
}

vec4 getInverse(vec4 color) {
	return vec4(1.0f) - color;
}

vec4 getReducePalette(vec4 color) {
	return round(color);;
}

void main(void) {
	vec4 colorTemp;

	if (useTex) {
		if (tearFlag) colorTemp = texture (texSampler, getTear());
		else colorTemp = texture (texSampler, texCoordsTrans);
	}
	else colorTemp.rgb = color;
	
	if (inverse) colorTemp = getInverse(colorTemp);
	if (reducePalette) colorTemp = getReducePalette(colorTemp);
	
	colorTemp.a = alpha;
	colorOut = colorTemp;
}