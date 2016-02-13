// For newer cards, use "#version 330 core" or "#version 400 core"
#version 330

// color from scan conversion (interpolated from vertex shader output)
 
in vec4 Color;
 
// final color to write into output buffer
out vec4 FragColor;

void main()
{ 
	FragColor = vec4(1.0,1.0,1.0,1.0);
}
