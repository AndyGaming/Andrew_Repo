# version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normal;

uniform vec3 ambientLight;
uniform mat4 transform;

out vec3 tempColor;

void main()
{	
	vec4 v = vec4(position, 1.0);
	gl_Position = transform * v;
	tempColor = normal;
}