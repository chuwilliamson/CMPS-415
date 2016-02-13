// CMPS 415 : Lighting example.
// Shows how to pass normals and parameters to shaders for lighting.
// Uses LoadShaders from Red Book code archive

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#pragma comment (lib, "glew32.lib")
#include <GL/freeglut.h>

#include <gmtl/gmtl.h>
#include <gmtl/Matrix.h>

#include "LoadShaders.h"

using namespace std;

// Vertices for triangles making a sheared cube:
// This representation describes each triangle explicitly
static const GLfloat g_vertex_buffer_data[] = {
	//Front Face
	-0.25f, -0.25f, 0.75f,
	-0.65f, -0.25f, 0.75f,
	-0.65f, -0.65f, 0.75f,
	
	-0.25f, -0.65f, 0.75f,
	-0.25f, -0.25f, 0.75f,
	-0.65f, -0.65f, 0.75f,

	//Back Face
	-0.50f, -0.10f, -0.75f,
	-0.10f, -0.10f, -0.75f,
	-0.50f, -0.50f, -0.75f,

	-0.10f, -0.50f, -0.75f,
	-0.50f, -0.10f, -0.75f,
	-0.50f, -0.50f, -0.75f,

	//Top Face
	-0.25f, -0.25f, 0.75f,
	-0.65f, -0.25f, 0.75f,
	-0.50f, -0.10f, -0.75f,

	-0.25f, -0.25f, 0.75f,
	-0.10f, -0.10f, -0.75f,
	-0.50f, -0.10f, -0.75f,

	//Bottom Face
	-0.25f, -0.65f, 0.75f,
	-0.65f, -0.65f, 0.75f,
	-0.50f, -0.50f, -0.75f,

	-0.35f, -0.65f, 0.75f,
	-0.50f, -0.50f, -0.75f,
	-0.10f, -0.50f, -0.75f,

	//Right Face
	-0.25f, -0.25f, 0.75f,
	-0.10f, -0.10f, -0.75f,
	-0.10f, -0.50f, -0.75f,

	-0.25f, -0.25f, 0.75f,
	-0.25f, -0.65f, 0.75f,
	-0.10f, -0.50f, -0.75f,

	//Left Face
	-0.65f, -0.25f, 0.75f,
	-0.65f, -0.65f, 0.75f,
	-0.50f, -0.50f, -0.75f,

	-0.65f, -0.25f, 0.75f,
	-0.50f, -0.10f, -0.75f,
	-0.50f, -0.50f, -0.75f,
};

// RGB color triples for every coordinate above.
static const GLfloat g_color_buffer_data[] = {
	//Front Face
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	//Back Face
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	//Top Face
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	//Bottom Face
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	//Right Face
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	
	//Left Face
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

// Normals -- these are not really correct for the sheared cube, but OK if unsheared.
static const GLfloat g_normal_buffer_data[] = {
	//Front Face
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,

	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,

	//Back Face
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,

	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,

	//Top Face
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,

	//Bottom Face
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,

	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,

	//Right Face
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,

	//Left Face
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,

	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
};

gmtl::Matrix44f translateMatrix, Matrix;

// Vertex array object
GLuint VertexArrayID;

// Vertex buffer objects
GLuint vertexbuffer;
GLuint colorbuffer;
GLuint normalbuffer;

GLuint MatrixID;

GLuint vertex_position, vertex_color, vertex_normal;


// We will use this parameter to demonstrate passing a vector to the shaders.
// To illustrate, it will act like L from the lighting equation.
GLuint a_parameter_LightID;
gmtl::Vec4f a_parameter_Light_Component;

GLenum errCode;
const GLubyte *errString;

void init(){


	// Enable depth test (visible surface determination)
	glEnable(GL_DEPTH_TEST);

	// OpenGL background color
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] = {{GL_VERTEX_SHADER, "Cube_Vertex_Shader.vert"},
	{GL_FRAGMENT_SHADER, "Cube_Fragment_Shader.frag"},
	{GL_NONE, NULL}};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	//Get the shader parameter locations to connect them to the
	//respective buffers later.
	vertex_position = glGetAttribLocation(program, "vertexPosition");
	vertex_color = glGetAttribLocation(program, "vertexColor");
	vertex_normal = glGetAttribLocation(program, "vertexNormal");

	// initialize a GMTL matrix to identity
	gmtl::identity(Matrix);

	// initialize a GMTL matrix for a translation
	translateMatrix.set(1.0f, 0.0f, 0.0f, 0.75f,
					0.0f, 1.0f, 0.0f, 0.75f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

	// Find locations needed to communicate values to vertex shader
	MatrixID = glGetUniformLocation(program, "Matrix");

	a_parameter_LightID = glGetUniformLocation(program, "a_parameter");

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_position, // Attribute 0. This number must match the layout in the shader
						  3, // Size
						  GL_FLOAT, // Type
						  GL_FALSE, // Is normalized
						  0, 
						  ((void*)0)); // vertex buffer object
	glEnableVertexAttribArray(vertex_position);
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);	
	glVertexAttribPointer(vertex_color, // Attribute 1. This number must match the layout in the shader
						  3, // Size
						  GL_FLOAT, // Type
						  GL_FALSE, // Is normalized
						  0, 
						  ((void*)0)); // vertex buffer object
	glEnableVertexAttribArray(vertex_color);

	// Set up the the normal buffering.
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);	
	glVertexAttribPointer(vertex_normal, // Attribute 1. This number must match the layout in the shader
						  3, // Size
						  GL_FLOAT, // Type
						  GL_FALSE, // Is normalized
						  0, 
						  ((void*)0)); // vertex buffer object
	glEnableVertexAttribArray(vertex_normal);  
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Send our transformation to the shader
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Matrix[0][0]);

	//Send the example parameter to the shader
	// Set a value for the example parameter. To illustrate, we will use first 3 elements like a unit direction.
	a_parameter_Light_Component.set(0.267f, 0.5353f, 0.802f, 1.0f);
	glUniform4f(a_parameter_LightID, a_parameter_Light_Component[0], a_parameter_Light_Component[1], a_parameter_Light_Component[2], a_parameter_Light_Component[3]);

	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &translateMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

	// Swap buffers
	glutSwapBuffers();

	//Check for any errors and print the error string.
	//NOTE: The string returned by gluErrorString must not be altered.
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		cout << "OpengGL Error: "  << errString << endl;
	}
}

//User-defined keyboard handler
void keyboard( unsigned char key, int x, int y ){
	switch ( key ) {
	case 'q': case 'Q': case 033 /* Escape key */:
		exit( EXIT_SUCCESS );
		break;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);

	glutInitDisplayMode( GLUT_RGBA);

	glutInitWindowSize(1024, 1024);

// Specify OpenGL version and core profile.
// For newer cards, use (3, 3) or (4, 0).
// For old cards or programming style, may have to remove calls below.
// However, we only want to use recent versions, so avoid removing if possible!
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("415/515 CUBE DEMO");

	glewExperimental = GL_TRUE;

	if(glewInit())
		exit(EXIT_FAILURE);

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc( keyboard );

	glutMainLoop();

	return 0;
}
