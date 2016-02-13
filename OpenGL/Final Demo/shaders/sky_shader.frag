// For newer cards, use "#version 330 core" or "#version 400 core"
#version 330

// color from scan conversion (interpolated from vertex shader output)
 
uniform sampler2D TextureColor; 

in vec4 Color; 
in vec2 UV;
 
// final color to write into output buffer
out vec4 FragColor;

void main()
{	
	FragColor = texture2D(TextureColor, UV);	
}

