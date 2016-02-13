// CMPS 415/515 : Texture example. Draws a texture mapped 
// Uses LoadShaders from Red Book code archive

// Include standard headers
#include <iostream>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/freeglut.h>

#include <gmtl/gmtl.h>
#include <gmtl/Matrix.h>

#include "LoadShaders.h"

#include "Texture.h"


using namespace std;

// Our vertices. Three consecutive floats give a 3D vertex; 
// A quad has 1 face.
static const GLfloat g_vertex_buffer_data[] = {
	-0.5f, -0.5f, 0.75f, 
	0.5f, -0.5f, 0.75f,
	0.5f, 0.5f, 0.75f, 
	-0.5f, 0.5f, 0.75f
};

// Two UV coordinatesfor each vertex. 
static const GLfloat g_uv_buffer_data[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f	
};

GLuint program;
GLuint vertexbuffer, uvbuffer;
GLuint VertexArrayID;
GLuint MatrixID, TextureID;

//Our Model-View and Projection Matrix
//To keep it simple, we are using using any camera
//and projections.
gmtl::Matrix44f Matrix;

GLenum errCode;
const GLubyte *errString;

void init(){
	GLuint vertex_position, vertex_UV;

	unsigned int texwidth, texheight;
	unsigned char *imagedata;

	glClearColor(0.0f, 1.0f, 1.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] = {{GL_VERTEX_SHADER, "vertex_shader.vert"},
	{GL_FRAGMENT_SHADER, "fragment_shader.frag"},
	{GL_NONE, NULL}};

	program = LoadShaders(shaders);
	glUseProgram(program);

	// initialize the gmtl matrix to identity
	gmtl::identity(Matrix);

	Matrix[2][2] = -1;

	// Get a handle for our "Matrix" uniform
	MatrixID = glGetUniformLocation(program, "Matrix");

	//Get the shader parameter locations to connect them to the
	//respective buffers later.
	vertex_position = glGetAttribLocation(program, "vertexPosition");
	vertex_UV = glGetAttribLocation(program, "vertexUV");

	// get new vertex array identifier (number) and generate/activate the object
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	// Get new vertex buffer objects for both geometry and color -- 
	//generate,activate buffers, and transfer in data
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertex_position);
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertex_UV);
	glVertexAttribPointer(vertex_UV, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//Load the image from the ppm file.
	LoadPPM("marbles2.ppm", &texwidth, &texheight, &imagedata, 1);

	cout << "TEXTURE IMAGE WIDTH: " << texwidth << "\nTEXTURE IMAGE HEIGHT: "  << texheight << endl;

	//Generate a texture
	glGenTextures(1, &TextureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, texheight, texwidth, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Get a handle for our "texture_Colors" uniform
	TextureID  = glGetUniformLocation(program, "texture_Colors");
}

void display(){
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Send our transformation to the currently bound shader,
	// in the "Matrix" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Matrix[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	
	// Set our "texture_Colors" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	//Bind the vertex array object created in the init() and
	//it will encapsulate the attribute pointers from the init()
	glBindVertexArray(VertexArrayID);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4*3); // 12*3 indices starting at 0 -> 12 triangles

	// Swap buffers
	glutSwapBuffers();

	//Check for any errors and print the error string.
	//NOTE: The string returned by gluErrorString must not be altered.
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
		cout << "OpengGL Error: "  << errString << endl;
	}
}

void keyboard( unsigned char key, int x, int y ){
	switch ( key ) {
	case 'q': case 'Q': case 033 /* Escape key */:
		exit( EXIT_SUCCESS );
		break;
	}
}

int main( int argc, char** argv ){
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA);
	glutInitWindowSize(1024, 1024);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("TEXTURE EXAMPLE");

	glewExperimental = GL_TRUE;

	if(glewInit())
		exit(EXIT_FAILURE);

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(program);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	system("PAUSE");
	return 0;
}
