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
in vec2 texCoordsTrans;

uniform mat4 PVMMatrix;
uniform mat4 MMatrix;
uniform mat4 VMatrix;
uniform mat4 normalMatrix; // inverse transposed Matrix
uniform TMaterial material;
uniform bool whiteFlag;
uniform bool redFlag;
uniform bool blueFlag;
uniform bool pointFlag;
uniform vec3 cameraPosition;
uniform bool useTex;
uniform float alpha;
uniform sampler2D texSampler;

TLight white, red, blue, point;

out vec4 color;

// ----------------------------------------------------------------------------------- Functions
void lightsInit(void) {
	if (whiteFlag) {
		white.position	= (VMatrix * vec4(-1.0f, 0.0f, -1.0f, 0.0f)).xyz;
		white.ambient	= vec3(1.0f);
		white.diffuse	= vec3(1.0f);
		white.specular	= vec3(0.5f);
	}
	if (redFlag) {
		red.position	= (VMatrix * vec4(1.0f, 0.0f, -1.0f, 0.0f)).xyz;
		red.ambient		= vec3 (1.0f, 1.0f, 1.0f);
		red.diffuse		= vec3 (0.5f, 0.0f, 0.0f);
		red.specular	= vec3 (1.5f, 0.0f, 0.0f);
	}
	if (blueFlag) {
		blue.position	= (VMatrix * vec4(-1.0f, -1.0f, 1.0f, 0.0f)).xyz;
		blue.ambient	= vec3 (1.0f, 1.0f, 1.0f);
		blue.diffuse	= vec3 (0.5f, 0.0f, 0.5f);
		blue.specular	= vec3 (1.0f, 0.0f, 1.0f);
	}
	if (pointFlag) {
		point.position	= (/*VMatrix */ vec4(cameraPosition, 0.0f)).xyz;
		point.ambient	= vec3 (2.0f);
		point.diffuse	= vec3 (2.0f);
		point.specular	= vec3 (2.0f);
	}
}

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
	if (useTex) result *= texture(texSampler, texCoordsTrans);

	// specular
	cos_angle = dot (s, cameraSpaceNormal); 
	result += pow ((max (cos_angle, 0)), material.shininess) * light.specular * material.specular;
    
	return vec4 (result, 1.0f);
}

vec4 directionalLight(const TLight light, const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	return computeLightParts (light, cameraSpacePosition, cameraSpaceNormal, normalize (-light.position));
}

float computeAttenuation (const float dist, const float constant, const float linear, const float quadratic) {
	return 1.0f / (constant + linear * dist + quadratic * pow (dist, 2));
}

vec4 pointLight(const TLight light, const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	vec3 toLight = light.position - cameraSpacePosition;
	vec4 result = computeLightParts(light, cameraSpacePosition, cameraSpaceNormal, normalize (toLight));
	float attenuationFactor = computeAttenuation (length(toLight), 0.0f, 0.1f, 0.03f);

	return result * attenuationFactor;
}

vec4 lightItUp(const vec3 cameraSpacePosition, const vec3 cameraSpaceNormal) {
	vec4 result = vec4 (0.05f);
	if (whiteFlag)	result += directionalLight(white, cameraSpacePosition, cameraSpaceNormal);
	if (redFlag)	result += directionalLight(red,	cameraSpacePosition, cameraSpaceNormal);
	if (blueFlag)	result += directionalLight(blue, cameraSpacePosition, cameraSpaceNormal);
	if (pointFlag)	result += pointLight(point, cameraSpacePosition, cameraSpaceNormal);
	if (useTex) result *= texture(texSampler, texCoordsTrans);
	return result;
}

void main (void) {
	vec3 cameraSpacePosition = (VMatrix * MMatrix * vec4(positionTrans, 0.0f)).xyz;
	vec3 cameraSpaceNormal = normalize((normalMatrix * vec4(normalTrans, 1.0f)).xyz);
	
	lightsInit();
	color = lightItUp(cameraSpacePosition, cameraSpaceNormal);
	color.a = alpha;
}