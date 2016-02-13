// For newer cards, use version 330 core or version 400 core
#version 330

// incoming vertex information
layout (location = 0) in vec4 VertexPosition; 
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal; 
layout (location = 3) in vec2 VertexUV;


out vec4 Color;
out vec4 Normal;
out vec2 UV;  
out vec3 ecPosition;

uniform mat4 MVPMatrix;
uniform mat4 NormalMatrix;

void main()
{
	UV = VertexUV; 
	Color = VertexColor; 	
	Normal = normalize(NormalMatrix * vec4(VertexNormal,0));	 
	ecPosition = vec3(NormalMatrix * VertexPosition); 
	gl_Position = MVPMatrix * VertexPosition; 
	
}

