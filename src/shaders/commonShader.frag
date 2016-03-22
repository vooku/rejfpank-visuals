#version 140

// ----------------------------------------------------------------------------------- Struct definitions
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	vec3  position;
};

struct Spotlight {
	Light light; // because inheritance doesnt seem to work in glsl
	vec3  direction;
	float cosCutOff;
	float exponent;
};

// ----------------------------------------------------------------------------------- Global variables
in vec3 positionTrans;
in vec3 normalTrans;
in vec2 texCoordsTrans;

uniform mat4 PVMmatrix;
uniform mat4 Mmatrix;
uniform mat4 Vmatrix;
uniform mat4 normalmatrix; // inverse transposed Mmatrix
uniform Material material;
uniform sampler2D texSampler;

uniform float colorMultiplier;
uniform int fogControl;
uniform int pointLightControl;
uniform int directionalLightControl;
uniform int spotLightControl;
uniform vec3 cameraDirection;

Light god; // point light
Light sun; // directional
Spotlight spotlight;

out vec4 color;

// ----------------------------------------------------------------------------------- Functions
void initLights (const vec3 cameraSpacePosition) {
	//god.position = (Vmatrix * vec4 (0.0f, 0.0f, 5.0f, 0.0f)).xyz;
	god.position = vec3 (0.0f);
	god.ambient = vec3 (0.5f);
	god.diffuse = vec3 (0.5f);
	god.specular = vec3 (0.5f);

	sun.position = (Vmatrix * vec4 (-1.0f, 0.0f, 0.0f, 0.0f)).xyz;
	sun.ambient = vec3 (0.3f);
	sun.diffuse = vec3 (0.3f);
	sun.specular = vec3 (0.3f);
	
	spotlight.light.position = vec3 (0.0f);
	spotlight.light.ambient = vec3 (0.5f);
	spotlight.light.diffuse = vec3 (0.5f);
	spotlight.light.specular = vec3 (0.5f);
	spotlight.direction = normalize ((Vmatrix * vec4 (cameraDirection, 0.0)).xyz);
	spotlight.cosCutOff = 0.95f; 
	spotlight.exponent = 0.0f;
}

vec3 objectColor (void) {
	return texture (texSampler, texCoordsTrans);
}

vec4 computeLightParts (const Light light, const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal, const vec3 toLight) {
	vec3 result = vec3 (0.0f);
	vec3 toCamera = normalize (-cameraSpacePosition);
	vec3 s = normalize ((toCamera + toLight) / length (toCamera + toLight));

	// ambient
	result += light.ambient * material.ambient;
	
	 // diffuse
	float cos_angle = dot (toLight, cameraSpaceNormal);
	result += (max (cos_angle, 0)) * light.diffuse * material.diffuse;

	// add texture
	result *= objectColor ();

	// specular
	cos_angle = dot (s, cameraSpaceNormal); 
	result += pow ((max (cos_angle, 0)), material.shininess) * light.specular * material.specular;
    
	return vec4 (result, 1.0f);
}

vec4 directional (const Light light, const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	return computeLightParts (light, cameraSpacePosition, cameraSpaceNormal, normalize (-light.position));
}

float computeAttenuation (const float dist, const float constant, const float linear, const float quadratic) {
	return 1.0f / (constant + linear * dist + quadratic * pow (dist, 2));
}

vec4 point (const Light light, const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	vec3 toLight = light.position - cameraSpacePosition;
	vec4 result = computeLightParts (light, cameraSpacePosition, cameraSpaceNormal, normalize (toLight));
	float attenuationFactor = computeAttenuation (length (toLight), 0.0f, 0.2f, 0.2f);

	return result * attenuationFactor;
}

vec4 spot (const Spotlight light, const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	vec3 toLight = light.light.position - cameraSpacePosition;
	
	float cosPosition = max (0.0, dot (light.direction, normalize (-toLight)));
	if (cosPosition < light.cosCutOff) return vec4 (0.0f);

	vec4 result = computeLightParts (light.light, cameraSpacePosition, cameraSpaceNormal, normalize (toLight));
	float attenuationFactor = computeAttenuation (length (toLight), 0.0f, 0.1f, 0.0f);

	return result * attenuationFactor * pow (light.cosCutOff, light.exponent);
}

vec4 lightItUp (const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	vec4 result = vec4 (0.0f);
	result += vec4 (0.1f) * objectColor (); // Global ambient
	if (pointLightControl > 0) result += point (god, cameraSpacePosition, cameraSpaceNormal);
	if (directionalLightControl > 0) result += directional (sun, cameraSpacePosition, cameraSpaceNormal);
	if (spotLightControl > 0) result += spot (spotlight, cameraSpacePosition, cameraSpaceNormal);
	return result;
}

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
	vec3 cameraSpaceNormal = normalize ((Vmatrix * Mmatrix * vec4 (normalTrans, 0.0)).xyz); // normalmatrix ?!!!
	
	initLights (cameraSpacePosition);
	color = lightItUp (cameraSpacePosition, cameraSpaceNormal);
	
	if (fogControl > 0) color = fogItUp (cameraSpacePosition);

}