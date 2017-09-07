//
// Vertex shader for drawing the earth with one texture
//

//varying float LightIntensity;
//uniform vec3 LightPosition;
uniform float time;

attribute float angle;

//const float specularContribution = 0.1;
//const float diffuseContribution  = 1.0 - specularContribution;

void main(void)
{
/*
    vec3 ecPosition = vec3 (gl_ModelViewMatrix * gl_Vertex);
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition);

    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 16.0);

    LightIntensity  = diffuseContribution * max(dot(lightVec, tnorm), 0.0)
                      + specularContribution * spec;
*/
  float s = 0;
  s = s + sin ( 0.0005 * time );

  angle += 2.0 * sin ( 0.0005 * time );

  gl_TexCoord[0]  = gl_MultiTexCoord0;
  //gl_TexCoord[1]  = gl_MultiTexCoord1;
  gl_Position = gl_ModelViewProjectionMatrix * (vec4(s, s, s, 1.0)  * gl_Vertex );
  //gl_Position     = ftransform();
}
