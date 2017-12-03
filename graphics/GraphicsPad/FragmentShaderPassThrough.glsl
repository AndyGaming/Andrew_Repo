#version 430;

in vec3 vertexToFragmentColor;

out vec4 finalColor;

void main()
{
	finalColor = vec4(vertexToFragmentColor, 1.0);
}