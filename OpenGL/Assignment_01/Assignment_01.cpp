#include <stdio.h> 
#include <GL/glut.h> 
#include <math.h> 
#define WIDTH 1200	 
#define HEIGHT 800
static GLubyte frame_buffer[HEIGHT][WIDTH][3];
/* frame_buffer simulates a frame buffer with 3 bytes per pixel (24 bit RGB). The first index of "frame_buffer" refers to the y-coordinate (0 to HEIGHT-1).
The second index of "frame_buffer" refers to the x-coordinate (0 to WIDTH-1). The third index of "frame_buffer" selects the R, G, or B byte. Each element has value 0 to 255, with 0 being
minimum intensity and 255 being max. Example [setting the pixel at (100,10) to red]: frame_buffer[10][100][0] = 255; frame_buffer[10][100][1] = 0; frame_buffer[10][100][2] = 0;
General Coding and Submission Requirements can be found on Moodle. */
struct Point2D { int x; int y; int r; int g; int b; };
struct Line2D { Point2D Point1; Point2D Point2; };

void DrawLine(Point2D first, Point2D second, int color);
void DrawPoint(int x, int y, int r, int g, int b);
void DrawTriangle(Point2D first, Point2D second, Point2D third);
float CheckForZero(int x1, int x2, int y1, int y2);
void displayHandler(void)
{
	glRasterPos2i(-1, -1); //Specifies the raster position for pixel operations. //Set the raster position to the lower-left corner to avoid a problem
	//(with glDrawPixels) when the window is resized to smaller dimensions. 
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer); //glDrawPixels — write a block of pixels to the frame buffer
	//Write the information stored in "frame_buffer" to the color buffer
	glFlush();
}
void mouseHandler(int button, int state, int x, int y)
{
	static int clicks = 0;
	static Point2D firstPoint;
	static Point2D secondPoint;
	static Point2D thirdPoint;
	Point2D mouseClick;
	mouseClick.x = x;
	mouseClick.y = HEIGHT - y;
	switch (button)//switch on if the button was pressed down or up		
	{
	case GLUT_LEFT_BUTTON:
		switch (state) //switch on which button was pressed
		{
		case GLUT_DOWN: //If the left button is pressed color the pixel red
			//printf("Mouse button event, button=%d, state=%d, x=%d, y=%d\n", button, state, x, y); 
			clicks++;
			//printf("clicks = %d\n", clicks); 
			if (clicks == 1)
			{
				firstPoint.x = mouseClick.x;
				firstPoint.y = mouseClick.y;
				firstPoint.r = rand() % (mouseClick.x + mouseClick.y) % 255;
				firstPoint.g = rand() % (mouseClick.x + mouseClick.y) % 255;
				firstPoint.b = rand() % (mouseClick.x + mouseClick.y) % 255;
				DrawPoint(firstPoint.x, firstPoint.y, firstPoint.r, firstPoint.g, firstPoint.b);
			}
			else if (clicks == 2)
			{
				secondPoint.x = mouseClick.x;
				secondPoint.y = mouseClick.y;
				secondPoint.r = rand() % (mouseClick.x + mouseClick.y) % 155;
				secondPoint.g = rand() % (mouseClick.x + mouseClick.y) % 255;
				secondPoint.b = rand() % (mouseClick.x + mouseClick.y) % 255;
				DrawPoint(secondPoint.x, secondPoint.y, secondPoint.r, secondPoint.g, secondPoint.b);
			}
			else if (clicks == 3)
			{
				thirdPoint.x = mouseClick.x; thirdPoint.y = mouseClick.y;
				thirdPoint.r = rand() % (mouseClick.x + mouseClick.y) % 255;
				thirdPoint.g = rand() % (mouseClick.x + mouseClick.y) % 255;
				thirdPoint.b = rand() % (mouseClick.x + mouseClick.y) % 255;
				DrawPoint(thirdPoint.x, thirdPoint.y, thirdPoint.r, thirdPoint.g, thirdPoint.b);
				//for(int i = 0; i < 3; i++)
					//DrawLine(firstPoint, secondPoint, i)
				

				DrawTriangle(firstPoint, secondPoint, thirdPoint);
				
				clicks = 0;
			}
		}
		break;
	case GLUT_RIGHT_BUTTON:
		switch (state)
		{
		case GLUT_DOWN:
			//glutDisplayFunc(displayHandler);
			printf("clear that \n");
			break;
		}
		break;
	}
	// cause a display event to occur for GLUT: 
	glutPostRedisplay();
}
void DrawPoint(int x, int y, int r, int g, int b)
{

	frame_buffer[y][x][0] = r;
	frame_buffer[y][x][1] = g;
	frame_buffer[y][x][2] = b;

}
/*void DrawLine(Point2D first, Point2D second, int color)
{
float dx = second.x - first.x;
float dy = second.y - first.y;
float slope = dy / dx;
float start_x = first.x;
float end_x = second.x;
float start_y = first.y;
float end_y = second.y;

if (abs(slope) > 1)//if the slope is too steep color pixels from top to bottom
{
printf("slope is %f\n", slope); printf("dx is %f\n", dx);
printf("dy is %f\n", dy); printf("drawing line from <%f,%f> to <%f,%f>\n", start_x, start_y, end_x, end_y);
slope = dx / dy;
//calculate a new slope based on dx being the rise and dy being the run if (dy <= 0) //if the new run is less than or equal to zero then the order of iteration must
{ //be reversed. This is if the user clicked a point below the previous point.
start_x = second.x; end_x = first.x; start_y = second.y; end_y = first.y;
}
for (int i = start_y; i < end_y; i++) //start the coloring at the beginning of the run and stop at the
{
//end of the run
DrawPoint(floor(start_x), i, color);
start_x += slope;
//increment the pixel based on the run
}
}
else
//if the slope is less than 1 or not too steep we will color pixels left to right
{
printf("slope is %f\n", slope); printf("dx is %f\n", dx); printf("dy is %f\n", dy);
printf("drawing line from <%f,%f> to <%f,%f>\n", start_x, start_y, end_x, end_y); if (dx <= 0) //the user clicked to the left so swap the first and last end points
{
start_x = second.x; end_x = first.x; start_y = second.y; end_y = first.y;
}
for (int i = start_x; i <= end_x; i++) { DrawPoint(i, floor(start_y), color); start_y += slope; }
}
}
*/

float CheckForZero(int x2, int x1, int y2, int y1)
{	
	if (x2 - x1 == 0 || y2 - y1 == 0)
	{
		printf("DIVIDE BY ZERO\n");
		return 0;
	}

		
	else
		return (float(x2 - x1) / float(y2 - y1));
}

void DrawTriangle(Point2D first, Point2D second, Point2D third)
{
	Point2D bot;
	Point2D top;
	Point2D mid;

	


	bool skip_bot = false;
	bool skip_top = false;

	bool swap_top = false;
	bool swap_bot = false;

	if (first.y < second.y && second.y <= third.y) { bot = first; top = third; mid = second; }
	else if (first.y <= third.y && third.y <= second.y) { bot = first; top = second; mid = third; }
	else if (second.y <= third.y && third.y <= first.y) { bot = second; top = first; mid = third; }
	else if (second.y <= first.y && first.y <= third.y) { bot = second; top = third; mid = first; }
	else if (third.y <= first.y && first.y <= second.y) { bot = third; top = second; mid = first; }
	else if (third.y <= second.y && second.y <= first.y) { bot = third; top = first; mid = second; }

	if (mid.y == bot.y) { skip_bot = true; }
	if (mid.y == top.y) { skip_top = true; }

	printf("top point is (%i, %i) | rgb:[%i, %i, %i]\n", top.x, top.y, top.r, top.g, top.b);
	printf("mid point is (%i, %i) | rgb:[%i, %i, %i]\n", mid.x, mid.y, mid.r, mid.g, mid.b);
	printf("bot point is (%i, %i) | rgb:[%i, %i, %i]\n", bot.x, bot.y, bot.r, bot.g, bot.b);

	//top.x = 250; top.y = 700;
	//mid.x = 100; mid.y = 500;
	//bot.x = 50; bot.y = 250;

	if (!skip_bot)
	{
		printf("draw bottom\n");
		float left_x = (float)bot.x;  float right_x = (float)bot.x;
		float red_l = (float)bot.r;   float red_r = (float)bot.r;
		float green_l = (float)bot.g; float green_r = (float)bot.g;
		float blue_l = (float)bot.b;  float blue_r = (float)bot.b;
		float slope_l01 = CheckForZero(mid.x, bot.x, mid.y, bot.y);
		float slope_r01 = float(mid.r - bot.r) / float(mid.y - bot.y);		
		float slope_g01 = float(mid.g - bot.g) / float(mid.y - bot.y);
		float slope_b01 = float(mid.b - bot.b) / float(mid.y - bot.y);
		float slope_l02 = CheckForZero(top.x, bot.x, top.y, bot.y);
		float slope_r02 = float(top.r - bot.r) / float(top.y - bot.y);
		float slope_g02 = float(top.g - bot.g) / float(top.y - bot.y);
		float slope_b02 = float(top.b - bot.b) / float(top.y - bot.y);
		float red = (float)bot.r; float blue = (float)bot.b; float green = (float)bot.g;
		for (int y_inc = bot.y; y_inc < mid.y; y_inc++)//scanline going up
		{
			//amount of change in color per scan line
			red_l += slope_r02; red_r += slope_r01;
			blue_l += slope_b02; blue_r += slope_b01;
			green_l += slope_g02; green_r += slope_g01;
			
			
			//go over the scanline left to right and assign color value based on the rate of color change per scanline
			for (int x_inc = (int)ceil(left_x); x_inc <= (int)floor(right_x); x_inc++)
			{
				
					float dr = (red_r - red_l) / (right_x - left_x);
					float db = (blue_r - blue_l) / (right_x - left_x);
					float dg = (green_r - green_l) / (right_x - left_x);
					red = red_l + (ceil(left_x) - left_x) * dr;
					blue = blue_l + (ceil(left_x) - left_x) * db;
					green = green_l + (ceil(left_x) - left_x) * dg;
					DrawPoint(x_inc, y_inc, (int)red, (int)blue, (int)green);
				
			}

			if (slope_l01 < slope_l02) //if bot to top is steeper change left_x
			{
				left_x += slope_l01; right_x += slope_l02;
			}
			else
			{
				left_x += slope_l02; right_x += slope_l01;
			}
		}
		printf("done\n");
	}

	if (!skip_top)
	{
		printf("draw top\n");
		float left_x = (float)top.x; float right_x = (float)top.x;
		float red_l = (float)top.r; float red_r = (float)top.r;
		float green_l = (float)top.g; float green_r = (float)top.g;
		float blue_l = (float)top.b; float blue_r = (float)top.b;
		float slope_l21 = CheckForZero(mid.x, top.x, mid.y, top.y);
		float slope_r21 = float(mid.r - top.r) / float(mid.y - top.y);//red change increment from top to middle
		float slope_g21 = float(mid.g - top.g) / float(mid.y - top.y); //green change increment from top to middle
		float slope_b21 = float(mid.b - top.b) / float(mid.y - top.y); //blue change from top to middle
		float slope_l20 = CheckForZero(bot.x, top.x, bot.y, top.y); //position change increment from top to bottom
		float slope_r20 = float(bot.r - top.r) / float(bot.y - top.y);//red change increment from top to bottom
		float slope_g20 = float(bot.g - top.g) / float(bot.y - top.y);//green change increment from top to bottom
		float slope_b20 = float(bot.b - top.b) / float(bot.y - top.y);//blue change from top to bottom
		float red = (float)top.r; float blue = (float)top.b; float green = (float)top.g;
		
		for (int y_inc = top.y; y_inc >= mid.y; y_inc--)
		{		
			red_l -= slope_r20;	red_r -= slope_r21;
			green_l -= slope_g20; green_r -= slope_g21;
			blue_l -= slope_b20; blue_r -= slope_b21;			
			
			for (int x_inc = (int)ceil(left_x); x_inc <= (int)floor(right_x); x_inc++)
			{		
			
					float dr = (red_r - red_l) / (right_x - left_x);
					float db = (blue_r - blue_l) / (right_x - left_x);
					float dg = (green_r - green_l) / (right_x - left_x);
					red = red_l + (ceil(left_x) - left_x) * dr;
					blue = blue_l + (ceil(left_x) - left_x) * db;
					green = green_l + (ceil(left_x) - left_x) * dg;
					DrawPoint(x_inc, y_inc, (int)red, (int)blue, (int)green);
				
			}

			if (slope_l20 < slope_l21)
			{
				left_x -= slope_l21; right_x -= slope_l20;
			}
			else
			{
				left_x -= slope_l20; right_x -= slope_l21;

			}
		}
	}
	else
	{
		//top was skipped but still must draw the midline
		printf("skip top draw one line\n");
		float left_x = (float)top.x; float right_x = (float)mid.x;
		int y = mid.y;
		float red_l = (float)top.r; float red_r = (float)mid.r;
		float green_l = (float)top.g; float green_r = (float)mid.g;
		float blue_l = (float)top.b; float blue_r = (float)mid.b;
		float red = (float)top.r; float blue = (float)top.b; float green = (float)top.g;
		for (int i = (int)ceil(left_x); i <= (int)floor(right_x); i++)
		{		
			float dr = (red_r - red_l) / (right_x - left_x);
			float db = (blue_r - blue_l) / (right_x - left_x);
			float dg = (green_r - green_l) / (right_x - left_x);
			red = red_l + (ceil(left_x) - left_x) * dr;
			blue = blue_l + (ceil(left_x) - left_x) * db;
			green = green_l + (ceil(left_x) - left_x) * dg;
			DrawPoint(i, y, (int)red, (int)blue, (int)green);
		}
	}


}



int main(int argc, char ** argv)
{
	//GLUT initialization:	
	glutInit(&argc, argv); //Usage: glutInit is used to initialize the GLUT library. //Parameters: (argcp) - A pointer to the program's unmodified argc variable from main. Upon return, the value pointed //to by argcp will be updated, because glutInit extracts any command line options intended for //the GLUT library. (argv) - The program's unmodified argv variable from main. Like argcp, the data for argv will 
	//be updated because glutInit extracts any command line options understood by the GLUT library.
	//Description: glutInit will initialize the GLUT library and negotiate a session with the window system.During this process, 
	//glutInit may cause the termination of //the GLUT program with an error message to the user if GLUT cannot be properly initialized.Examples of this situation include 
	//the failure to connect to the //window system, the lack of window system support for OpenGL, and invalid command line options. 
	//glutInit also processes command line options, but the specific options parse are window system dependent. 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	//glutInitDisplayMode sets the initial display mode. //Description:	The initial display mode is used when creating top-level windows, subwindows, and overlays to determine the OpenGL display mode for the to-be-created window or overlay.
	//Note that GLUT_RGBA selects the RGBA color model, but it does not request any bits of alpha (sometimes called an alpha buffer or destination alpha) be allocated. 
	//To request alpha, specify GLUT_ALPHA. The same applies to GLUT_LUMINANCE.	

	glutInitWindowSize(WIDTH, HEIGHT); //Usage: glutInitWindowPosition and glutInitWindowSize set the initial window position and size respectively. 
	//Parameters: (WIDTH) - the xmin xmax value of the window //(HEIGHT) - the height 
	//Description: Windows created by glutCreateWindow will be requested to be created with the current initial window position and size.

	glutInitWindowPosition(500, 250);
	glutCreateWindow("mcw4553 : Assignment_01"); //Usage: Make the window and give it a name 

	glutDisplayFunc(displayHandler);
	//Usage: glutDisplayFunc sets the display callback for the current window. 
	//Parameters: ((*func) (void)) - pointer to the display callback function. 
	glutMouseFunc(mouseHandler);
	glutMainLoop(); //Usage: void glutMainLoop(void); //Description: glutMainLoop enters the GLUT event processing loop. //This routine should be called at most once in a GLUT program. Once called, this routine will never return. //It will call as necessary any callbacks that have been registered. 
	return 1;
}
