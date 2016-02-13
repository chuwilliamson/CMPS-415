// For newer cards, use version 330 core or version 400 core
#version 330

// incoming vertex information
layout (location = 0) in vec4 VertexPosition; 
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec3 VertexNormal; 
layout (location = 3) in vec2 VertexUV;

out Vertex
{
	vec4 color;
	vec4 normal;

}vout; 

void main()
{	
	vout.color = VertexColor;		
	vout.normal = vec4(VertexNormal, 0.0);
	gl_Position = VertexPosition;
}

