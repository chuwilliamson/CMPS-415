#pragma once

#include "Texture.h"
#include "LoadShaders.h"
#include "Geometry.h"
#include "Transforms.h"
#include "Particles.h"

Particles flock;
GLuint vao1, vao2, vao3, vao4;
GLuint vbo1, vbo2, vbo3, vbo4;
GLuint ibo1, ibo2, ibo3, ibo4;

#pragma region Shader Variables

GLuint PROGRAM;
//===Lights===
GLuint
light_ambient,
light_color,
light_shininess,
light_strength,
light_position,
world_position,
hf, df, sf;
//===End Lights

//===Matrix Positions===
GLuint mvpID, mvID, eye_position;
//=== END Matrix Positions===

//===Geometry attributes
GLuint vertex_color, vertex_UV,
vertex_normal, texture_color, vertex_position;
//===End Geometry attributes

//===Textures===
GLuint sphereTexture, backgroundTexture;
//===End Textures===

#pragma endregion


class Light
{

public:

	static enum LightState
	{
		diffuse,
		specular,
		hem,
		full,
	};

	Light(float x, float y, float z, float shine, float strength);
	void ChangeLights(char);

	float shine;
	float strength;
	LightState lightState;
	gmtl::Point3f Pos;
	float hfac;
	float dfac;
	float sfac;

private:
	float x;
	float y;
	float z;


};

Light::Light(float X, float Y, float Z, float Shine, float Strength)
{
	this->x = X;
	this->y = Y;
	this->z = Z;
	this->shine = Shine;
	this->strength = Strength;
	this->Pos = gmtl::Point3f(X, Y, Z);
	this->sfac = 0;
	this->hfac = 0;
	this->dfac = 1;
}

void Light::ChangeLights(char s)
{
	switch (s)
	{
	case 'd':
		this->sfac = 0;
		this->hfac = 0;
		this->dfac = 1;

		break;

	case 's':
		this->sfac = 1;
		this->hfac = 0;
		this->dfac = 0;
		break;

	case 'h':
		this->sfac = 0;
		this->hfac = 1;
		this->dfac = 0;
		break;

	case 'f':
		this->sfac = 1;
		this->hfac = 1;
		this->dfac = 1;
		break;

	default:
		break;
	}

}

Light light(0, 5, 0, 20, 10);


class Timer
{
public:
	int frame;
	float time;
	int currentTime;
	float previousTime;
	float deltaTime;
	float timebase;
};

Timer Time;

gmtl::Matrix44f MVP;

float mouse_long, mouse_lat, mouse_azi, mouse_ele;
int oldX, oldY, oldZ;
bool mouseMoving = false;

GLenum errCode;
const GLubyte *errString;

GLuint texwidth, texheight;
unsigned char *imagedata1, *imagedata2;


bool showTex = 1;
bool animate = 1;

void SetupTextures();
void CheckError();
template <typename Type> void print(Type a);
void DrawObject(GLuint &v, GLuint &b, GLuint &i, int indexSize, gmtl::Matrix44f transform);
void SetupShaders();
void ChangeLights(Light c);
void RenderBitmapString(float x, float y, char *string);
void RenderScene();
void DrawGlobe();
void DrawBird(gmtl::Matrix44f birdRot);
void DrawAxis(gmtl::Matrix44f rot);
void CheckShader();
void Keyboard();
void Mouse();
void Motion();
void Update();



//================================================================================================================================================================================





void DrawBird(gmtl::Matrix44f birdRot, float mass)
{
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	{
		gmtl::Matrix44f body = birdRot * Scale(.035f * mass);
		gmtl::Matrix44f topPropellerBase = body * Rotation(-90, 'z') * Scale(1.5f);
		gmtl::Matrix44f topPropeller = topPropellerBase * Translate(1, 'y') * T_j2_b;
		gmtl::Matrix44f nose = body * Translate(1, 'y') * Scale(8.0f);
		gmtl::Matrix44f frontPropeller = nose * Translate(.25f, 'y') * Scale(0.25f) * T_j2_b;

		//body of helicopter
		DrawObject(vao1, vao1, ibo1, GetGeometrySize('s'), body);
		//nose of helicopter
		DrawObject(vao3, vao3, ibo3, GetGeometrySize('c'), nose);
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), topPropellerBase);
		//rotor mast
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), topPropeller * Rotation(90, 'z'));
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), topPropeller * Rotation(90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), topPropeller * Rotation(-90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), topPropeller * Rotation(-90, 'z'));

		//front propeller
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), frontPropeller * Rotation(90, 'z'));
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), frontPropeller * Rotation(90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), frontPropeller * Rotation(-90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), frontPropeller * Rotation(-90, 'z'));

		
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawAxis(gmtl::Matrix44f rot)
{
	DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), rot * Rotation(90, 'x'));
	DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), rot * Scale(1.0f));
	DrawObject(vao2, vao2, ibo2, GetGeometrySize('a'), rot * Rotation(-90, 'z'));
}

void DrawGlobe()
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(texture_color, 0);
	if (!showTex)
	{
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		DrawObject(vao1, vbo1, ibo1, GetGeometrySize('s'), T_s_w * Translate(1.10f, 'x'));
		DrawObject(vao1, vbo1, ibo1, GetGeometrySize('s'), T_s_w * Translate(-1.10f, 'x'));
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, sphereTexture);
		DrawObject(vao1, vbo1, ibo1, GetGeometrySize('s'), T_s_w * Scale(.75f));
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	
}

void SetupTextures()
{
	//==== SETUP TEXTURES ====
	glGenTextures(1, &sphereTexture);
	LoadPPM("wood.ppm", &texwidth, &texheight, &imagedata1, 1);
	std::cout << "TEXTURE IMAGE WIDTH: " << texwidth << "\nTEXTURE IMAGE HEIGHT: " << texheight << std::endl;
	glBindTexture(GL_TEXTURE_2D, sphereTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texheight, texwidth, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenTextures(1, &backgroundTexture);
	LoadPPM("background.ppm", &texwidth, &texheight, &imagedata2, 1);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texheight, texwidth, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	//==== END SETUP TEXTURES ====
}

void SetupShaders()
{
	// Enable depth test (visible surface determination)
	glEnable(GL_DEPTH_TEST);

	// OpenGL background color
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "v_shader.vert" },
		{ GL_FRAGMENT_SHADER, "f_shader.frag" },
		{ GL_NONE, NULL }
	};

	PROGRAM = LoadShaders(shaders);
	glUseProgram(PROGRAM);

	//==== UNIFORM LOCATION BINDING ====
	mvpID = glGetUniformLocation(PROGRAM, "MVPMatrix");
	mvID = glGetUniformLocation(PROGRAM, "NormalMatrix");
	eye_position = glGetUniformLocation(PROGRAM, "EyePosition");
	world_position = glGetUniformLocation(PROGRAM, "WorldPosition");
	light_position = glGetUniformLocation(PROGRAM, "LightPosition");
	texture_color = glGetUniformLocation(PROGRAM, "TextureColor");
	light_color = glGetUniformLocation(PROGRAM, "LightColor");
	light_shininess = glGetUniformLocation(PROGRAM, "Shininess");
	light_strength = glGetUniformLocation(PROGRAM, "Strength");
	hf = glGetUniformLocation(PROGRAM, "hFactor");
	df = glGetUniformLocation(PROGRAM, "dFactor");
	sf = glGetUniformLocation(PROGRAM, "sFactor");

	//==== END UNIFORM LOCATION BINDINGS ====

	//==== ATTRIBUTE LOCATION BINDINGS ====
	vertex_position = glGetAttribLocation(PROGRAM, "VertexPosition");
	vertex_color = glGetAttribLocation(PROGRAM, "VertexColor");
	vertex_UV = glGetAttribLocation(PROGRAM, "VertexUV");
	vertex_normal = glGetAttribLocation(PROGRAM, "VertexNormal");
	//==== END ATTRIBUTE LOCATION BINDINGS ====

	CheckShader();

}

void DrawObject(GLuint &v, GLuint &b, GLuint&i, int indexSize, gmtl::Matrix44f Model)
{

	gmtl::Matrix44f View = GetCamera();
	gmtl::Matrix44f Projection = GetProjection();
	//invert the camera for View matrix
	gmtl::invert(View);

	MVP = Projection * View * Model;


	gmtl::Matrix44f NormalMatrix = View * Model;
	//print(NormalMatrix);

	gmtl::Point3f l(0, 5, 1);
	gmtl::Point3f w(View[0][3], View[1][3], View[2][3]);

	//std::cout<<w<<std::endl;

	light.Pos = View * l;

	glBindVertexArray(v);
	glBindBuffer(GL_ARRAY_BUFFER, b);

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &NormalMatrix[0][0]);

	glUniform3f(light_position, light.Pos[0], light.Pos[1], light.Pos[2]);
	glUniform1f(light_shininess, light.shine);
	glUniform1f(light_strength, light.strength);
	glUniform1f(sf, light.sfac);
	glUniform1f(hf, light.hfac);
	glUniform1f(df, light.dfac);

	glUniform3f(world_position, w[0], w[1], w[2]);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void RenderBitmapString(float x, float y, char *string)
{
	char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}

}

void CheckShader()
{
	std::cout << "location of mvpID: " << mvpID << std::endl;
	std::cout << "location of mvID: " << mvID << std::endl;
	std::cout << "location of eye_position: " << eye_position << std::endl;
	std::cout << "location of world_position: " << world_position << std::endl;
	std::cout << "location of light_position: " << light_position << std::endl;
	std::cout << "location of texture_color: " << texture_color << std::endl;
	std::cout << "location of light_color: " << light_color << std::endl;
	std::cout << "location of light_shininess: " << light_shininess << std::endl;
	std::cout << "location of light_strength: " << light_strength << std::endl;
	std::cout << "location of vertex_position: " << vertex_position << std::endl;
	std::cout << "location of vertex_color: " << vertex_color << std::endl;
	std::cout << "location of vertex_UV: " << vertex_UV << std::endl;
	std::cout << "location of vertex_normal: " << vertex_normal << std::endl;

}

void CheckError()
{

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		//std::cout << "OpengGL Error: " << errString;// << std::endl;
	}
}

template <typename Type> void print(Type a)
{
	std::cout << a << std::endl;

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033 /* Escape key */:
		exit(EXIT_SUCCESS);
		break;
	case 'R':
		light.shine--;
		break;
	case 'r':
		light.shine++;
		break;
	case 'E':
		light.strength--;
		break;
	case 'e':	  
		break;
	case '1':
		SetCamera(Camera1);
		break;
	case '2':
		SetCamera(Camera2);
		break;
	case '3':
		showTex = !showTex;
		break;
	case 'F':
		light.hfac -= 0.1f;
		break;
	case 'f':
		light.hfac += 0.1f;
		break;
	case '4':
		light.ChangeLights('d');
		break;
	case '5':
		light.ChangeLights('s');
		break;
	case '6':
		light.ChangeLights('h');
		break;
	case '7':
		light.ChangeLights('f');
		break;
	case 'z':
		flock.birds.at(0).Print();
			
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
		zInc += .05f;//positive z means camera moves toward user becuase of right hand rule
		//glutPostRedisplay();
	}
	else if (button == 3)
	{
		zInc -= .05f;
		//glutPostRedisplay();
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

		switch (cameraState)
		{	//Rlat is x rotation by lat and Rlong is y rotation by long
			//increment long according to horizontal mouse movement and lat according to vertical motion
		case Camera1:
			//T_c1_w =  rLong * rLat * Translate(zInc, 'z');
			break;
			//r_long * r_lat where r_lat is x-rotation by lat and
			//r_long is y rotation by long
			//increment long according to 
		case Camera2:
			//	T_c2_o = World * T_r_w * T_o_r * rAzi * rEle * Rotation(-90, 'z') * Translate(zInc, 'z');

			break;
		}
	}

	//reset the oldX and Y values to prevent the sphere from resetting
	//itself after clicking a new click event
	oldX = x;
	oldY = y;

}
