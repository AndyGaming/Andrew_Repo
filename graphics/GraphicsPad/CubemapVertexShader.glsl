#version 430

in layout(location=0) vec4 vertexPositionModel;
in layout(location=1) vec3 vertexColor;
in layout(location=2) vec3 vertexNormalModel;
in layout(location=3) vec2 texCoordModel;

out vec3 reflectDir;
out vec3 refractDir;

uniform bool drawSkyBox;
uniform vec3 cameraPositionWorld;
uniform mat4 modelToWorldMatrix;
//uniform mat4 modelViewMatrix;
//uniform mat3 normalMatrix;
//uniform mat4 projectionMatrix;
uniform mat4 MVP;

void main()
{
	bool b = true;

	if (b) {
		reflectDir = -vec3(vertexPositionModel);
	}
	else {
		vec3 worldPos = vec3(modelToWorldMatrix * vertexPositionModel);
		vec3 worldNorm = vec3(modelToWorldMatrix * vec4(vertexNormalModel, 0.0));

		vec3 worldView = normalize(cameraPositionWorld - worldPos);
		reflectDir = reflect(-worldView, worldNorm);
	}

	gl_Position = MVP * vertexPositionModel;
}