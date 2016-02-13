
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

#include <Windows.h>
#include <ctime>

#include "Assignment_05.h"


using namespace gmtl;

void Display()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderScene();
	glutSwapBuffers();

	// Check for errors
	CheckError();
}
void Update()
{
	Time.currentTime = glutGet(GLUT_ELAPSED_TIME);
	Time.deltaTime = (Time.currentTime - Time.previousTime) / CLOCKS_PER_SEC;
	Time.previousTime = Time.currentTime;

	if (Time.deltaTime > 0)
	{
		for (int i = 0; i < flock.Size(); i++)
		{
			int birdID = i;
			flock.birds.at(i).RotateBird(birdID, flock.birds, Time.deltaTime);
			//std::cout<<flock.birds.at(0).influence<<'\r';
		}

		

		//rotate the propellars
		T_j2_b *= Rotation(45 * Time.deltaTime * 15, 'y');	

		glutPostRedisplay();
	}
}

void RenderScene()
{
	DrawGlobe();
	T_c1_w = rLong * rLat  * Translate(zInc, 'z');
	for (int i = 0; i < flock.Size(); i++)
	{
		//draw each bird with the updated transformation matrix
		DrawBird(flock.birds.at(i).Transform(), flock.birds.at(i).GetMass());
	}
	T_c2_o = flock.birds.at(0).Transform() * rAzi * rEle  * Rotation(-90, 'z') * Translate(zInc, 'z');
}


void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
	int width = 1600;
	int height = 900;

	srand (static_cast <unsigned> (time(0)));

	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
	glutInitDisplayMode(GLUT_RGBA);

	//Set the window size/dimensions
	glutInitWindowSize(width / 2, height / 2);
	glutInitWindowPosition(300, 300);

	// Specify OpenGL version and core profile.
	// For newer cards, use (3, 3) or (4, 0).
	// For old cards or programming style, may have to remove calls below.
	// However, we only want to use recent versions, so avoid removing if possible!
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Assignment_05");

	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);

	SetupTextures();
	// Set the shaders
	SetupShaders();

	SetPerspective(-.12f, .12f, -.08f, .08f, .1f, 100.0f, Projection);

	//Create geometry	
	SetupSphere(25, 25, sphereRadius, vao1, vbo1, ibo1);
	SetupAxis(4, vao2, vbo2, ibo2);
	SetupCone(10, vao3, vbo3, ibo3);

	//Setup Birds

	flock.Create(11);

	// Register a callback with glut that will handle drawing to the screen
	glutDisplayFunc(Display);

	// Register a callback with glut that will handle keypresses
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(Update);


	//Transfer the control to glut processing loop.
	glutMainLoop();

	return 0;
}

















