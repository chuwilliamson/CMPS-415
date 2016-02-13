#ifndef __ASSIGNMENT_04_H__
#define __ASSIGNMENT_04_H__

#include "Texture.h"
#include "LoadShaders.h"
#include "Geometry.h"


unsigned int vao1, vao2, vao3, vao4;
unsigned int vbo1, vbo2, vbo3, vbo4;
unsigned int ibo1, ibo2, ibo3, ibo4;

float shine = 100.0f;
float str = 2.0f;
enum LightState
{
	diffuse,
	specular,
	hem,
	full,
};

enum CameraState
{
	world,
	local,
};

enum SceneState
{
	sphere,
	axis,
	texture,
};

LightState lightState = diffuse;
CameraState cameraState = world;
SceneState sceneState = texture;

float currentTime = 0;
float previousTime = 0;
float deltaTime = 0;

bool animate = 1;
gmtl::Matrix44f MVP;
gmtl::Vec3f lightPos;

bool cam1 = true;
bool cam2 = false;
bool mouseMoving = false;
float mouse_long, mouse_lat, mouse_azi, mouse_ele;
int oldX, oldY, oldZ;
float width = 3.0f;
float depth = 3.0f;


GLenum errCode;
const GLubyte *errString;

unsigned int texwidth, texheight;
unsigned char *imagedata1, *imagedata2; 


bool showSphere = true;
bool showAxis = true;
bool showTex = true;

#pragma region Shader Variables
//===Lights===
GLuint 
light_ambient, 
light_color, 
light_shininess, 
light_strength, 
light_position,
world_position,
hf, df, sf;
GLuint CA, LA, QA;
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


void CheckError();
void DrawObject(GLuint &v, GLuint &b, GLuint &i, int indexSize, gmtl::Matrix44f transform);
void DrawCube(GLuint &v, GLuint &b, gmtl::Matrix44f Model);
void SetupTexture(GLuint program, GLuint TextureID);
void SetupShaders();
void ChangeLights(LightState c);

 //================================================================================================================================================================================


void CheckError()
{

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		//std::cout << "OpengGL Error: " << errString;// << std::endl;
	}
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

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);
	
	//==== UNIFORM LOCATION BINDING ====
	mvpID = glGetUniformLocation(program, "MVPMatrix");
	std::cout << "location of mvpID: " << mvpID << std::endl;

	mvID = glGetUniformLocation(program, "NormalMatrix");	
	std::cout << "location of mvID: " << mvID << std::endl;

	eye_position = glGetUniformLocation(program, "EyePosition");
	std::cout << "location of eye_position: " << eye_position << std::endl;

	world_position = glGetUniformLocation(program, "WorldPosition");
	std::cout << "location of QA: " << world_position << std::endl;

	light_position = glGetUniformLocation(program, "LightPosition");
	std::cout << "location of light_position: " << light_position << std::endl;

	texture_color = glGetUniformLocation(program, "TextureColor");
	std::cout << "location of texture_color: " << texture_color << std::endl;

	light_color = glGetUniformLocation(program, "LightColor");
	std::cout << "location of light_color: " << light_color << std::endl;

	light_shininess = glGetUniformLocation(program, "Shininess");
	std::cout << "location of light_shininess: " << light_shininess << std::endl;

	light_strength = glGetUniformLocation(program, "Strength");
	std::cout << "location of light_strength: " << light_strength << std::endl;

	

	hf = glGetUniformLocation(program, "hFactor");
	
	df = glGetUniformLocation(program, "dFactor");
	 
	sf = glGetUniformLocation(program, "sFactor");

	glUniform1f(hf, 1.0f);
	glUniform1f(df, 1.0f);
	glUniform1f(sf, 1.0f);
	
	//==== END UNIFORM LOCATION BINDINGS ====

	//==== ATTRIBUTE LOCATION BINDINGS ====
	vertex_position = glGetAttribLocation(program, "VertexPosition");
	std::cout << "location of vertex_position: " << vertex_position << std::endl;

	vertex_color = glGetAttribLocation(program, "VertexColor");
	std::cout << "location of vertex_color: " << vertex_color << std::endl;

	vertex_UV = glGetAttribLocation(program, "VertexUV");
	std::cout << "location of vertex_UV: " << vertex_UV << std::endl;

	vertex_normal = glGetAttribLocation(program, "VertexNormal");
	std::cout << "location of vertex_normal: " << vertex_normal << std::endl;
	//==== END ATTRIBUTE LOCATION BINDINGS ====
	//==== SETUP TEXTURES ====

	glGenTextures(1, &sphereTexture);
	LoadPPM("checkerboard.ppm", &texwidth, &texheight, &imagedata1, 1);
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

void DrawCube(GLuint &v, GLuint &b, gmtl::Matrix44f Model)
{
	//invert the camera for view matrix
	if (cam1)
	{
		view = T_c1_w;
		gmtl::invert(view);
	}
	else if (cam2)
	{
		view = T_c2_o;
		gmtl::invert(view);
	}

	MVP = perspective * view * Model;

	lightPos.set(-50.0f, 25.0f, 0.0f);
	xform(lightPos, view, lightPos);
	Matrix44f NormalMatrix = model * view;	 
	Vec3f WorldPosition;
	WorldPosition *= -1;

	glBindVertexArray(v);
	glBindBuffer(GL_ARRAY_BUFFER, b);

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &NormalMatrix[0][0]);


	glUniform1f(light_shininess, shine);
	glUniform1f(light_strength, str);

	glUniform3f(world_position, WorldPosition[0], WorldPosition[1], WorldPosition[2]);
	glUniform3f(eye_position, view[0][3], view[1][3], view[2][3]);
	glUniform3f(light_position, lightPos[0], lightPos[1], lightPos[2]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	// Bind the vertex array for the object
	glBindVertexArray(0);
	// Bind the index buffer for the object
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void DrawObject(GLuint &v, GLuint &b, GLuint&i, int indexSize, gmtl::Matrix44f Model)
{
	//invert the camera for view matrix
	if (cam1)
	{
		view = T_c1_w;
		gmtl::invert(view);
	}
	else if (cam2)
	{
		view = T_c2_o;
		gmtl::invert(view);
	}

	MVP = perspective * view * Model;

	lightPos.set(-50.0f, 25.0f, 0.0f);
	xform(lightPos, view, lightPos);
	Matrix44f NormalMatrix = model * view;

	glBindVertexArray(v);
	glBindBuffer(GL_ARRAY_BUFFER, b);

	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &NormalMatrix[0][0]);

	glUniform1f(light_shininess, shine);
	glUniform1f(light_strength, str);
	glUniform3f(eye_position, view[0][3], view[1][3], view[2][3]);
	glUniform3f(light_position, lightPos[0], lightPos[1], lightPos[2]);

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}




#endif