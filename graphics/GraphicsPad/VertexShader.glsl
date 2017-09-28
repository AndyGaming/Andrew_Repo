# version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 transform;
uniform mat4 modelTransMat;
uniform mat4 projectionMat;

out vec3 tempColor;

void main()
{	
	vec4 v = vec4(position, 1.0);
	vec4 newPos = modelTransMat * v;
	vec4 projectedPos = projectionMat * newPos;
	gl_Position = transform * projectedPos;
	tempColor = vertexColor;
}
