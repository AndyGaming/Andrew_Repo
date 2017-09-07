//tempcolor.vert

//uniform qualified variables are changed at most once per primitives
uniform float CoolestTemp;
uniform float TempRange;

//attribute qualified variables are typically changed per vertex
attribute float VertexTemp;

//varying qualified variables communicate from the vertex shader to
//the fragment shader
varying float Temperature;

void main(void)
{
  //compute a temperature to be interpolated per fragment
  //  in the range [0.0, 1.0]
  Temperature = ( VertexTemp - CoolestTemp ) / TempRange;
//  Temperature = ( VertexTemp/2 - CoolestTemp ) / TempRange;
  gl_Position     = ftransform();

  /*
    Same as:
      gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  */ 
}

