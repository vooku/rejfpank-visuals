#version 140

in vec2 texCoordsTrans;

uniform sampler2D texSampler;
uniform vec3 color;
uniform float alpha;
uniform bool useTex;
uniform bool inverse;
uniform bool reducePalette;
uniform bool tearFlag;
uniform int tearN; // actual number of tear border
uniform float tearBorders[10]; // enough space for max 5
uniform float tearOffsets[11]; // enough space for max 6

out vec4 colorOut;

void main (void) {
	vec4 colorTemp;

	if (useTex) {
		vec2 texCoordsOut = texCoordsTrans;

		if (tearFlag) {
			if (tearN == 1) {
				if (texCoordsOut.y <= tearBorders[0]) texCoordsOut.x += tearOffsets[0];
				else texCoordsOut.x += tearOffsets[1];
			}
			else {
				if (texCoordsOut.y <= tearBorders[0]) texCoordsOut.x += tearOffsets[0];
				else if (texCoordsOut.y > tearBorders[tearN]) texCoordsOut.x += tearOffsets[tearN + 1];
				else for (int i = 0; i < tearN - 1; i++)
					if (texCoordsTrans.y > tearBorders[i] && texCoordsTrans.y <= tearBorders[i + 1])
						texCoordsOut.x += tearOffsets[i];
			}
			if (texCoordsOut.x > 1.0f) texCoordsOut.x -= 1.0f;
		}

		colorTemp = texture (texSampler, texCoordsOut);
	}
	else colorTemp.rgb = color;
	
	if (inverse) colorTemp = vec4(1.0f) - colorTemp;

	if (reducePalette) colorTemp = round(colorTemp);
	
	colorTemp.a = alpha;
	colorOut = colorTemp;
}