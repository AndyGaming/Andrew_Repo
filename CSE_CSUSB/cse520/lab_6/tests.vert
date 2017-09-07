//tests.vert
//a minimal vertex shader

attribute vec3 temp;
varying vec3 color;

void main(void)
{
  color = temp;
  gl_Position     = ftransform();
 
  /* 
    Same as:
      gl_Position = gl_ModelViewMatrix * gl_ProjectionMatrix * gl_Vertex;
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  */ 
}
