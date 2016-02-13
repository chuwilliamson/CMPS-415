#version 150
layout(triangles) in;

// Three lines will be generated: 6 vertices
layout(line_strip, max_vertices=6) out;
//layout (triangles) in;
//layout (triangle_strip, max_vertices=3) out;

uniform float normal_length; 
uniform mat4 g_MVP;

in Vertex
{
  vec4 normal;
  vec4 color;
 
} vin[];


out vec4 vertex_color;
 
void main()
{
	  for(int i = 0; i < gl_in.length(); i++)
	  {
		vec3 P = gl_in[i].gl_Position.xyz;
		vec3 N = vin[i].normal.xyz;
    
		gl_Position = g_MVP * vec4(P, 1.0);
		vertex_color = vin[i].color;
		EmitVertex();
    
		gl_Position = g_MVP * vec4(P + N * normal_length, 1.0);
		vertex_color = vin[i].color;
		EmitVertex(); 
		EndPrimitive();
	   }
	
	
	

}