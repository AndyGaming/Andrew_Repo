#version 430

in vec3 lightDir;
in vec3 viewDir;
in vec2 texCoord;
in vec3 vertexPositionWorld;

uniform sampler2D texture_1;
uniform sampler2D normalMap_1;

struct LightInfo {
	vec3 position;
	vec3 intensity;
};

in LightInfo Light;

uniform vec4 ambientLight;

out vec4 finalColor;

vec4 phongModel(vec3 norm, vec3 diffR)
{
	// Ambient	
	vec4 ambient = ambientLight * Light.intensity.x;

	// Diffuse
	float brightness = max(dot(lightDir, norm), 0.0);
	vec4 diffuse = vec4(Light.intensity.y * diffR * brightness, 1.0);

	// Specular
	vec4 specular = vec4(0.0);
	if (brightness > 0.0)
	{
		vec3 reflectedLightVectorWorld = reflect(-lightDir, norm);
		float shininess = pow(max(dot(reflectedLightVectorWorld, viewDir), 0.0), Light.intensity.z);
		specular = max(vec4(shininess, shininess, shininess, 1.0), 0.0);
	}

	// Attentuation
	float lightAttenuation = 0.05;
	float lightDistance = length(Light.position - vertexPositionWorld);
	float attenuation = 1.0 / (0.1 * lightDistance + lightAttenuation * pow(lightDistance, 2));
	
	return ambient + attenuation * (diffuse);
}

void main()
{
	// Normal
	vec4 normalColor = texture(normalMap_1, texCoord);
	vec4 texColor = texture(texture_1, texCoord);

	finalColor = phongModel(normalColor.xyz, vec3(1.0));
}