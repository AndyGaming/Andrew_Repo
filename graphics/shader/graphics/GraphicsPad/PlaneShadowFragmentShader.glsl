#version 430

in vec3 v2fcolor;
in vec3 AmbientLightColor;
in vec3 WorldPosition;
in vec2 f_uv;
in mat4 TengentToModelTransform;
in mat4 ModelMatrix;
in vec3 ObjectSpaceNormal;
out vec4 fragColor;

uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform sampler2D tex_temp;
uniform sampler2D NormalMap;
uniform float attenuationAmount;
uniform sampler2D ShadowMap;
uniform mat4 LightFullTransformMatrix;

void main()
{
	//normal map
	vec4 normalmapcolor = texture(NormalMap,f_uv);
	vec3 FixedNormalVector = (normalmapcolor.xyz - vec3(0.5f,0.5f,0.5f))*2;
	vec3 ModelNormal = vec3(TengentToModelTransform * vec4(FixedNormalVector,0.0));

	vec3 lightVector = normalize(LightPosition - WorldPosition);
	float distance = length(LightPosition - WorldPosition);
	float attenuation = 1 / (attenuationAmount * pow(distance,2));
	vec3 PreWorldNormal = vec3 (transpose(inverse(ModelMatrix)) * vec4(ObjectSpaceNormal,0));
	vec3 ActualModelNormal = mix(ObjectSpaceNormal,ModelNormal,clamp(dot(lightVector, PreWorldNormal)*10000,0,1));
	vec3 WorldNormal = vec3 (transpose(inverse(ModelMatrix)) * vec4(ActualModelNormal,0));

	vec4 texColor = texture(tex_temp,f_uv);
	
	vec4 LightclipPos = LightFullTransformMatrix * vec4(WorldPosition, 1.0);
	vec2 shadowmap_uv = vec2(LightclipPos.x / LightclipPos.w,LightclipPos.y / LightclipPos.w) ;
	vec2 shadowmap_uv_fixed = shadowmap_uv * 0.5 + 0.5;
	vec4 shadowmapColor = texture(ShadowMap,shadowmap_uv_fixed);
	float shadowed;
	if(shadowmap_uv_fixed.x >= 1 || shadowmap_uv_fixed.x <=0 || shadowmap_uv_fixed.y >= 1 || shadowmap_uv_fixed.y <=0)
		shadowed = 1;
	else
		shadowed = clamp((shadowmapColor.x - LightclipPos.z / LightclipPos.w ) * 100 ,0,1);
	float brightness = clamp(dot(lightVector, WorldNormal),0,1);
	vec4 diffuseLight =  shadowed * attenuation * brightness * texColor;

	vec3 reflectionVec = reflect(-lightVector,WorldNormal);
	vec3 viewdirection = normalize(ViewPosition - WorldPosition);
	float specularIntensity = pow(dot(viewdirection, reflectionVec),50);
	vec3 specularlight = shadowed * specularIntensity * vec3(1.0,0.0,0.0);

	vec3 AmbientLight = AmbientLightColor * texColor.xyz;

	fragColor = vec4(diffuseLight.xyz + AmbientLight, 1.0);
}
