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
in vec4 Position;
in vec2 UV;


uniform vec3 EyePosition;
uniform vec3 LightPosition;
uniform vec3 WorldPosition;


// final color to write into output buffer
out vec4 FragColor;

void main()
{	

	vec3 LightDirection = LightPosition - vec3(Position);
	float lightDistance = length(LightDirection);
	LightDirection = LightDirection / lightDistance;

	vec3 EyeDirection =  WorldPosition - EyePosition;
	float eyeDistance = length(EyeDirection);
	EyeDirection = EyeDirection / eyeDistance;

	vec3 UpDirection = LightPosition - WorldPosition;
	float upDistance = length(UpDirection);
	UpDirection = UpDirection / upDistance;
	
	vec3 HalfVector = normalize(LightDirection + EyeDirection);
	vec3 UpVector = normalize(LightPosition - vec3(Position));
	
	float diffuse  = max(0.0, dot(vec3(Normal), LightDirection)); 
	float specular = max(0.0, dot(vec3(Normal), HalfVector)); 
	float hemidot = (dot (vec3(Normal), UpVector));
	float hemisphere = 0.5 + 0.5 * hemidot;
	
	if (diffuse == 0.0) 
		specular = 0.0; 
	else specular = pow(specular, Shininess) * Strength;
 
	
	vec3 d = vec3(Color) * diffuse    * dFactor; 	 
	vec3 s = vec3(Color) * specular   * sFactor; 
	vec3 h = vec3(Color) * hemisphere * hFactor; 

	vec3 rgb = min(Color.rgb * d + s + h , vec3(1.0));
	FragColor = vec4(rgb, Color.a) * texture2D(TextureColor, UV);
	 
	
}

