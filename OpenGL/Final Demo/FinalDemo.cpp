
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

#include <Windows.h>
#include <ctime>

#include "FinalDemo.h"

#include "ObjLoader.h"

using namespace gmtl;

#define USE_DEVELOPMENT_MODE                          // Comment to run on 3d projection display


void Display()
{
	glActiveTexture(GL_TEXTURE0);
#ifdef USE_DEVELOPMENT_MODE   // Draw left- or right-eye image toggled by 's'
	RenderScene(draw_left_eye ? LEFT_EYE : RIGHT_EYE);
#else                         // Draw both eye images for stereo rendering
	RenderScene(LEFT_EYE);
	RenderScene(RIGHT_EYE);
#endif
	glutSwapBuffers();
}

void RenderScene(int eye)
{
#ifdef USE_DEVELOPMENT_MODE
	glDrawBuffer(GL_BACK);
#else
	if (eye == LEFT_EYE)
	{
		glDrawBuffer(GL_BACK_LEFT);
	}
	else
	{
		glDrawBuffer(GL_BACK_RIGHT);
	}
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// USE THIS VIEW FRUSTUM SETUP FOR THE PROJECTION DISPLAY
	if (eye == LEFT_EYE)
	{
		//glFrustum((VF_LEFT + IPD_D2) * VF_S, (VF_RIGHT + IPD_D2) * VF_S, VF_BOTTOM * VF_S, VF_TOP * VF_S, VF_NEAR, VF_FAR);
		//SetProjection(-.12f, .12f, -.08f, .08f, .1f, 100.0f, Projection);
		SetProjection(
			(VF_LEFT + IPD_D2) * (fovFac * VF_S),
			(VF_RIGHT + IPD_D2) *(fovFac * VF_S),
			VF_BOTTOM * (fovFac * VF_S),
			VF_TOP * (fovFac * VF_S),
			VF_NEAR,
			VF_FAR,
			Projection);
	}
	else
	{
		//glFrustum((VF_LEFT - IPD_D2) * VF_S, (VF_RIGHT - IPD_D2) * VF_S, VF_BOTTOM * VF_S, VF_TOP * VF_S, VF_NEAR, VF_FAR);
		//SetProjection(-.12f, .12f, -.08f, .08f, .1f, 100.0f, Projection);
		SetProjection(
			(VF_LEFT - IPD_D2) * (fovFac * VF_S),
			(VF_RIGHT - IPD_D2) *(fovFac * VF_S),
			VF_BOTTOM * (fovFac * VF_S),
			VF_TOP * (fovFac * VF_S),
			VF_NEAR,
			VF_FAR,
			Projection);
	}

	// EYE TRANSLATION MUST GO IN VIEW TRANSFORM FIRST, TO SEPARATE THE EYES
	if (eye == LEFT_EYE)
	{	// left/right eye shift for 3D viewing
		//glTranslatef(IPD_D2, 0, 0);
		T_c1_w = Translate(IPD_D2, 'x') * rLong * rLat  * Translate(zInc, 'z');
		T_c2_w = Translate(IPD_D2, 'x') * T_s2_w * rLong * rLat   *  Translate(zInc, 'z');
		T_c3_w = Translate(IPD_D2, 'x') * T_s3_w * rLong * rLat  * Translate(zInc, 'z');
		T_c2_o = Translate(IPD_D2, 'x') * T_s2_w * flock.BirdAt(0)._T_o_r  * rAzi * rEle * Rotation(-90, 'z')  * Translate(zInc2 + 2, 'z');

	}
	else
	{
		//glTranslatef(-IPD_D2, 0, 0);
		T_c1_w = Translate(-IPD_D2, 'x') * rLong * rLat  * Translate(zInc, 'z');
		T_c2_w = Translate(-IPD_D2, 'x') * T_s2_w * rLong * rLat   *  Translate(zInc, 'z');
		T_c3_w = Translate(-IPD_D2, 'x') * T_s3_w * rLong * rLat  * Translate(zInc, 'z');
		T_c2_o = Translate(-IPD_D2, 'x') * T_s2_w * flock.BirdAt(0)._T_o_r  * rAzi * rEle * Rotation(-90, 'z')  * Translate(zInc2 + 2, 'z');

	}

	glUseProgram(PROGRAM3);
	DrawSkybox(starTex, GetCamera() * Scale(50.0f));

	/*glUseProgram(PROGRAM2);*/
	DrawTeapot(backgroundTexture, T_s3_w * Translate(-4, 'y'));
	DrawGlobe(backgroundTexture, T_s3_w);
	glUseProgram(PROGRAM1);
	DrawTeapot(backgroundTexture, T_teaBase_w);
	glUseProgram(PROGRAM1);
	
	DrawGlobe(futureTex, T_s2_w);
	DrawGlobe(checkerboardTexture, T_s3_w);
	DrawTeapot(backgroundTexture, T_s3_w * Translate(-4, 'y'));
	glUseProgram(PROGRAM1);

	for (int i = 1; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
 
			DrawCube(backgroundTexture, T_s3_w *  Translate(i * -5, 'z') * Translate(j * -5, 'x'));
			DrawTeapot(backgroundTexture, T_s3_w * Translate(i * -5, 'z') * Translate(j * -5, 'x') * Translate(1, 'y'));
			DrawGlobe(backgroundTexture, T_s3_w * Translate(i * -5, 'x'));
			DrawGlobe(backgroundTexture, T_s3_w * Translate(i * 5, 'x'));

			DrawCube(backgroundTexture, T_s3_w *  Translate(i * 5, 'z') * Translate(j * -5, 'x'));			
			DrawTeapot(backgroundTexture, T_s3_w * Translate(i * 5, 'z') * Translate(j * -5, 'x') * Translate(1, 'y'));

			DrawCube(backgroundTexture, T_s3_w *  Translate(i * 5, 'y') * Translate(j * -5, 'x'));
			DrawCube(backgroundTexture, T_s3_w *  Translate(i * -5, 'y') * Translate(j * -5, 'x'));
			

			DrawCube(backgroundTexture, T_s2_w *  Translate(i * -5, 'z') * Translate(j * 5, 'x'));
			DrawTeapot(backgroundTexture, T_s2_w * Translate(i * -5, 'z') * Translate(j * 5, 'x') * Translate(1, 'y'));
			
			DrawCube(backgroundTexture, T_s2_w *  Translate(i * 5, 'z') * Translate(j * 5, 'x'));
			DrawTeapot(backgroundTexture, T_s2_w * Translate(i * 5, 'z') * Translate(j * 5, 'x') * Translate(1, 'y'));
			//bottom then right
			DrawCube(backgroundTexture, T_s2_w *  Translate(i * 5, 'y') * Translate(j * 5, 'x'));
			DrawCube(backgroundTexture, T_s2_w *  Translate(i * -5, 'y') * Translate(j * 5, 'x'));

			DrawTeapot(woodTexture, World * Translate(5 * i, 'z') * Rotation(-90, 'y'));
			DrawTeapot(woodTexture, World * Translate(-5 * i, 'z') * Rotation(90, 'y'));
			DrawTeapot(futureTex, World * Translate(-5 * i, 'y') * Rotation(90, 'y'));
			DrawTeapot(futureTex, World * Translate(5 * i, 'y') * Rotation(90, 'y'));

			
		}
		

		
	}
	
 
 
	for (int i = 0; i < flock._size; i++)
	{ 
		DrawTeapot(backgroundTexture, T_tea1_w * flock.BirdAt(i)._T_o_r * T_tea1_f);		
		DrawBird(futureTex, flock.BirdAt(i));
		DrawForce(flock.BirdAt(i));
	}
	for (int i = 0; i < preds._size; i++)
		DrawPred(checkerboardTexture, preds.BirdAt(i));
	
}

void Update()
{
	Time.currentTime = glutGet(GLUT_ELAPSED_TIME);
	Time.deltaTime = (Time.currentTime - Time.previousTime) / CLOCKS_PER_SEC;
	Time.previousTime = Time.currentTime;

	if (Time.deltaTime != 0)
	{
		
		//update the flock
		flock.Update(dispersion, centering, alignment, Time.deltaTime);
		preds.Update(dispersion*.5f, centering * 2, alignment, Time.deltaTime);
		//rotate the propellars
		T_j2_b *= Rotation(45 * Time.deltaTime * 15, 'y');

	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
 
	srand(static_cast <unsigned> (time(0)));

	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
#ifdef USE_DEVELOPMENT_MODE     // Enable quad-buffered stereo only when requested
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO);
#endif
	//Set the window size/dimensions
	glutInitWindowSize(960, 540);
	glutInitWindowPosition(100, 600);
	glutCreateWindow("Final Demo");
	// Specify OpenGL version and core profile.
	// For newer cards, use (3, 3) or (4, 0).
	// For old cards or programming style, may have to remove calls below.
	// However, we only want to use recent versions, so avoid removing if possible!
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	 
	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);


	glutDisplayFunc(Display); 
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutIdleFunc(Update);
	

#ifndef USE_DEVELOPMENT_MODE    // Enable full screen rendering only in stereo mode
	glutFullScreen();
#endif 
	init(); 
	//Transfer the control to glut processing loop.
	glutMainLoop();

	return 0;
}

void init()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
	// Enable depth test (visible surface determination)
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	// OpenGL background color
	

	// Set the shaders
	SetupShaders();
	SetupTextures();
	SetMatrices();

	//Create geometry	 
	SetupSphere(15, 25, sphereRadius, vao1, vbo1, ibo1);
	SetupAxis(40, vao2, vbo2, ibo2);
	SetupCone(100, vao3, vbo3, ibo3);

	char* a = "models/teapot2.obj"; //3241 verts
	char* b = "models/teapot.obj"; //3241 verts
	char* c = "models/cube.obj";
	char* d = "models/teapot-low.obj"; //137 verts	 
	char* s = "models/sphere.obj";

	SetSize('t', SetupOBJ(a, vao5, vbo5, ibo5));
	SetSize('b', SetupOBJ(c, vao6, vbo6, ibo6)); 
	SetSize('m', SetupOBJ(s, vao7, vbo7, ibo7));
	SetupForceVisualization();

}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}
