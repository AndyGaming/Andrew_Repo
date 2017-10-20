#version 430

out vec4 color;
//in vec3 finalColor;
in vec3 finalNormal;
in vec3 finalPosition;

uniform vec3 lightPosition;

void main()
{
	vec3 lightVector = normalize(lightPosition - finalPosition);
	float brightness = dot(lightVector, finalNormal);
	color = vec4(brightness, brightness, brightness, 1.0);
}