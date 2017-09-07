//morph.vert: morphing vertex

uniform float t;	//value provided by application program
attribute vec4 Vertex2;

void main(void)
{
  //vec4 f = vec4 ( mix(gl_Vertex.xyz, Vertex2.xyz, t ), 1.0 );	//f = Vertex*(1-t) + Vertex2*t  
  vec4 f = vec4 ( mix(gl_Vertex.xy, Vertex2.xy, t ), 0.0, 1.0 );	//f = Vertex*(1-t) + Vertex2*t  

  gl_Position = gl_ModelViewProjectionMatrix * f;

}
