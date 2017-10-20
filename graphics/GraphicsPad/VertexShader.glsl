# version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normal;

uniform mat4 transform;
uniform vec3 ambientLight;

//out vec3 finalColor;
out vec3 finalNormal;
out vec3 finalPosition;

void main()
{	
	vec4 v = vec4(position, 1.0);
	gl_Position = transform * v;
	finalNormal = normal;
	finalPosition = position;
}
