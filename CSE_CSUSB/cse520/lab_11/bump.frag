//bump.frag
varying vec3 lightVec;
varying vec3 eyeVec;

uniform vec3  SurfaceColor;
uniform float BumpDensity;
uniform float BumpSize;
uniform float SpecularFactor;

void main (void)
{
  vec3 litColor;
  vec2 c = BumpDensity * gl_TexCoord[0].st;
  vec2 p = fract( c ) - vec2( 0.5 );

  float d, f;

  d = p.x * p.x + p.y * p.y;
  f = 1.0 / sqrt ( d + 1.0 );

  if ( d >= BumpSize ){
    p = vec2 ( 0.0 );
    f = 1.0; 
  }

  vec3 normalDelta = vec3 ( p.x, p.y, 1.0 ) * f;
  litColor = SurfaceColor * max(dot(normalDelta, lightVec), 0.0 );
  vec3 reflectVec = reflect ( lightVec, normalDelta );
 
  float specular = max ( dot(eyeVec, reflectVec), 0.0 );
  specular = pow ( specular, 6.0 ) * SpecularFactor;
  litColor = min ( litColor + specular, vec3(1.0) );

  gl_FragColor = vec4 ( litColor, 1.0 );
}     
