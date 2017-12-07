#version 430

in vec3 VertexColor;
out vec4 fragColor;

void main()
{
	fragColor = vec4(VertexColor,1.0);
}
