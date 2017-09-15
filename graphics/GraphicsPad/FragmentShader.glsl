#version 430

out vec4 color;
in vec3 tempColor;

void main()
{
	color = vec4(tempColor, 1.0);
}