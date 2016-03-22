#version 140

in vec3 positionTrans;

uniform mat4 Mmatrix;
uniform mat4 Vmatrix;

uniform int fogControl;
uniform float colorMultiplier;

out vec4 color;

vec4 fogItUp (const vec3 cameraSpacePosition) {
	float distance = length (cameraSpacePosition);
	float fogDensity = 0.001f * colorMultiplier;
	float mixingFactor = exp (- fogDensity * distance);
	mixingFactor = clamp (mixingFactor, 0.0f, 1.0f);
	vec4 fogColor = vec4 (1.0f, 0.6f, 0.5f, 1.0f);
	
	return mixingFactor * color + (1 - mixingFactor) * fogColor;
}

void main (void) {
	vec3 cameraSpacePosition = (Vmatrix * Mmatrix * vec4 (positionTrans, 1.0f)).xyz;

	color = colorMultiplier * vec4 (positionTrans, 1.0f);
	if (fogControl > 0) color = fogItUp (cameraSpacePosition);
}