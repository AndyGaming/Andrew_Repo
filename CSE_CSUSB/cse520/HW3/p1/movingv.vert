//movingv.vert: moving vertex

uniform float time;	//value provided by application program
uniform float tempRange;

attribute float angle;
attribute float VertexTemp;

varying float temperature;

void main(void)
{
  float s = 0;
  
  angle += 2.0 * sin ( 0.0005 * time );
 
  s = s + 2.0 * sin ( 0.0005 * time );

  temperature = (VertexTemp / tempRange) + sin ( 0.0005 * time );

  gl_Position = gl_ModelViewProjectionMatrix * (vec4(s, s, s, 1.0)  * gl_Vertex );
  //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  //gl_Position     = ftransform();
}
