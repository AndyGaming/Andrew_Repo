//movingv.vert: moving vertex

uniform float time;	//value provided by application program

void main(void)
{
  float s = 0;
 
  //s = s + 2.0 * sin ( 0.0002 * time );

  //gl_Position = gl_ModelViewProjectionMatrix * (vec4(s, s, s, 1.0)  * gl_Vertex );
  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_Position     = ftransform();
}
