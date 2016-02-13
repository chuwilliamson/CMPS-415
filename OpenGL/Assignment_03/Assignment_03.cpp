
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

#include "LoadShaders.h"
#include "Assignment_03.h"
#include <Windows.h>
#include <ctime>

using namespace gmtl;
float currentTime = 0;
float previousTime = 0;
float deltaTime = 0;
Quatf q,qYaw;
Matrix44f qm, yaw;
void SetupShaders()
{
	// Enable depth test (visible surface determination)
	glEnable(GL_DEPTH_TEST);

	// OpenGL background color
	glClearColor(0.50f, 0.50f, 0.50f, 0.50f);

	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "Cube_Vertex_Shader.vert" },
		{ GL_FRAGMENT_SHADER, "Cube_Fragment_Shader.frag" },
		{ GL_NONE, NULL }
	};

	unsigned int program = LoadShaders(shaders);
	glUseProgram(program);

	//Get the shader parameter locations to connect them to the
	//respective buffers later.
	vertex_position = glGetAttribLocation(program, "vertexPosition");
	vertex_color = glGetAttribLocation(program, "vertexColor");

	// Find location needed to communicate value to vertex shader
	MatrixID = glGetUniformLocation(program, "Matrix");	
}

void SetProjection(float width, float depth, gmtl::Matrix44f &p)
{
	p.set(2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / width, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f / depth, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void SetMatrices()
{
	// initialize a GMTL matrix to identity
	gmtl::identity(M_world);

	T_s_w = gmtl::identity(T_s_w);	
	T_c1_w = gmtl::invert(M_world);
	T_r_w = Rotation(45, 'z');
	T_o_r = Translate(sphereRadius + .35f, 'y');
	T_b_o = gmtl::identity(T_b_o);
	T_c2_o = Translate(.5f, 'z');
	//T_j1_o =
	//T_j2_o =
	
	//T_other_j1 =
	//T_other_j2 =
	
	
	//camera_2 = T_c2_o * Rotation(-90, 'z');
	SetProjection(width, depth, projection);

}



void DrawObject(unsigned int &v, unsigned int &b, unsigned int &i, int indexSize, gmtl::Matrix44f transform)
{
	gmtl::Matrix44f view;
	gmtl::Matrix44f trans;
	
	T_c2_o = T_r_w * T_o_r * rAzi * rEle * Rotation(-90, 'z');;
	if (cam1)
	{	
		view = T_c1_w;
	}
	else if (cam2)
	{		
		view = T_c2_o;
	}
	gmtl::invert(view);
	trans = projection * view * transform;
	float *pvm = &trans[0][0];

	// Bind the vertex array for the object
	glBindVertexArray(v);
	// Bind the buffer array for the object
	glBindBuffer(GL_ARRAY_BUFFER, b);
	// Bind the index buffer for the object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
	// Send the transformation to the shader
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, pvm);
	// Initialize primitive restarts
	glEnable(GL_PRIMITIVE_RESTART); 
	// Set the primitive restart index
	glPrimitiveRestartIndex(0xFFFF);
	// Draw the object
	glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)0);	
}

void Display()
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (showSphere)
	{
		// Draw objects
		DrawObject(vao1, vbo1, ibo1, sphereIndexSize, T_s_w);
		if (showAxis)
		{
		
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_s_w * Rotation(90, 'x'));
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_s_w);
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_s_w * Rotation(-90, 'z'));
			//axis related to the r frame
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_r_w * Rotation(90, 'x'));
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_r_w);
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_r_w * Rotation(-90, 'z'));
			//axis related to the o frame
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_r_w * T_o_r  * Rotation(90, 'x') * Scale(0.25f));
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_r_w * T_o_r  * Scale(0.25f));
			DrawObject(vao2, vbo2, ibo2, axisIndexSize, T_r_w * T_o_r  * Rotation(-90, 'z') * Scale(0.25f));

		}
	}

	DrawObject(vao1, vbo1, ibo1, sphereIndexSize, T_r_w * T_o_r * T_b_o * Scale(.25f));
	DrawObject(vao3, vbo3, ibo3, coneIndexSize, T_r_w * T_o_r * T_b_o);
	//DrawObject(vao3, vbo3, ibo3, coneIndexSize, T_r_w * T_o_r * T_b_o * Translate(.25f, 'y') * Scale(.25f));
	
	// Swap Buffers
	glutSwapBuffers();
	
	// Check for errors
	CheckError();

}

void Update()
{
	
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	
	deltaTime = (currentTime - previousTime) /CLOCKS_PER_SEC;
	previousTime = currentTime;
	qX = make<gmtl::Quatf>(Rotation(-15 * deltaTime, 'x'));
	qY = make<gmtl::Quatf>(Rotation(-15 * deltaTime, 'y'));
	qZ = make<gmtl::Quatf>(Rotation((-15 * deltaTime), 'z'));
	q = q * qZ;
	//std::cout << qZ << std::endl;
	q = makeNormal(q);
	qm = make<Matrix44f>(q);
	//transform r with respect to w set to the quaternion matrix rotation
	T_r_w =  qm;
	T_c2_o = T_r_w * T_o_r * rAzi * rEle * Rotation(-90, 'z');;
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{
#pragma region Update
		
	case 'e': 					
		//glutPostRedisplay();
		break;
	case 'r':
		//yaw						
		//glutPostRedisplay();
		break; 
	case 't':
		//bank		
		glutPostRedisplay();
		break; 
	case 'a':		
		if (showSphere)
		{
			showAxis = !showAxis;
			glutPostRedisplay();
		}
		break;
	case 'S':	
		showSphere = !showSphere;
		glutPostRedisplay();
		break;
	case 's':		
		break;
	case 'D':		
		depth++;
		SetProjection(width, depth, projection);
		glutPostRedisplay();
		break;
	case 'd':		
		depth--;
		SetProjection(width, depth, projection);
		glutPostRedisplay();
		break;
	case '1':
		cam1 = true;
		cam2 = false;
		glutPostRedisplay();
		break;
	case '2':
		cam1 = false;
		cam2 = true;
		glutPostRedisplay();
		break;
#pragma endregion

	}
}


void Mouse(int button, int state, int x, int y)
{	
	mouseMoving = false;
	if (state == GLUT_LEFT_BUTTON) 
	{
		oldX = x;
		oldY = y;
		mouseMoving = true;		
	}
	if (button == 4)
	{
		width += .05f;
		SetProjection(width, depth, projection);
		glutPostRedisplay();
	}
	else if (button == 3)
	{
		width -= .05f;
		SetProjection(width, depth, projection);
		glutPostRedisplay();
	}
}

void Motion(int x, int y)
{
	
	
	if (mouseMoving)
	{
		//get the change in x since mouse down and increment
		mouse_long += (x - oldX);
		rLong = Rotation(mouse_long, 'y');
		mouse_azi += (x - oldX);
		//rAzi is x rotation by azi
		rAzi = Rotation(mouse_azi, 'x');

		//get the change in y since mouse down and increment
		mouse_lat += (y - oldY);
		rLat = Rotation(mouse_lat, 'x');
		
		mouse_ele += (y - oldY);
		//rEle is y rotation by ele
		rEle = Rotation(mouse_ele, 'y');
		
		
		
		//Rlat is x rotation by lat and Rlong is y rotation by long
		//increment long according to horizontal mouse movement and lat according to vertical motion
		if (cam1)
		{//r_long * r_lat where r_lat is x-rotation by lat and
			//r_long is y rotation by long
			//increment long according to 
			
			T_c1_w = rLong * rLat;
		}
		else if (cam2)
		{
			T_c2_o = T_r_w * T_o_r * rAzi * rEle * Rotation(-90, 'z');;
		}
	}
	
	//reset the oldX and Y values to prevent the sphere from resetting
	//itself after clicking a new click event
	oldX = x;
	oldY = y;
	
	//glutPostRedisplay();
	
}

int main(int argc, char** argv)
{
	
	vector<float> sphere_vertices, axis_vertices, cone_vertices;
	vector<unsigned int> sphere_indices, axis_indices, cone_indices;
	vector<float> sphere_color_data, axis_color_data, cone_color_data;

	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
	glutInitDisplayMode(GLUT_RGBA);

	//Set the window size/dimensions
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(300, 300);

	// Specify OpenGL version and core profile.
	// For newer cards, use (3, 3) or (4, 0).
	// For old cards or programming style, may have to remove calls below.
	// However, we only want to use recent versions, so avoid removing if possible!
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Assignment_03");

	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);

	// Set the shaders
	SetupShaders();

	// Set transformation matrices
	SetMatrices();

	// Create geometry
	CreateSphere(3, 3, sphere_vertices, sphere_indices, sphere_color_data, sphereRadius,vao1, vbo1, ibo1);
	CreateAxis(25, axis_vertices, axis_indices, axis_color_data, vao2, vbo2, ibo2);
	CreateCone(25, cone_vertices, cone_indices, cone_color_data, vao3, vbo3, ibo3);
	
	// Register a callback with glut that will handle drawing to the screen
	glutDisplayFunc(Display);

	// Register a callback with glut that will handle keypresses
	glutKeyboardFunc(Keyboard);	
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(Update);
	
	
	
	//Transfer the control to glut processing loop.
	glutMainLoop();
	
	return 0;
}
