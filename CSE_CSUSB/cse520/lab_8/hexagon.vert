//tests.vert
//a minimal vertex shader

varying float temperature;

uniform float time;
uniform float CoolTemp;
uniform float tempRange;

attribute float VertexTemp;

void main(void)
{
  //the temperature changes from 1 to 0 and then 0 to 1
  temperature = (( VertexTemp - CoolTemp ) / tempRange) + sin ( 0.0005 * time );

  gl_Position = ftransform();
}
