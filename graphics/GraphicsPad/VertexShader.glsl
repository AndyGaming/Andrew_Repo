# version 430

in layout(location=0) vec4 vertexPositionNodel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;

uniform mat4 modelToProjectionMat;
uniform mat4 modelToWorldMat;
uniform vec3 ambientLight;

out vec3 normalWorld;
out vec3 vertexPositionWorld;

void main()
{	
	gl_Position = modelToProjectionMat * vertexPositionNodel;
	normalWorld = vec3(modelToWorldMat * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMat * vertexPositionNodel);
}
