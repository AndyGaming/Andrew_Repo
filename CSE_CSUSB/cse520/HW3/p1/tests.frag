//tests.frag
//a minimal fragment shader

uniform vec3 CoolColor;
uniform vec3 HotColor;

varying float temperature;

void main(void)
{
   vec3 color = mix( CoolColor, HotColor, temperature );

   gl_FragColor = vec4( color, 1);	
}
