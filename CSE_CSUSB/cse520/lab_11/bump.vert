//bump.vert

varying vec3 lightVec; 
varying vec3 eyeVec;

uniform vec3 LightPosition;

//attribute vec3 vTangent; 
					 
void main(void)
{
  gl_Position = ftransform();
  gl_TexCoord[0] = gl_MultiTexCoord0;
  eyeVec = vec3(gl_ModelViewMatrix * gl_Vertex);

  vec3 vTangent;

  //vTangent.x = -gl_Normal.y;	//vTangent perp to gl_Normal
  vTangent.y = -gl_Normal.z;
  vTangent.z = gl_Normal.y; 
  vTangent.x = 0; // vTangent.y = 0;
	
  vec3 n = normalize(gl_NormalMatrix * gl_Normal);	//normal
  vec3 t = normalize(gl_NormalMatrix * vTangent);	//tangent
  vec3 b = cross(n, t);					//binormal

  //coordinate system origin at vertex with  ( t, b, n ) axis	
  vec3 v;

  v.x = dot(LightPosition, t);
  v.y = dot(LightPosition, b);
  v.z = dot(LightPosition, n);

  lightVec = normalize ( v );

  
  v.x = dot(eyeVec, t);
  v.y = dot(eyeVec, b);
  v.z = dot(eyeVec, n);

  eyeVec = normalize ( v );
}

