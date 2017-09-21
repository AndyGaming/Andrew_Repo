# version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 translate;
uniform mat4 rotate;

out vec3 tempColor;

void main()
{
	
	gl_Position = (translate * rotate) * vec4(position, 1.0);
	tempColor = vertexColor;
}
