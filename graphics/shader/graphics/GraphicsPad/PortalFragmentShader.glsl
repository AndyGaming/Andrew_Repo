#version 430

in vec2 Texcoord;

out vec4 fragColor;

uniform sampler2D portalTexture;

void main()
{
	fragColor = texture(portalTexture,Texcoord);
}
