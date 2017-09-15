# version 430

in layout(location=0) vec2 position;
in layout(location=1) vec3 vertexColor;

out vec3 tempColor;

uniform mat3 transform;

void main()
{
	gl_Position = vec4(transform * position, 1.0, 1.0);
	tempColor = vertexColor;
}
