#pragma once

#include "Texture.h"
#include "LoadShaders.h"
#include "Geometry.h"
#include "Transforms.h"
#include "Particles.h"

#pragma region Physics

Particles flock(10, false);
Particles preds(10, true);
float dispersion = 1.0f;
float centering = 1.0f;
float alignment = 1.0f;
GLuint bird_vao;
GLuint dbo, cbo, abo;
float dForce[6];

#pragma endregion

#pragma region Stereo

enum Eye { LEFT_EYE, RIGHT_EYE };
const float IPD_D2 = 0.07f / 2;       // Interpupillary distance divided by 2
const float VF_NEAR = .05f;             // Distance to near clipping plane
const float VF_FAR = 100;           // Distance to far clipping plane
const float VF_FD = 2.5;           // Distance to focal plane (focal depth)
const float VF_LEFT = -2.05;          // Left clipping coordinate at focal depth
const float VF_RIGHT = 2.05;          // Right clipping coordinate at focal depth
const float VF_BOTTOM = -1.6;           // Bottom clipping coordinate at focal depth
const float VF_TOP = 0.68;          // Top clipping coordinate at focal depth
const float VF_S = VF_NEAR / VF_FD;  // Scale factor
bool draw_left_eye = true;         // Draw left-eye image or right-eye image toggled by the key 's'

#pragma endregion

GLuint
vao1, vao2, vao3, vao4, vao5, vao6, vao7,
vbo1, vbo2, vbo3, vbo4, vbo5, vbo6, vbo7,
ibo1, ibo2, ibo3, ibo4, ibo5, ibo6, ibo7;

#pragma region Shader Variables

GLuint PROGRAM1, PROGRAM2, PROGRAM3;
//===Lights===
GLuint
light_ambient,
light_color,
light_shininess,
light_strength,
light_position,
world_position1,
world_position2,
hf, df, sf;
//===End Lights

//===Matrix Positions===
GLuint mvpID1, mvID1, eye_position1;
GLuint mvpID2, mvID2, eye_position2;
GLuint mvpID3, g_ModelViewProjection, nlen;
//=== END Matrix Positions===

//===Geometry attributes
GLuint vertex_color1, vertex_UV1,
vertex_normal1, texture_color1, vertex_position1;

GLuint vertex_color2, vertex_UV2,
vertex_normal2, texture_color2, vertex_position2;

GLuint vertex_color3, vertex_UV3,
vertex_normal3, texture_color3, vertex_position3;

//===End Geometry attributes

#pragma endregion

#pragma region Lights

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
	x = X;
	y = Y;
	z = Z;
	shine = Shine;
	strength = Strength;
	Pos = gmtl::Point3f(X, Y, Z);
	sfac = 1;
	hfac = .25f;
	dfac = 1;
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

bool lightFlag = true;

#pragma endregion

#pragma region Time

class Timer
{
public:

	float time;
	int currentTime;
	float previousTime;
	float deltaTime;

};
Timer Time;

#pragma endregion



#pragma region Textures
GLuint starTex,
texh6, texw6;
unsigned char *img_data6;
GLuint futureTex, texh7, texw7;
unsigned char *img_data7;

GLuint
texw1, texh1,
texh2, texw2,
texh3, texw3,
texh4, texw4,
texh5, texw5;
unsigned char
*img_data1, *img_data2, *img_data3, *img_data4, *img_data5;
GLuint
woodTexture, backgroundTexture,
checkerboardTexture, checkerboard,
skyTexture, earthTexture;
bool showTex = 0;
float fovFac = 2.5F;

#pragma endregion

float mouse_long, mouse_lat, mouse_azi, mouse_ele;
int oldX, oldY, oldZ;
int oldX2, oldY2, oldZ2;
bool mouseMoving = false;

GLenum errCode;
const GLubyte *errString;

//-----/Debug Functions/-----//
void CheckShader(int num);
void CheckError();
template <typename Type> void print(Type a);

//-----/Init Functions/-----//
void init();
void SetupShaders();
void SetupTextures();
void SetupForceVisualization();
void BuildShader(GLuint &shader);
void ChangeLights(Light c);
void CreateTexture(GLuint &name, unsigned char* &data, char* path, GLuint &w, GLuint &h);

//-----/Draw Functions/-----//
void RenderScene(int eye);
void DrawObject(GLuint &v, GLuint &b, GLuint &i, int indexSize, gmtl::Matrix44f t);
void DrawGlobe(GLuint tex, gmtl::Matrix44f model);
void DrawTeapot(GLuint tex, gmtl::Matrix44f model);
void DrawSkybox(GLuint tex, gmtl::Matrix44f model);
void DrawPred(GLuint tex, Bird b);
void DrawBird(GLuint tex, Bird b, gmtl::Matrix44f birdRot);
void DrawForce(Bird b);
void DrawAxis(GLuint tex, gmtl::Matrix44f rot);

//-----/Callback functions/-----//
void Keyboard();
void Mouse();
void Motion();
void Update();
void Reshape(int width, int height);

//================================================================================================================================================================================
template <typename Type> void print(Type a)
{
	std::cout << a << std::endl;

}

void DrawBird(GLuint tex, Bird b)
{
	gmtl::Matrix44f birdRot = T_s2_w * b._T_o_r;


	gmtl::Matrix44f body = birdRot * Scale(.05f * b._mass);
	gmtl::Matrix44f topPropellerBase = body * Rotation(-90, 'z') * Scale(1.5f);
	gmtl::Matrix44f topPropeller = topPropellerBase * Translate(1, 'y') * T_j2_b;
	gmtl::Matrix44f nose = body * Translate(.5f, 'y') * Scale(8.0f);
	gmtl::Matrix44f tail = body *  Translate(-1, 'y') * Rotation(180, 'x') * Scale(4.0f);
	gmtl::Matrix44f frontPropeller = nose * Translate(.25f, 'y') * Scale(0.15f) * T_j2_b;

	glBindTexture(GL_TEXTURE_2D, tex);
	//body of helicopter
	DrawObject(vao1, vao1, ibo1, GetSize('s'), body);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
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
	glBindTexture(GL_TEXTURE_2D, 0);


}

void DrawPred(GLuint tex, Bird b)
{
	gmtl::Matrix44f birdRot = T_s3_w * b._T_o_r;
	gmtl::Matrix44f body = birdRot * Scale(.035f * b._mass);
	gmtl::Matrix44f topPropellerBase = body * Rotation(-90, 'z') * Scale(1.5f);
	gmtl::Matrix44f topPropeller = topPropellerBase * Translate(1, 'y') * T_j2_b;
	gmtl::Matrix44f nose = body * Translate(0.70f, 'y') * Rotation(5, 'z') * Scale(6.0f);

	gmtl::Matrix44f tail = body * Translate(-1.25f, 'y') * Rotation(180, 'y') * Scale(4.0f);
	gmtl::Matrix44f frontPropeller1 = nose * Translate(.25f, 'y') * Scale(0.25f) * T_j2_b;
	gmtl::Matrix44f frontPropeller2 = tail * Scale(0.15f) * T_j2_b;

	glBindTexture(GL_TEXTURE_2D, tex);
	{//body of helicopter
		DrawObject(vao1, vao1, ibo1, GetSize('s'), body);

	}glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
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

void DrawAxis(GLuint tex, gmtl::Matrix44f rot)
{
	glBindTexture(tex, 0);
	DrawObject(vao2, vao2, ibo2, GetSize('a'), rot * Rotation(90, 'x'));
	DrawObject(vao2, vao2, ibo2, GetSize('a'), rot * Scale(1.0f));
	DrawObject(vao2, vao2, ibo2, GetSize('a'), rot * Rotation(-90, 'z'));
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawGlobe(GLuint tex, gmtl::Matrix44f Model)
{

	if (!showTex)
	{

		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(tex, 0);
	}
	else {		
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		glUniform1i(backgroundTexture, 0);
	}
	DrawObject(vao1, vbo1, ibo1, GetSize('s'), Model);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawSkybox(GLuint tex, gmtl::Matrix44f model)
{
	DrawGlobe(tex, model);
}

void DrawForce(Bird b)
{
	gmtl::Matrix44f Model = T_s2_w * b._T_o_r;
	gmtl::Matrix44f View = GetCamera();
	gmtl::Matrix44f Projection = GetProjection();
	//invert the camera for View matrix
	gmtl::invert(View);
	gmtl::Matrix44f ModelViewProjection = Projection * View * Model;
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
	glUniformMatrix4fv(mvpID1, 1, GL_FALSE, &ModelViewProjection[0][0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dForce), &dForce[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glDrawArrays(GL_LINES, 0, 2);


}

void DrawTeapot(GLuint tex, gmtl::Matrix44f model)
{
	glBindTexture(GL_TEXTURE_2D, tex);

	glUniform1i(tex, 0);
	DrawObject(vao5, vbo5, ibo5, GetSize('t'), model);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawCube(GLuint tex, gmtl::Matrix44f model)
{
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glUniform1i(tex, 0);
	DrawObject(vao6, vbo6, ibo6, GetSize('c'), model);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawObject(GLuint &v, GLuint &b, GLuint&i, int indexSize, gmtl::Matrix44f Model)
{
	gmtl::Matrix44f View = GetCamera();
	gmtl::Matrix44f Projection = GetProjection();
	//invert the camera for View matrix
	gmtl::invert(View);

	gmtl::Matrix44f ModelViewProjection = Projection * View * Model;
	gmtl::Matrix44f NormalMatrix = View * Model;

	gmtl::Point3f l(0, 5, 1);
	gmtl::Point3f w(View[0][3], View[1][3], View[2][3]);

	light.Pos = View * l;


	//send position uniforms
	glUniformMatrix4fv(mvpID1, 1, GL_FALSE, &ModelViewProjection[0][0]);
	glUniformMatrix4fv(mvID1, 1, GL_FALSE, &NormalMatrix[0][0]);
	glUniform3f(world_position1, w[0], w[1], w[2]);
	glUniform1i(texture_color1, 0);
	glUniform1i(texture_color2, 0);
	glUniform1i(texture_color3, 0);
	glUniformMatrix4fv(g_ModelViewProjection, 1, GL_FALSE, &ModelViewProjection[0][0]);
	glUniform1f(nlen, .1250f);

	//send light uniforms
	glUniform3f(light_position, light.Pos[0], light.Pos[1], light.Pos[2]);
	glUniform1f(light_shininess, light.shine);
	glUniform1f(light_strength, light.strength);
	glUniform1f(sf, light.sfac);
	glUniform1f(hf, light.hfac);
	glUniform1f(df, light.dfac);

	glBindVertexArray(v);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(9000);
	glDrawElements(GL_TRIANGLE_STRIP, indexSize, GL_UNSIGNED_INT, (void*)0);
	glDisable(GL_PRIMITIVE_RESTART);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void SetupForceVisualization()
{
	glGenVertexArrays(1, &bird_vao);
	glGenBuffers(1, &dbo);

}

void SetupTextures()
{
	//==== SETUP TEXTURES ====


	CreateTexture(woodTexture, img_data1, "textures/wood.ppm", texh1, texw1);
	CreateTexture(backgroundTexture, img_data2, "textures/background.ppm", texh2, texw2);
	CreateTexture(checkerboardTexture, img_data3, "textures/checkerboard.ppm", texh3, texw3);
	CreateTexture(skyTexture, img_data4, "textures/sky.ppm", texh4, texw4);
	CreateTexture(earthTexture, img_data5, "textures/couch.ppm", texh5, texw5);
	CreateTexture(starTex, img_data6, "textures/star.ppm", texh6, texw6);
	CreateTexture(futureTex, img_data7, "textures/honey.ppm", texh7, texw7);
	//==== END SETUP TEXTURES ====
}

void CreateTexture(GLuint &name, unsigned char* &data, char* path, GLuint &texh, GLuint &texw)
{
	//==== SETUP TEXTURES ====
	glGenTextures(1, &name);
	LoadPPM(path, &texw, &texh, &data, 1);
	glBindTexture(GL_TEXTURE_2D, name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texh, texw, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SetupShaders()
{
	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/v_shader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/f_shader.frag" },
		{ GL_NONE, NULL }
	};

	PROGRAM1 = LoadShaders(shaders);
	glUseProgram(PROGRAM1);
	//==== VERTEX SHADER LOCATION BINDINGS ====
	vertex_position1 = glGetAttribLocation(PROGRAM1, "VertexPosition");
	vertex_color1 = glGetAttribLocation(PROGRAM1, "VertexColor");
	vertex_UV1 = glGetAttribLocation(PROGRAM1, "VertexUV");
	vertex_normal1 = glGetAttribLocation(PROGRAM1, "VertexNormal");
	mvpID1 = glGetUniformLocation(PROGRAM1, "MVPMatrix");
	mvID1 = glGetUniformLocation(PROGRAM1, "NormalMatrix");
	//==== END VERTEX SHADER LOCATION BINDINGS ====

	//==== FRAGMENT SHADER LOCATION BINDINGS ====
	eye_position1 = glGetUniformLocation(PROGRAM1, "EyePosition");
	world_position1 = glGetUniformLocation(PROGRAM1, "WorldPosition");
	light_position = glGetUniformLocation(PROGRAM1, "LightPosition");
	texture_color1 = glGetUniformLocation(PROGRAM1, "TextureColor");
	light_color = glGetUniformLocation(PROGRAM1, "LightColor");
	light_shininess = glGetUniformLocation(PROGRAM1, "Shininess");
	light_strength = glGetUniformLocation(PROGRAM1, "Strength");
	hf = glGetUniformLocation(PROGRAM1, "hFactor");
	df = glGetUniformLocation(PROGRAM1, "dFactor");
	sf = glGetUniformLocation(PROGRAM1, "sFactor");
	//==== END FRAGMENT SHADER BINDINGS ====

	glUseProgram(0);

	ShaderInfo fuzShaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/normal_shader.vert" },
		{ GL_GEOMETRY_SHADER, "shaders/normal_shader.geom" },
		{ GL_FRAGMENT_SHADER, "shaders/normal_shader.frag" },
		{ GL_NONE, NULL }
	};

	PROGRAM2 = LoadShaders(fuzShaders);
	glUseProgram(PROGRAM2);
	//==== VERTEX SHADER LOCATION BINDINGS ====
	vertex_position2 = glGetAttribLocation(PROGRAM2, "VertexPosition");
	vertex_color2 = glGetAttribLocation(PROGRAM2, "VertexColor");
	vertex_UV2 = glGetAttribLocation(PROGRAM2, "VertexUV");
	vertex_normal2 = glGetAttribLocation(PROGRAM2, "VertexNormal");
	//==== END VERTEX SHADER LOCATION BINDINGS ====	 

	//==== FRAGMENT SHADER LOCATION BINDINGS ==== 
	//==== END FRAGMENT SHADER LOCATION BINDINGS ==== 

	//==== GEOMETRY SHADER LOCATION BINDINGS ==== 
	nlen = glGetUniformLocation(PROGRAM2, "normal_length");
	g_ModelViewProjection = glGetUniformLocation(PROGRAM2, "g_MVP");
	//==== END GEOMETRY SHADER LOCATION BINDINGS ====	
	glUseProgram(0);

	ShaderInfo dShaders[] =
	{
		{ GL_VERTEX_SHADER, "shaders/v_shader.vert" },
		{ GL_FRAGMENT_SHADER, "shaders/sky_shader.frag" },
		{ GL_NONE, NULL }
	};

	PROGRAM3 = LoadShaders(dShaders);
	glUseProgram(PROGRAM3);
	//==== VERTEX SHADER LOCATION BINDINGS ====
	mvpID3 = glGetUniformLocation(PROGRAM3, "MVPMatrix");
	vertex_position3 = glGetAttribLocation(PROGRAM3, "VertexPosition");
	vertex_color3 = glGetAttribLocation(PROGRAM3, "VertexColor");
	vertex_UV3 = glGetAttribLocation(PROGRAM3, "VertexUV");
	vertex_normal3 = glGetAttribLocation(PROGRAM3, "VertexNormal");
	//==== END VERTEX SHADER LOCATION BINDINGS ====

	texture_color3 = glGetUniformLocation(PROGRAM3, "TextureColor");
	CheckShader(3);
	glUseProgram(0);

}

void CheckShader(int val)
{
	switch (val)
	{
	case 1:
		std::cout << "location of mvpID: " << mvpID1 << std::endl;
		std::cout << "location of mvID: " << mvID1 << std::endl;
		std::cout << "location of eye_position: " << eye_position1 << std::endl;
		std::cout << "location of world_position: " << world_position1 << std::endl;
		std::cout << "location of light_position: " << light_position << std::endl;
		std::cout << "location of texture_color: " << texture_color1 << std::endl;
		std::cout << "location of light_color: " << light_color << std::endl;
		std::cout << "location of light_shininess: " << light_shininess << std::endl;
		std::cout << "location of light_strength: " << light_strength << std::endl;
		std::cout << "location of vertex_position: " << vertex_position1 << std::endl;
		std::cout << "location of vertex_color: " << vertex_color1 << std::endl;
		std::cout << "location of vertex_UV: " << vertex_UV1 << std::endl;
		std::cout << "location of vertex_normal: " << vertex_normal1 << std::endl;
		break;
	case 2:
		std::cout << "location of mvpID: " << mvpID2 << std::endl;
		std::cout << "location of texture_color: " << texture_color2 << std::endl;
		std::cout << "location of vertex_position: " << vertex_position2 << std::endl;
		std::cout << "location of vertex_color: " << vertex_color2 << std::endl;
		std::cout << "location of vertex_UV: " << vertex_UV2 << std::endl;
		std::cout << "location of vertex_normal: " << vertex_normal2 << std::endl;
		break;
	case 3:
		std::cout << "location of mvpID: " << mvpID3 << std::endl;
		std::cout << "location of texture_color: " << texture_color3 << std::endl;
		std::cout << "location of vertex_position: " << vertex_position3 << std::endl;
		std::cout << "location of vertex_color: " << vertex_color3 << std::endl;
		std::cout << "location of vertex_UV: " << vertex_UV3 << std::endl;
		std::cout << "location of vertex_normal: " << vertex_normal3 << std::endl;
		break;


	}
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
	case 's':      // [Development mode only] 's' toggles between left- and right-eye image
		draw_left_eye = !draw_left_eye;
		printf("Showing image for [%s]\n", draw_left_eye ? "Left Eye" : "Right Eye");
		glutPostRedisplay();
		break;
	case 'F':
		fovFac += 0.1f;
		std::cout << "fovFac: " << fovFac << std::endl;
		break;
	case 'f':
		fovFac -= 0.1f;
		std::cout << "fovFac: " << fovFac << std::endl;
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
		T_teaBase_w *= Rotation(5, 'y');
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
		std::cout << "hemisphere lighting: " << light.hfac << std::endl;
		break;
	case 'H':
		light.hfac += 0.1f;
		std::cout << "hemisphere lighting: " << light.hfac << std::endl;
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


	}

}

void Mouse(int button, int state, int x, int y)
{
	mouseMoving = false;
	if (state == GLUT_LEFT_BUTTON)
	{ 
			oldX = x;
			oldY = y;
			oldX2 = x;
			oldY2 = y;
		mouseMoving = true;
	}
	if (button == 4)
	{
		zInc += .05f;
		zInc2 += .05f;
	}
	else if (button == 3)
	{
		zInc -= .05f;
		zInc2 -= .05f;
	}
}

void Motion(int x, int y)
{
	if (mouseMoving)
	{
		if (cameraState != Camera2)
		{

			mouse_long += (x - oldX);
			rLong = Rotation(mouse_long, 'y');
			mouse_lat += (y - oldY);
			rLat = Rotation(mouse_lat, 'x');
			oldX = x;oldY = y;
			
		}
		else  
		{
			mouse_azi += (x - oldX2 );
			mouse_ele += (y - oldY2 );
			rAzi = Rotation(mouse_azi, 'x');
			rEle = Rotation(mouse_ele, 'y');
			oldX2 = x;
			oldY2 = y;

		}

	}

	//reset the oldX and Y values to prevent the sphere from resetting
	//itself after clicking a new click event
	

}
