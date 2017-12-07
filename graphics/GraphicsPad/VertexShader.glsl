#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 vertexNormalModel;
in layout(location=3) vec2 texCoordModel;
in layout(location=4) vec3 vertexTangentModel;

uniform mat4 MVP;
uniform mat4 modelToWorldMatrix;

out vec3 vertexPositionWorld;
out vec2 texCoord;
out mat4 tangentToModelMat;

void main()
{	
	gl_Position = MVP * vertexPositionModel;
	vertexPositionWorld = vec3(modelToWorldMatrix * vertexPositionModel);
	texCoord = texCoordModel;

	vec3 bitangent = normalize(cross(vertexNormalModel, vertexTangentModel));

	tangentToModelMat = mat4(
		vec4(vertexTangentModel, 0), vec4(bitangent, 0), vec4(vertexNormalModel, 0), vertexPositionModel
	);
}
