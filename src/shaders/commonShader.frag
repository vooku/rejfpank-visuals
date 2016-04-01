#version 140

in vec3 positionTrans;
in vec3 normalTrans;

out vec4 color;

void main (void) {
	color = /*0.2 * */vec4 (normalTrans, 1.0f);
}