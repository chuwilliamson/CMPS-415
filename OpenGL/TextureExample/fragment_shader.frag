#version 330 core

// Texture coordinate values from the vertex shaders
in vec2 UV;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_Colors;

void main(){
	color = texture2D( texture_Colors, UV ).rgb;
}