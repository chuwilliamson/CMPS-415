
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

#include <Windows.h>
#include <ctime>

#include "Assignment_06.h"

#include "ObjLoader.h"

using namespace gmtl;  

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	RenderScene();
	
	glutSwapBuffers();
}

void RenderScene()
{
	if (Time.deltaTime != 0)
	{
		T_c1_w = rLong * rLat  * Translate(zInc, 'z');
		T_c2_w = T_s2_w * rLong * rLat   *  Translate(zInc, 'z');
		T_c3_w = T_s3_w * rLong * rLat  * Translate(zInc, 'z');
		T_c2_o = T_s2_w * flock.BirdAt(0)._T_o_r  * rAzi * rEle * Rotation(90, 'z') * Translate(zInc + 1.5f, 'z');

		flock.Update(dispersion, centering, alignment, Time.deltaTime);
		preds.Update(dispersion*.5f, centering * 2, alignment, Time.deltaTime);
		//rotate the propellars
		T_j2_b *= Rotation(45 * Time.deltaTime * 15, 'y');
		 
	}
	glUseProgram(PROGRAM1);
	DrawGlobe();
	
	DrawTeapot(vao5, vbo5, ibo5, GetSize('t'), World);
 
	for (int i = 0; i < flock._size; i++)
		{
		DrawTeapot(vao5, vbo5, ibo5, GetSize('t'), Translate(.25f, 'y')* Rotation(180, 'z') * Scale(0.5f) * flock.BirdAt(i)._T_o_r * Rotation(-90, 'z')  * Rotation(180, 'y') *  Scale(0.25f));
			DrawBird(flock.BirdAt(i));			
			DrawForce(flock.BirdAt(i));
		}
		for (int i = 0; i < preds._size; i++)
			DrawPred(preds.BirdAt(i));

	glUseProgram(0);
}

void Update()
{
	Time.currentTime = glutGet(GLUT_ELAPSED_TIME);
	Time.deltaTime = (Time.currentTime - Time.previousTime) / CLOCKS_PER_SEC;
	Time.previousTime = Time.currentTime;

	glutPostRedisplay();
}
 
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char** argv)
{
	int width = 1600;
	int height = 900;

	srand(static_cast <unsigned> (time(0)));

	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
	glutInitDisplayMode(GLUT_RGBA);

	//Set the window size/dimensions
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);

	// Specify OpenGL version and core profile.
	// For newer cards, use (3, 3) or (4, 0).
	// For old cards or programming style, may have to remove calls below.
	// However, we only want to use recent versions, so avoid removing if possible!
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Assignment_06");

	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);

	SetupTextures();
	// Set the shaders
	SetupShaders();
	SetMatrices();
	SetPerspective(-.12f, .12f, -.08f, .08f, .1f, 100.0f, Projection);

	//Create geometry
	SetupSphere(20, 20,sphereRadius, vao1, vbo1, ibo1);
	SetupAxis(4, vao2, vbo2, ibo2);
	SetupCone(10, vao3, vbo3, ibo3);
	//SetupTeapot(vao5, vbo5, ibo5);


	//SetupSkybox(25, 25, sphereRadius, vao4, vbo4, ibo4);
	SetupForceVisualization();


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
