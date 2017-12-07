#version 430

in vec3 Texcoord;

out vec4 fragColor;

uniform samplerCube CubeMap;

void main()
{
	fragColor = texture(CubeMap,Texcoord);
}
