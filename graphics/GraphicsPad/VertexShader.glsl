#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 vertexNormalModel;
in layout(location=3) vec2 vertexTexCoord;
in layout(location=4) vec3 vertexTangent;

struct LightInfo {
	vec3 position;
	vec3 intensity;
};

uniform vec3 lightPositionWorld;

uniform mat4 modelViewMat;
uniform mat3 normalMatrix;
uniform mat4 MVP;

out LightInfo Light;
out vec3 lightDir;
out vec3 viewDir;
out vec2 texCoord;
out vec3 vertexPositionWorld;

void main()
{
	Light.position = lightPositionWorld;
	Light.intensity = vec3(1.0, 1.0, 32.0);

	vec3 norm = normalize(normalMatrix * vertexNormalModel);
	vec3 tangent = normalize(normalMatrix * vertexTangent);

	vec3 binormal= normalize(cross(norm, tangent));

	mat3 toObjectLocal = mat3(
		tangent.x, binormal.x, norm.x,
		tangent.y, binormal.y, norm.y,
		tangent.z, binormal.z, norm.z,
	);

	vertexPositionWorld = vec3(modelViewMat * vertexPositionModel);

	lightDir = normalize(toObjectLocal * (Light.position - vertexPositionWorld));
	viewDir = toObjectLocal * normalize(-vertexPositionWorld);

	texCoord = vertexTexCoord;
	gl_Position = MVP * vertexPositionModel;
}
