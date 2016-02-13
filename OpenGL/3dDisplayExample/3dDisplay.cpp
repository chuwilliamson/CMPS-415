//|____________________________________________________________________
//| CMPS 415/515 University of Louisiana at Lafayette 
//| NOTE: Copyright 2011 
//| No permission for distribution beyond 415/515
//| --------------------------------------------------------------
//| OpenGL quad-buffered stereo example.
//|
//| Comment "#define USE_DEVELOPMENT_MODE" to run on the 3d display,
//| with quad-buffered stereo and fullscreen rendering.
//|
//| Uncomment "#define USE_DEVELOPMENT_MODE" to run in development mode,
//| without stereo and using 960x540 window.
//| 
//| Press ESC to exit.
//| Press 's' to switch between left- and right-eye view in development mode.
//|____________________________________________________________________

#include <stdio.h>
#include <GL/glut.h> 

#define USE_DEVELOPMENT_MODE                          // Comment to run on 3d projection display

// Frustum with our display geometry in meters.
// NOTE: KEEP THIS FRUSTUM SHAPE TO MATCH THE PROJECTION DISPLAY,
// IN COMBINATION WITH OUR glFrustum CALL AND EYE TRANSLATION.
// ONLY THE FOLLOWING CHANGES PRESERVE SHAPE:
// VF_NEAR and VF_FAR can be changed safely due to a fix in our Frustum call.
// The frustum can be scaled if all the following 6 values are scaled
// by same amount: IPD_D2, VF_FD, VF_LEFT, VF_RIGHT, VF_BOTTOM, VF_TOP
enum Eye { LEFT_EYE, RIGHT_EYE };
const float IPD_D2 = 0.07f / 2;       // Interpupillary distance divided by 2
const float VF_NEAR = 1;             // Distance to near clipping plane
const float VF_FAR = 100;           // Distance to far clipping plane
const float VF_FD = 2.5;           // Distance to focal plane (focal depth)
const float VF_LEFT = -2.05;          // Left clipping coordinate at focal depth
const float VF_RIGHT = 2.05;          // Right clipping coordinate at focal depth
const float VF_BOTTOM = -1.6;           // Bottom clipping coordinate at focal depth
const float VF_TOP = 0.68;          // Top clipping coordinate at focal depth
const float VF_S = VF_NEAR / VF_FD;  // Scale factor

//|________________________________
bool draw_left_eye = true;           // Draw left-eye image or right-eye image toggled by the key 's'

void displayFunc();
void drawScene(int eye);
void init();
void reshapeFunc(int width, int height);
void keyboardFunc(unsigned char key, int x, int y);

//|________________________________

int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
#ifdef USE_DEVELOPMENT_MODE     // Enable quad-buffered stereo only when requested
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO);
#endif
	glutInitWindowSize(960, 540);
	glutCreateWindow("Quad-Buffered Stereo Example");

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboardFunc);

#ifndef USE_DEVELOPMENT_MODE    // Enable full screen rendering only in stereo mode
	glutFullScreen();
#endif

	init();

	glutMainLoop();

	return 0;
}

void displayFunc()
{
#ifdef USE_DEVELOPMENT_MODE   // Draw left- or right-eye image toggled by 's'
	drawScene(draw_left_eye ? LEFT_EYE : RIGHT_EYE);
#else                         // Draw both eye images for stereo rendering
	drawScene(LEFT_EYE);
	drawScene(RIGHT_EYE);
#endif

	glutSwapBuffers();
}

void drawScene(int eye)
{
	// Specify a color buffer to draw to
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (eye == LEFT_EYE)
	{
		glFrustum((VF_LEFT + IPD_D2)*VF_S, (VF_RIGHT + IPD_D2)*VF_S, VF_BOTTOM*VF_S, VF_TOP*VF_S, VF_NEAR, VF_FAR);
	}
	else
	{
		glFrustum((VF_LEFT - IPD_D2)*VF_S, (VF_RIGHT - IPD_D2)*VF_S, VF_BOTTOM*VF_S, VF_TOP*VF_S, VF_NEAR, VF_FAR);
	}

	// EYE TRANSLATION MUST GO IN VIEW TRANSFORM FIRST, TO SEPARATE THE EYES
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (eye == LEFT_EYE)
	{         // left/right eye shift for 3D viewing
		glTranslatef(IPD_D2, 0, 0);
	}
	else
	{
		glTranslatef(-IPD_D2, 0, 0);
	}

	// Replace items below with your usual view and model components
	gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0); // usual view transform goes here

	static const GLfloat LIGHT_POS[] = { 2.5f, 2.5f, 2.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POS);

	static const GLfloat MAT_OBJECT[] = { 1.0f, 0.37f, 0.11f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, MAT_OBJECT);
	glutSolidTeapot(.5f);
	glTranslatef(0.75f, -1.50f, -6.0f);
	glutSolidSphere(1.0f, 32, 32);
}

void init()
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:       // ESC exits
		exit(0);
		break;
	case 's':      // [Development mode only] 's' toggles between left- and right-eye image
		draw_left_eye = !draw_left_eye;
		printf("Showing image for [%s]\n", draw_left_eye ? "Left Eye" : "Right Eye");
		glutPostRedisplay();
		break;
	}
}