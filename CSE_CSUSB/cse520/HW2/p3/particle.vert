//particle.vert

uniform float time;			//value provided by application program
uniform vec3 vel;			//value provided by application program
attribute vec3 temp;
varying vec3 color;



void main(void)
{
  color = temp;

  float s = 1000.0;			//scale factor
  float g = -10.0;
  float t;
  float h, h0;
  float t0;
  float cor = 0.85;
  t =  time / s; 			//time in ms

  h0 = gl_Vertex.y;
  vec3 n = vec3( 0,1,0 );
  vec3 v1;
  vec3 v2;

  t0 = sqrt ( 2.0 * h0 / (-g) ); 

  v1.x = vel.x;
  v1.y = vel.y + g * t0;
  v1.z = vel.z;

  h = h0 + g/(2.0)*t*t;
  int count = 0; 

  while (h <= 0.0) {
    v2 = cor * reflect (v1, n);
    t -= t0;
    if (t < 0.0)  t = 0.0;
    h = v2.y*t + g/(2.0)*t*t;
    h0 = cor * h0;
    if (h0 < 0.0)  h0 = 0.0;
    t0 *= 2; 
    v1.y = v2.y + g*t0;
    if (count++ > 100) break;
  }

  gl_Position = gl_ModelViewProjectionMatrix * vec4 (0, h, 0, 1);
}
