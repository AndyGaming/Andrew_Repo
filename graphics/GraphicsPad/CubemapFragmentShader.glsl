#version 430

in vec3 reflectDir;

uniform samplerCube cubeMapTex;
uniform bool drawSkyBox;

out vec4 fragColor;

void main()
{
	// Access cube map texture
	vec4 cubeMapColor = texture(cubeMapTex, reflectDir);
	vec4 materialColor = vec4(1.0, 1.0, 0.0, 1.0);
	float reflectFactor = 0.5;

	bool b = true;

	if (b) {
		fragColor = cubeMapColor;
	}
	else {
		fragColor = mix(materialColor, cubeMapColor, reflectFactor);
	}
}