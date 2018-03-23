#version 140

in vec2 texCoordsTrans;

uniform sampler2D texSampler;
uniform vec3 color;
uniform float alpha;
uniform bool useTex;
uniform bool inverse;
uniform bool reducePalette;
uniform bool deadPix;
uniform float deadPixP;
uniform bool tearFlag;
uniform int tearN; // actual number of tear borders
uniform float tearBorders[10]; // enough space for max 10
uniform float tearOffsets[11]; // enough space for max 11
uniform int colorShift;

out vec4 colorOut;

// magic noise function
float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898f,78.233f))) * 43758.5453f);
}

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

vec3 getInverse(vec3 color) {
	return vec3(1.0f) - color;
}

vec3 getReducePalette(vec3 color) {
	return round(color);;
}

vec3 getDeadPix(vec3 color) {
	if (rand(texCoordsTrans) < deadPixP)
		return vec3(rand(color.yz), rand(color.zx), rand(vec2(tearN, deadPixP)));
	return color;
}

vec3 getColorShift(vec3 color) {
	if (colorShift == 1) return color.gbr;
	else if (colorShift == 2) return color.brg;
	else return color;
}

void main(void) {
	vec3 colorTemp;

	if (useTex) {
		if (tearFlag) colorTemp = (texture(texSampler, getTear())).rgb;
		else colorTemp = (texture(texSampler, texCoordsTrans)).rgb;
	}
	else colorTemp.rgb = color;
	
	if (inverse) colorTemp = getInverse(colorTemp);
	if (reducePalette) colorTemp = getReducePalette(colorTemp);
	if (deadPix) colorTemp = getDeadPix(colorTemp);

	colorTemp = getColorShift(colorTemp);

	colorOut = vec4(colorTemp, alpha);
}