//
//pokemon.frag
//

varying float LightIntensity;
uniform sampler2D texHandle;

void main (void)
{
    vec3 lightColor = vec3 (texture2D(texHandle, gl_TexCoord[0].st));
    gl_FragColor    = vec4 (lightColor * LightIntensity, 1.0);
}
