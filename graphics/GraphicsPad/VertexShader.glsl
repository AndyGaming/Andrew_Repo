#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 normalModel;
in layout(location=3) vec2 uv;
in layout(location=4) vec3 tangent;

uniform mat4 modelToProjectionMat;
uniform mat4 modelToWorldMat;

out vec3 vertexPositionWorld;
//out vec3 normalWorld;
out vec2 texCoord;
out mat4 tangentToModelMat;

void main()
{	
	gl_Position = modelToProjectionMat * vertexPositionModel;
	//normalWorld = vec3(modelToWorldMat * vec4(normalModel, 0));
	vertexPositionWorld = vec3(modelToWorldMat * vertexPositionModel);
	texCoord = uv;

	vec3 bitangent = normalize(cross(normalModel, tangent));

	tangentToModelMat = mat4(
		vec4(tangent, 0), vec4(bitangent, 0), vec4(normalModel, 0), vertexPositionModel
	);
}
