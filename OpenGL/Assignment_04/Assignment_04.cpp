
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>


#include <Windows.h>
#include <ctime>

#include "Geometry.h"
#include "Transforms.h"
#include "Assignment_04.h" 

using namespace gmtl;


void ChangeLights(LightState c)
{
	switch (c)
	{
	case diffuse:
		glUniform1f(hf, 0.0f);
		glUniform1f(df, 1.0f);
		glUniform1f(sf, 0.0f);
		break;
	case specular:
		glUniform1f(hf, 0.0f);
		glUniform1f(df, 0.0f);
		glUniform1f(sf, 1.0f);
		break;
	case hem:
		glUniform1f(hf, 1.0f);
		glUniform1f(df, 0.0f);
		glUniform1f(sf, 0.0f);
		break;
	case full:
		glUniform1f(hf, 1.0f);
		glUniform1f(df, 1.0f);
		glUniform1f(sf, 1.0f);
	default:
		break;
	}
}

void Display()
{	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 
	
	
	
	// Send the example parameter to the shader
	// Set a value for the example parameter. To illustrate, we will use first 3 elements like a unit direction.
	// Bind our texture in Texture Unit 0 
	 
	glActiveTexture(GL_TEXTURE0);
	if(showTex){		
		glUniform1i(texture_color, 0);
		glBindTexture(GL_TEXTURE_2D, sphereTexture);
		DrawObject(vao1, vbo1, ibo1, GetGeometrySize('s'), World * T_s_w * Translate(2.0f, 'x')  );
		DrawObject(vao1, vbo1, ibo1, GetGeometrySize('s'), World * T_s_w * Translate(-2.0f, 'x'));
		glBindTexture(GL_TEXTURE_2D, 0);
	}else{
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		DrawObject(vao1, vbo1, ibo1, GetGeometrySize('s'),  T_s_w);
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	//BIND backgroundTexture
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	DrawCube(vao4, vbo4, World * Translate(.75f, 'y') * Translate(0.0f, 'x') * Translate(-1.5f, 'z') * Scale(0.3f));
	DrawCube(vao4, vbo4, World * Translate(-.75f, 'y') * Translate(0.0f, 'x') * Translate (1.5f, 'z') * Scale(0.3f));
	DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), World * Rotation(90, 'x'));
	DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), World * Scale(1.0f));
	DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), World * Rotation(-90, 'z'));
	
	
		
	
	
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * T_s_w * T_r_w * T_o_r * T_b_o * Scale(.05f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World * T_r_w * T_o_r * T_b_o * Rotation(-90, 'z'));
 
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * Rotation(-155, 'y')* T_r_w * T_o_r * Translate(-.5f, 'z') * T_b_o * Scale(.1f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World *  Rotation(-155, 'y')* T_r_w * T_o_r * Translate(-.5f, 'z') * T_b_o * Rotation(-90, 'z'));
	
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * Rotation(-222, 'z') * T_r_w * T_o_r * Translate(.2f, 'x') * T_b_o * Scale(.1f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World * Rotation(-222, 'z') * T_r_w * T_o_r * Translate(.2f, 'x') * T_b_o * Rotation(-90, 'z'));
	
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * Rotation(125, 'x') * T_r_w * T_o_r * Translate(.5f, 'z') *T_b_o * Scale(.15f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World * Rotation(125, 'x') * T_r_w * T_o_r * Translate(.5f, 'z') *T_b_o * Rotation(-90, 'z'));
	
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * Rotation(-155, 'y') * T_r_w * T_o_r * Translate(-.5f, 'z') * T_b_o * Scale(.1f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World * Rotation(-155, 'y') * T_r_w * T_o_r * Translate(-.5f, 'z') * T_b_o * Rotation(-90, 'z'));
	
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * Rotation(-222, 'z') * T_r_w * T_o_r * Translate(.2f, 'x') * T_b_o * Scale(.13f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World * Rotation(-222, 'z') * T_r_w * T_o_r * Translate(.2f, 'x') * T_b_o * Rotation(-90, 'z'));
	
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * Rotation(100, 'x') * T_r_w * T_o_r * Translate(.75f, 'x') *T_b_o * Scale(.14f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World * Rotation(100, 'x') * T_r_w * T_o_r * Translate(.75f, 'x') *T_b_o * Rotation(-90, 'z'));
	
	DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), World * Rotation(-135, 'y') * T_r_w * T_o_r * Translate(-.75f, 'x') * T_b_o * Scale(.1f));
	DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), World * Rotation(-135, 'y') * T_r_w * T_o_r * Translate(-.75f, 'x') * T_b_o * Rotation(-90, 'z'));
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//UNBIND
	// Swap Buffers
	glutSwapBuffers();
	
	// Check for errors
	CheckError();

}

void Update()
{
	
	currentTime = (float)glutGet(GLUT_ELAPSED_TIME);	
	deltaTime = (currentTime - previousTime) /CLOCKS_PER_SEC;
	previousTime = currentTime;
	if(animate)
	{
		if (deltaTime != 0)
		{
			qY = make<gmtl::Quatf>(Rotation(15 * deltaTime, 'y'));
			qZ = make<gmtl::Quatf>(Rotation((-35 * deltaTime), 'z'));
			q = q * qZ;
			//std::cout << qZ << std::endl;
			q = makeNormal(q);
			qm = make<Matrix44f>(q);
			//transform r with respect to w set to the quaternion matrix rotation
			T_r_w = World * qm;
			glutPostRedisplay();
		}
	}
	
	T_c1_w =  World * rLong * rLat * Translate(zInc, 'z');
	T_c2_o =  World * T_r_w * T_o_r * rAzi * rEle * Rotation(-90, 'z') * Translate(zInc, 'z');
	
}

void Keyboard(unsigned char key, int x, int y)
{	
#pragma region Update
	switch (key)
	{

	case 033 /* Escape key */:
		exit(EXIT_SUCCESS);
		break;
	case 'w':
		//T_s_w = Translate(1.0f, 'y');
		World = Translate(.10f, 'y');
		break;
	case 'a':
		World = Translate(-.10f, 'x');
		break;
	case 's':
		World = Translate(-.10f, 'y');
		break;
	case 'd':
		World= Translate(.10f, 'x');
		break;
	case 'r':
		T_s_w *= Rotation(5, 'y');
		break;
	case 'e':
		T_s_w *= Rotation(-5, 'y');
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
	case '3':
		showTex = !showTex;
		glutPostRedisplay();
		break;
	case 'f':
		animate = !animate;
		break;
	case '4':
		ChangeLights(diffuse);
		break;
	case '5':
		ChangeLights(specular);
		break;
	case '6':
		ChangeLights(hem);
		break;
	case '7':
		ChangeLights(full);
		break;
	case 'z':
		std::cout<<std::endl<<view				
				 //<<std::endl<<lightPos
				 <<std::endl;


	}
#pragma endregion
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
		zInc += .05f;//positive z means camera moves toward user becuase of right hand rule
		glutPostRedisplay();
	}
	else if (button == 3)
	{
		zInc -= .05f;
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
			
			T_c1_w = World * rLong * rLat * Translate(zInc, 'z');
		}
		else if (cam2)
		{
			T_c2_o = World * T_r_w * T_o_r * rAzi * rEle * Rotation(-90, 'z') * Translate(zInc, 'z');
		}
	}
	
	//reset the oldX and Y values to prevent the sphere from resetting
	//itself after clicking a new click event
	oldX = x; oldY = y;
	
	//glutPostRedisplay();
	
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{	
	int width = 1920;
	int height = 1080;	
	
	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
	glutInitDisplayMode(GLUT_RGBA);

	//Set the window size/dimensions
	glutInitWindowSize(1920/2, 1080/2);
	glutInitWindowPosition(300, 300);

	// Specify OpenGL version and core profile.
	// For newer cards, use (3, 3) or (4, 0).
	// For old cards or programming style, may have to remove calls below.
	// However, we only want to use recent versions, so avoid removing if possible!
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Assignment_04");

	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);

	// Set the shaders
	SetupShaders();

	// Set transformation matrices
	SetMatrices();

	CreateCube(vao4, vbo4);
	//Create geometry	
	CreateSphere(40, 30, sphereRadius, vao1, vbo1, ibo1);
	CreateAxis(4, vao2, vbo2, ibo2);	
	CreateCone(4, vao3, vbo3, ibo3);
	//Geometry cube();
	//cube.draw;
	//cube.
	
	// Register a callback with glut that will handle drawing to the screen
	glutDisplayFunc(Display);

	// Register a callback with glut that will handle keypresses
	glutKeyboardFunc(Keyboard);	
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(Update);
	glutReshapeFunc(Reshape);
	
	//Transfer the control to glut processing loop.
	glutMainLoop();
	
	return 0;
}
