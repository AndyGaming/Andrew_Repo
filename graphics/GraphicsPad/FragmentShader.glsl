#version 430

out vec4 fragColor;

in vec3 vertexPositionWorld;
in vec2 texCoord;
in mat4 tangentToModelMat;

uniform vec3 lightPositionWorld;
uniform vec3 cameraPositionWorld;
uniform vec4 ambientLight;
uniform mat4 modelToWorldMatrix;

uniform sampler2D texture_1;
uniform sampler2D normalMap_1;

vec4 phongModel(vec3 norm)
{
	// Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(norm));
	vec4 diffuseLight = max(vec4(brightness, brightness, brightness, 1.0), 0.0);

	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, norm);
	vec3 cameraVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specular = max(dot(reflectedLightVectorWorld, cameraVectorWorld), 0.0);
	specular = pow(specular, 32);
	vec4 specularLight = max(vec4(specular, specular, specular, 1.0), 0.0);

	// Attentuation
	float lightAttenuation = 0.05;
	float lightDistance = length(lightPositionWorld - vertexPositionWorld);
	float attenuation = 1.0 / (0.3 * lightDistance + lightAttenuation * pow(lightDistance, 2));

	vec4 lighting = ambientLight + attenuation * (diffuseLight);

	return lighting;
}

void main()
{
	// Normal
	vec4 normalColor = texture(normalMap_1, texCoord);
	vec3 normalTangent = 2 * normalColor.xyz - 1;
	vec3 normalModel = vec3(tangentToModelMat * vec4(normalTangent, 0));
	vec3 normalWorld = vec3(modelToWorldMatrix * vec4(normalModel, 1));

	vec4 texColor = texture(texture_1, texCoord);

	fragColor = phongModel(normalWorld);
}