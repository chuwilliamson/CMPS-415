#pragma once

#include "Texture.h"
#include "LoadShaders.h"
#include "Geometry.h"
#include "Transforms.h"
#include "Particles.h"

Particles flock(10, false);
Particles preds(10, true);
float dispersion = 1.0f;
float centering = 1.0f;
float alignment = 1.0f;
GLuint bird_vao;
GLuint dbo, cbo, abo;
//gmtl::Point3f dForce[2];
float dForce[6];


bool lightFlag = true;
GLuint vao1, vao2, vao3, vao4, vao5;
GLuint vbo1, vbo2, vbo3, vbo4, vbo5;
GLuint ibo1, ibo2, ibo3, ibo4, ibo5;
GLuint CBO, UVBO, VBO, VAO;

#pragma region Shader Variables

GLuint PROGRAM1, PROGRAM2;
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
GLuint sphereTexture, backgroundTexture, predTexture;
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
	this->sfac = 1;
	this->hfac = 1;
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
unsigned char *imagedata1, *imagedata2, *imagedata3;


bool showTex = 0;
bool animate = 1;

void SetupTextures();
void CheckError();
template <typename Type> void print(Type a);
void DrawObject(GLuint &v, GLuint &b, GLuint &i, int indexSize, gmtl::Matrix44f transform);
void SetupShaders();
void CheckShader();
void BuildShader(GLuint shader);
void ChangeLights(Light c);
void RenderScene();
void DrawGlobe();
void DrawSkybox();
void DrawBird(Bird b, gmtl::Matrix44f birdRot);
void DrawForce(Bird b);
void DrawAxis(gmtl::Matrix44f rot);
void DrawForces(Bird b);
void Keyboard();
void Mouse();
void Motion();
void Update();
void SetupForceVisualization();
void DrawPred(Bird b);

//================================================================================================================================================================================
template <typename Type> void print(Type a)
{
	std::cout << a << std::endl;

}

void DrawBird(Bird b)
{
	gmtl::Matrix44f birdRot = T_s2_w * b._T_o_r;

	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	{
		gmtl::Matrix44f body = birdRot * Scale(.05f * b._mass);
		gmtl::Matrix44f topPropellerBase = body * Rotation(-90, 'z') * Scale(1.5f);
		gmtl::Matrix44f topPropeller = topPropellerBase * Translate(1, 'y') * T_j2_b;
		gmtl::Matrix44f nose = body * Translate(.5f, 'y') * Scale(8.0f);
		gmtl::Matrix44f tail = body *  Translate(-1, 'y') * Rotation(180, 'x') * Scale(4.0f);
		gmtl::Matrix44f frontPropeller = nose * Translate(.25f, 'y') * Scale(0.15f) * T_j2_b;

		//body of helicopter
		DrawObject(vao1, vao1, ibo1, GetSize('s'), body);
		//nose of helicopter
		DrawObject(vao3, vao3, ibo3, GetSize('c'), nose);
		DrawObject(vao3, vao3, ibo3, GetSize('c'), tail);
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropellerBase);
		//rotor mast
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(90, 'z'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(-90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(-90, 'z'));

		//front propeller
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller * Rotation(90, 'z'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller * Rotation(90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller * Rotation(-90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller * Rotation(-90, 'z'));




	}glBindTexture(GL_TEXTURE_2D, 0);

}

void DrawPred(Bird b)
{
	gmtl::Matrix44f birdRot = T_s3_w * b._T_o_r;
	gmtl::Matrix44f body = birdRot * Scale(.035f * b._mass);
	gmtl::Matrix44f topPropellerBase = body * Rotation(-90, 'z') * Scale(1.5f);
	gmtl::Matrix44f topPropeller = topPropellerBase * Translate(1, 'y') * T_j2_b;
	gmtl::Matrix44f nose = body * Translate(0.70f, 'y') * Rotation(5, 'z') * Scale(6.0f);

	gmtl::Matrix44f tail = body * Translate(-1.25f, 'y') * Rotation(180, 'y') * Scale(4.0f);
	gmtl::Matrix44f frontPropeller1 = nose * Translate(.25f, 'y') * Scale(0.25f) * T_j2_b;
	gmtl::Matrix44f frontPropeller2 = tail    * Scale(0.15f) * T_j2_b;
	glBindTexture(GL_TEXTURE_2D, predTexture);
	{//body of helicopter
		DrawObject(vao1, vao1, ibo1, GetSize('s'), body);

	}glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, sphereTexture);
	{//nose of helicopter
		DrawObject(vao3, vao3, ibo3, GetSize('c'), nose);
		DrawObject(vao3, vao3, ibo3, GetSize('c'), tail);
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropellerBase);
		//rotor mast
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(90, 'z'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(-90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), topPropeller * Rotation(-90, 'z'));

		//front propeller
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller1 * Rotation(90, 'z'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller1 * Rotation(90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller1 * Rotation(-90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller1 * Rotation(-90, 'z'));

		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller2 * Rotation(90, 'z'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller2 * Rotation(90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller2 * Rotation(-90, 'x'));
		DrawObject(vao2, vao2, ibo2, GetSize('a'), frontPropeller2 * Rotation(-90, 'z'));
	}glBindTexture(GL_TEXTURE_2D, 0);

}

void DrawAxis(gmtl::Matrix44f rot)
{
	DrawObject(vao2, vao2, ibo2, GetSize('a'), rot * Rotation(90, 'x'));
	DrawObject(vao2, vao2, ibo2, GetSize('a'), rot * Scale(1.0f));
	DrawObject(vao2, vao2, ibo2, GetSize('a'), rot * Rotation(-90, 'z'));
}

void DrawGlobe()
{
	//what does this even do???
	//comment it out and nothing changes
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(texture_color, 0);
	if (!showTex)
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	else
		glBindTexture(GL_TEXTURE_2D, sphereTexture);

	DrawObject(vao1, vbo1, ibo1, GetSize('s'), T_s2_w);
	DrawObject(vao1, vbo1, ibo1, GetSize('s'), T_s3_w);
	glBindTexture(GL_TEXTURE_2D, 0);


}

void DrawForce(Bird b)
{
	gmtl::Matrix44f Model = T_s2_w * b._T_o_r;

	gmtl::Matrix44f View = GetCamera();
	gmtl::Matrix44f Projection = GetProjection();
	//invert the camera for View matrix
	gmtl::invert(View);

	MVP = Projection * View * Model;
	gmtl::Vec3f o = b._origin;
	gmtl::Vec3f od = b._origin + b._dispersion;
	dForce[0] = 0;
	dForce[1] = 0;
	dForce[2] = 0;
	dForce[3] = b._dispersion[0];
	dForce[4] = b._dispersion[1];
	dForce[5] = b._dispersion[2];

	glBindVertexArray(bird_vao);
	glBindBuffer(GL_ARRAY_BUFFER, dbo);
	glEnableVertexAttribArray(0);
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dForce), &dForce[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glDrawArrays(GL_LINES, 0, 2);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void DrawTeapot(GLuint &v, GLuint &b, GLuint &i, int indexSize, gmtl::Matrix44f Model)
{
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	gmtl::Matrix44f View = GetCamera();
	gmtl::Matrix44f Projection = GetProjection();
	gmtl::Point3f l(0, 5, 1);
	gmtl::Point3f w(View[0][3], View[1][3], View[2][3]);

	//invert the camera for View matrix
	gmtl::invert(View);

	MVP = Projection * View * Model;
	gmtl::Matrix44f NormalMatrix = View * Model;
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &NormalMatrix[0][0]);

	glUniform3f(light_position, light.Pos[0], light.Pos[1], light.Pos[2]);
	glUniform1f(light_shininess, light.shine);
	glUniform1f(light_strength, light.strength);
	glUniform1f(sf, light.sfac);
	glUniform1f(hf, light.hfac);
	glUniform1f(df, light.dfac);

	glUniform3f(world_position, w[0], w[1], w[2]);
	light.Pos = View * l;

	glBindVertexArray(v);

	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, b);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
	glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_PRIMITIVE_RESTART);


	for (int i = 0; i < 4; i++)
	{
		glDisableVertexAttribArray(i);
	}
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawObject(GLuint &v, GLuint &b, GLuint&i, int indexSize, gmtl::Matrix44f Model)
{
	gmtl::Matrix44f View = GetCamera();
	gmtl::Matrix44f Projection = GetProjection();
	//invert the camera for View matrix
	gmtl::invert(View);

	MVP = Projection * View * Model;

	gmtl::Matrix44f NormalMatrix = View * Model;

	gmtl::Point3f l(0, 5, 1);
	gmtl::Point3f w(View[0][3], View[1][3], View[2][3]);

	light.Pos = View * l;

	glBindVertexArray(v);

	for (int i = 0; i < 4; i++)
	{

		glEnableVertexAttribArray(i);
	}
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
	glDisable(GL_PRIMITIVE_RESTART);
	glBindVertexArray(0);
	for (int i = 0; i < 4; i++)
	{
		glDisableVertexAttribArray(i);
	}

}

void SetupForceVisualization()
{
	glGenVertexArrays(1, &bird_vao);
	glGenBuffers(1, &dbo);

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

	glGenTextures(1, &predTexture);
	LoadPPM("checkerboard.ppm", &texwidth, &texheight, &imagedata3, 1);
	glBindTexture(GL_TEXTURE_2D, predTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texheight, texwidth, 0, GL_RGB, GL_UNSIGNED_BYTE, imagedata3);
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

	ShaderInfo skyShaders[] =
	{
		{ GL_VERTEX_SHADER, "sky_shader.vert" },
		{ GL_FRAGMENT_SHADER, "sky_shader.frag" },
		{ GL_NONE, NULL }
	};

	PROGRAM1 = LoadShaders(shaders);
	BuildShader(PROGRAM1);

	PROGRAM2 = LoadShaders(skyShaders);
	BuildShader(PROGRAM2);

	CheckShader();

}

void BuildShader(GLuint shader)
{

	//==== UNIFORM LOCATION BINDING ====
	mvpID = glGetUniformLocation(shader, "MVPMatrix");
	mvID = glGetUniformLocation(shader, "NormalMatrix");
	eye_position = glGetUniformLocation(shader, "EyePosition");
	world_position = glGetUniformLocation(shader, "WorldPosition");
	light_position = glGetUniformLocation(shader, "LightPosition");
	texture_color = glGetUniformLocation(shader, "TextureColor");
	light_color = glGetUniformLocation(shader, "LightColor");
	light_shininess = glGetUniformLocation(shader, "Shininess");
	light_strength = glGetUniformLocation(shader, "Strength");
	hf = glGetUniformLocation(shader, "hFactor");
	df = glGetUniformLocation(shader, "dFactor");
	sf = glGetUniformLocation(shader, "sFactor");

	//==== END UNIFORM LOCATION BINDINGS ====

	//==== ATTRIBUTE LOCATION BINDINGS ====
	vertex_position = glGetAttribLocation(shader, "VertexPosition");
	vertex_color = glGetAttribLocation(shader, "VertexColor");
	vertex_UV = glGetAttribLocation(shader, "VertexUV");
	vertex_normal = glGetAttribLocation(shader, "VertexNormal");
	//==== END ATTRIBUTE LOCATION BINDINGS ====


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

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033 /* Escape key */:
		exit(EXIT_SUCCESS);
		break;
	case 'a':
		alignment--;
		//
		std::cout << "centering = " << centering << std::endl;
		std::cout << "alignment = " << alignment << std::endl;
		std::cout << "dispersion = " << dispersion << std::endl;
		break;
	case 'A':
		alignment++;
		//
		std::cout << "centering = " << centering << std::endl;
		std::cout << "alignment = " << alignment << std::endl;
		std::cout << "dispersion = " << dispersion << std::endl;
		break;
	case 'c':
		centering -= 1;
		//
		std::cout << "centering = " << centering << std::endl;
		std::cout << "alignment = " << alignment << std::endl;
		std::cout << "dispersion = " << dispersion << std::endl;
		break;
	case 'C':
		centering += 1;
		//
		std::cout << "centering = " << centering << std::endl;
		std::cout << "alignment = " << alignment << std::endl;
		std::cout << "dispersion = " << dispersion << std::endl;
		break;
	case 'd':
		dispersion -= 1;
		//
		std::cout << "centering = " << centering << std::endl;
		std::cout << "alignment = " << alignment << std::endl;
		std::cout << "dispersion = " << dispersion << std::endl;
		break;
	case 'D':
		dispersion += 1;
		//
		std::cout << "centering = " << centering << std::endl;
		std::cout << "alignment = " << alignment << std::endl;
		std::cout << "dispersion = " << dispersion << std::endl;
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
		SetCamera(Camera3);
		break;
	case '4':
		SetCamera(Camera4);
		break;
	case 't':
		break;
	case 'h':
		light.hfac -= 0.1f;
		break;
	case 'H':
		light.hfac += 0.1f;
		break;
	case 'z':
		light.ChangeLights('d');
		break;
	case 'x':
		light.ChangeLights('s');
		break;
	case '6':
		light.ChangeLights('h');
		break;
	case '7':
		light.ChangeLights('f');
		break;
	case 'p':
		flock.BirdAt(0).Print();
		for (int i = 0; i < flock._size; i++)
		{
			std::cout << "bird " << i << " velocity: " << flock.BirdAt(i)._fwdVel << std::endl;
			std::cout << "bird " << i << " dispersion: " << flock.BirdAt(i)._dAffectors << std::endl;
		}

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

	}

	//reset the oldX and Y values to prevent the sphere from resetting
	//itself after clicking a new click event
	oldX = x;
	oldY = y;

}
