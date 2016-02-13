// For newer cards, use "#version 330 core" or "#version 400 core"
#version 330

// color from scan conversion (interpolated from vertex shader output)
 
uniform sampler2D TextureColor;

uniform vec3 LightColor;
uniform float Shininess;
uniform float Strength;
uniform float hFactor;
uniform float dFactor;
uniform float sFactor;


in vec4 Color;
in vec4 Normal;
in vec3 ecPosition;
in vec2 UV;

uniform vec3 EyePosition;
uniform vec3 LightPosition;
uniform vec3 WorldPosition;


// final color to write into output buffer
out vec4 FragColor;

void main()
{	
	vec3 SkyColor = vec3(0,0,1);
	vec3 GroundColor = vec3(0,1,0);
	 
	vec3 SurfaceToLight = LightPosition - ecPosition;
	SurfaceToLight = normalize(SurfaceToLight);

	vec3 SurfaceToEye = -1 * ecPosition;
	SurfaceToEye = normalize(SurfaceToEye);

	vec3 WorldToSurface =  ecPosition - WorldPosition;
	WorldToSurface = normalize(WorldToSurface);

	vec3 HalfVector = normalize(SurfaceToLight + SurfaceToEye); 
	HalfVector = normalize(HalfVector);
	 
	float diffuse  = max(0.0, dot(vec3(Normal), SurfaceToLight)); 
	float specular = max(0.0, dot(vec3(Normal), HalfVector)); 
	float hemisphere = (dot(vec3(Normal), WorldToSurface) * 0.5) + 0.5;

	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess) * sFactor;

	vec3 Id = dFactor * vec3(Color) * diffuse ; 	 
	vec3 Is = sFactor * vec3(Color) * specular;
	vec3 Ih = hFactor * mix(GroundColor, SkyColor, hemisphere); 

	vec3 rgb =  Id + Is + Ih;

	FragColor = vec4(rgb, Color.a) * texture2D(TextureColor, UV);	
}

