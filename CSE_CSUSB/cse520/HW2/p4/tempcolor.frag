/*
  tempcolor.frag:
  uniform qualified variables are changed at most once per primitive
  by the application, and vec3 declares a vector of three
  floating-point numbers.
*/
  
uniform vec3 CoolestColor; 	
uniform vec3 HottestColor;	

varying float Temperature;

void main(void)
{
  //get a color between coolest and hottest colors, using the
  //  mix() built-in function
  vec3 color = mix ( CoolestColor, HottestColor, Temperature );
  //make a color with alpha of 1.0
  gl_FragColor = vec4(color, 1.0);	
}

