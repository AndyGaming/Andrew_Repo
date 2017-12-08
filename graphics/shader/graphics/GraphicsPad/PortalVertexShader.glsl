#version 430

in layout(location = 0) vec3 Position;
in layout(location = 1) vec3 Color;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec2 uv;
in layout(location = 4) vec4 tangent;

out vec2 Texcoord;

uniform mat4 portalMVP;

void main()
{
	gl_Position = portalMVP * vec4(Position,1.0);
	Texcoord = uv;
}
