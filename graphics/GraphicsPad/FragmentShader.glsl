#version 430

out vec4 color;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 lightPositionWorld;
uniform vec4 ambientLight;

void main()
{
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);
	color = clamp(diffuseLight, 0, 1) + ambientLight;
}