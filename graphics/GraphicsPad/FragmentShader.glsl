#version 430

out vec4 color;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 lightPositionWorld;
uniform vec3 cameraPositionWorld;
uniform vec4 ambientLight;

void main()
{
	// Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	
	// Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 cameraVectorWorld = normalize(cameraPositionWorld - vertexPositionWorld);
	float specular = dot(reflectedLightVectorWorld, cameraVectorWorld);
	specular = pow(specular, 32);
	vec4 specularLight = vec4(specular, 0, 0, 1.0);
	
	color = clamp(diffuseLight, 0, 1) + ambientLight + clamp(specularLight, 0, 1);
}