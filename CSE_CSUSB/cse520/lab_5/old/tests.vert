//tests.vert
//a minimal vertex shader
void main(void)
{
  gl_Position     = ftransform();
  /*
    Same as:
      gl_Position = gl_ModelViewMatrix * gl_ProjectionMatrix * gl_Vertex;
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  */ 
}
