//tests.frag
//a minimal fragment shader

varying vec3 color;

void main(void)
{
  //gl_FragColor = vec3( 0, 0, 1);	//this does not work anymore
  gl_FragColor = vec4( color, 1);	//red color
}
