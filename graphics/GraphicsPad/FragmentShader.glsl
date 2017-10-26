#version 430

out vec4 finalColor;

in vec2 texCoord;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 lightPositionWorld;
uniform vec3 cameraPositionWorld;
uniform vec4 ambientLight;
uniform sampler2D texture_1;

void main()
{
	// Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec4 diffuseLight = clamp(vec4(brightness, brightness, brightness, 1.0), 0, 1);
	
	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 cameraVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specular = clamp(dot(reflectedLightVectorWorld, cameraVectorWorld), 0, 1);
	specular = pow(specular, 32);
	vec4 specularLight = clamp(vec4(specular, 0, 0, 1.0), 0, 1);

	// Attentuation
	float lightAttenuation = 0.05;
	float lightDistance = length(lightPositionWorld - vertexPositionWorld);
	float attenuation = 1.0 / (1.0 + lightAttenuation * pow(lightDistance, 2));
	
	vec4 lighting = ambientLight + attenuation * (diffuseLight + specularLight);
	vec4 texColor = texture(texture_1, texCoord);
	finalColor = texColor * lighting;
}