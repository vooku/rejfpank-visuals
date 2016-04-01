#version 140

// ----------------------------------------------------------------------------------- Struct definitions
struct TMaterial {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct TLight {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

// ----------------------------------------------------------------------------------- Global variables
in vec3 positionTrans;
in vec3 normalTrans;
//in vec2 texCoordsTrans;

uniform mat4 PVMMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 normalMatrix; // inverse transposed Matrix
uniform TMaterial material;
//uniform sampler2D texSampler;

TLight whiteLight, redLight, blueLight;

out vec4 color;

// ----------------------------------------------------------------------------------- Functions
void lightsInit(void) {
	whiteLight.position = (VMatrix * vec4 (-1.0f, 1.0f, 0.0f, 0.0f)).xyz;
	whiteLight.ambient = vec3 (1.0f, 1.0f, 1.0f);
	whiteLight.diffuse = vec3 (0.5f, 0.5f, 0.5f);
	whiteLight.specular = vec3 (0.3f, 0.3f, 0.3f);
	
	redLight.position = (VMatrix * vec4 (-1.0f, 1.0f, -1.0f, 0.0f)).xyz;
	redLight.ambient = vec3 (1.0f, 1.0f, 1.0f);
	redLight.diffuse = vec3 (0.5f, 0.0f, 0.0f);
	redLight.specular = vec3 (3.0f, 0.0f, 0.0f);

	blueLight.position = (VMatrix * vec4 (-1.0f, -1.0f, 1.0f, 0.0f)).xyz;
	blueLight.ambient = vec3 (1.0f, 1.0f, 1.0f);
	blueLight.diffuse = vec3 (0.0f, 0.0f, 0.5f);
	blueLight.specular = vec3 (3.0f, 0.0f, 0.0f);
}
/*
vec3 objectColor (void) {
	return texture (texSampler, texCoordsTrans);
}
*/
vec4 computeLightParts(const TLight light, const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal, const vec3 toLight) {
	vec3 result = vec3 (0.0f);
	vec3 toCamera = normalize (-cameraSpacePosition);
	vec3 s = normalize ((toCamera + toLight) / length (toCamera + toLight));

	// ambient
	result += light.ambient * material.ambient;
	
	 // diffuse
	float cos_angle = dot (toLight, cameraSpaceNormal);
	result += (max (cos_angle, 0)) * light.diffuse * material.diffuse;

	// add texture
	//result *= objectColor ();

	// specular
	cos_angle = dot (s, cameraSpaceNormal); 
	result += pow ((max (cos_angle, 0)), material.shininess) * light.specular * material.specular;
    
	return vec4 (result, 1.0f);
}

vec4 lightItUp (const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	vec4 result = vec4 (0.0f);
	//result += vec4 (0.1f) * objectColor (); // Global ambient
	result += computeLightParts (whiteLight, cameraSpacePosition, cameraSpaceNormal, normalize (whiteLight.position));
	//result += computeLightParts (redLight, cameraSpacePosition, cameraSpaceNormal, normalize (redLight.position));
	//result += computeLightParts (blueLight, cameraSpacePosition, cameraSpaceNormal, normalize (blueLight.position));
	return result;
}

void main (void) {
	vec3 cameraSpacePosition = (VMatrix * MMatrix * vec4 (positionTrans, 1.0f)).xyz;
	vec3 cameraSpaceNormal = normalize ((VMatrix * MMatrix * vec4 (normalTrans, 0.0)).xyz); // normalmatrix ?!!!
	
	lightsInit();
	color = lightItUp(cameraSpacePosition, cameraSpaceNormal);
}