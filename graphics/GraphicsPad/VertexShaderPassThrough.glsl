#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;

out vec3 vertexToFragmentColor;

uniform mat4 modelToProjectionMat;

void main()
{
	gl_Position = modelToProjectionMat * vertexPositionModel;
	vertexToFragmentColor = vec3(1.0, 1.0, 1.0);
}