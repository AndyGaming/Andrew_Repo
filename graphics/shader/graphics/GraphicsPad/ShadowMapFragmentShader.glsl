#version 430

in vec2 f_uv;

out vec4 fragColor;

uniform sampler2D tex_temp;

void main()
{
	fragColor = texture(tex_temp,f_uv);
}
