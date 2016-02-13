
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

#include "LoadShaders.h"

//PLEASE READ:
//Submitting as is because I have to move on to the next project...
//I was not able to get the sphere to display properly even though I have done the math in the functions
//Please provide as much feedback as possible as to why this isn't working.
//I have devoted a large amount of time to this project and would very much like to see it finished or at least understand
//what I am doing wrong
//
//Tasks:
//[SPHERE GEOMETRY]
//1. Origin-centered sphere with some color that reveals the shape and motion (done)
//2. nm and np - reasonable values (done)
//3.a Generate vertices (with correct geometry and matches the given algorithms and equations (done)
//3.b Stepping through rotations for nm + 1 sets (include duplicate) (stepping through rotations but not displaying properly)
//3.c Generating half circle input with np points = (done) if nm is 1 and np is an arbitrary amount the half circle will display
//4.d Proper index list for triangle strips and separated by primitive restart = (not done) can't close the gap
//[AXES ARROW GEOMETRY]
//axes arrow geometry(does not include transforming) = (done)
//5. Buffering everything (including index lst) in two VAO's to redraw the objects = (done)
//6. Applying the transformation matrix (m) in the vertex shader = (done)
//Sphere Translations [Cumulative]: (visual appearance, transformation, and matrix, setup)
//7. Positive and negative world translations = (done)
//8. Positive and negative local translations = (done)
//Sphere Rotations [Cumulative]: (visual appearance, transformation, and matrix setup)
//9.Positive and negative world rotations = (done)
//10 Positive and negative local rotations = (done)
//[ARROW TRANSFORMATIONS]
//11.arrow transformations to draw three world axes = (not done) unable to draw any geometry properly
//[GMTL Matrix Management]
//12.Initialize and set appropriate mState type for M = (done)




static enum RotationState
{
	Rpos_x,
	Rneg_x,
	Rpos_y,
	Rneg_y,
	Rpos_z,
	Rneg_z,
};

static enum TranslationState
{
	left_t,
	right_t,
	up_t,
	down_t,

};
gmtl::Matrix44f translate_Matrix;
gmtl::Matrix44f model_Matrix;
gmtl::Matrix44f rotation_Matrix;
gmtl::Matrix44f axis_Matrix;

const float rot_factor = gmtl::Math::deg2Rad(5.0f);
const float mov_factor = 0.1f;
static const float radius = 0.45f;
// Vertex array object


// Vertex buffer objects
unsigned int vao1, vao2, vao3, vao4;
unsigned int vbo1, vbo2, vbo3, vbo4; //vertex buffer object
unsigned int ibo1, ibo2, ibo3, ibo4;
static int sphereIndexSize;
static int axisIndexSize;



GLuint vertex_position;
GLuint vertex_color;

unsigned int render_model_matrix_loc;

GLenum errCode;
const GLubyte *errString;
static const double pi = 3.1415926;


std::vector<float> sphere_color_data;
std::vector<float> axis_color_data;


void GenerateCircleVerts(std::vector<float> &start_verts, int np)
{
	for (int i = 0; i < np; i++)
	{
		//get the current x_angle
		float theta = pi * (((float(i) / float(np - 1)) - 0.5f));
		float newX = radius * cos(theta);
		float newY = radius * sin(theta);
		float newZ = 0.0f;
		start_verts.push_back(newX); //x
		start_verts.push_back(newY); //y
		start_verts.push_back(newZ); //z
	}
}

void RotatePoints(std::vector<float> &vertices, int nm, int np)
{
	//calculate new x y z values for the point set around the y axis
	for (unsigned int i = 0; i < nm; ++i)
	{

		float phi = (2 * pi) * (float(i) / float(nm));

		for (int j = 0; j < (np * 3) + 1; j += 3) //np times
		{

			float x = vertices.at(j + 0); //need to grab a point and calculate it's new offset 
			float y = vertices.at(j + 1);
			float z = vertices.at(j + 2);


			float newX = x * cos(phi) - (z * sin(phi));//New_X = z * sin(phi) + x * cos(phi)
			float newY = y;
			float newZ = x * sin(phi) + z * sin(phi);//New_Z = z * cos(phi) – x * sin(phi) 

			vertices.push_back(newX);
			vertices.push_back(newY);
			vertices.push_back(newZ);
		}
	}
}

void GenerateColor(std::vector<float> &vertices)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		//sphere_color_data.push_back(.75);
		sphere_color_data.push_back(vertices.at(i));
	}
}

void GenerateColorAxis(std::vector<float> &vertices)
{
	for (int i = 0; i < vertices.size(); i++)
	{
		axis_color_data.push_back(vertices.at(i));
	}
}

void SetIndices(int nm, int np, std::vector<unsigned int> &indices)
{
	for (int i = 0; i < np; i++)
	{
		for (int j = 0; j < nm + 1; j++) //nm = 3
		{
			int start = i * np;
			if (j == start - 1)
				break;
			indices.push_back((2 * j) + start);
			indices.push_back((2 * j + np) + start);
			indices.push_back((2 * j + 1) + start);
			indices.push_back(2 * j + (np + 1) + start);
		}

		indices.push_back(0xFFFF);
	}

}


void printArray(std::vector<float> &vertices)
{
	int count = 0;
	for (int i = 0; i < vertices.size();)
	{
		count++;
		std::cout << count << "| " << " x: " << vertices[i++];
		std::cout << "| " << " y: " << vertices[i++];
		std::cout << "| " << " z: " << vertices[i++];
		std::cout << "| " << std::endl;
	}

}

void printIndices(std::vector<unsigned int> &info)
{
	int count = 0;
	for (int i = 0; i < info.size(); i++)
	{
		if ((i % (7) == 0))
			std::cout << std::endl;
		std::cout << "| " << info[i];


	}
	std::cout << std::endl;
}

void CreateSphere(int nm, int np, std::vector<float> &vertices, std::vector<unsigned int> &indices)
{
	GenerateCircleVerts(vertices, np);
	RotatePoints(vertices, nm, np);
	GenerateColor(vertices);
	SetIndices(nm, np, indices);
	//printIndices(indices);
	sphereIndexSize = indices.size();
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);

	glGenBuffers(1, &ibo1); //create 1 buffer name and store the generated buffer object name their
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1); //bind the buffer name to the target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
		indices.size() * sizeof(unsigned int), //size of buffer object
		&indices[0], //pointer to the data
		GL_STATIC_DRAW);


	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
		(vertices.size() * sizeof(float)) + (sphere_color_data.size() * sizeof(float)), //this is the total size of the buffer
		0, //address of the content to be updated nothing because we are setting it in the subData buffers
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER,
		0, //offset where data replacement will begin
		vertices.size() * sizeof(float), //size
		&vertices[0]);//address of the content to be updated

	glBufferSubData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		sphere_color_data.size() * sizeof(float),
		&sphere_color_data[0]); //address of the content to be updated

	//Set up the vertex attributes pointer
	glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, ((void*)0)); // vertex buffer object
	glEnableVertexAttribArray(vertex_position);
	glVertexAttribPointer(vertex_color, 3, GL_FLOAT, GL_FALSE, 0, (const void *)(vertices.size() * sizeof(float)));
	glEnableVertexAttribArray(vertex_color);

	glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
	glBindVertexArray(0); // Disable our Vertex Buffer Object
}

void UnwrapPointToArray(gmtl::Point3f &point, std::vector<float> &array)
{
	for (int i = 0; i < 3; i++)
	{
		array.push_back(point[i]);
	}
}


//create axis
void CreateAxis(int nm, std::vector<float> &vertices, std::vector<unsigned int> &indices, char loc)
{
	int np = 4;

	if (loc == 'x')
	{
		gmtl::Point3f p0(.0f, .0f, .0f);
		gmtl::Point3f p1(.9f, .0f, .0f);
		gmtl::Point3f p2(.9f, -.05f, .0f);
		gmtl::Point3f p3(.98f, .0f, .0f);
		UnwrapPointToArray(p0, vertices);
		UnwrapPointToArray(p1, vertices);
		UnwrapPointToArray(p2, vertices);
		UnwrapPointToArray(p3, vertices);
		RotatePoints(vertices, 1, 4);
		GenerateColorAxis(vertices);
		SetIndices(nm, np, indices);
		axisIndexSize = indices.size();
		glGenVertexArrays(1, &vao2);
		glBindVertexArray(vao2);

		glGenBuffers(1, &ibo2); //create 1 buffer name and store the generated buffer object name their
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2); //bind the buffer name to the target
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
			indices.size() * sizeof(unsigned int), //size of buffer object
			&indices[0], //pointer to the data
			GL_STATIC_DRAW);


		glGenBuffers(1, &vbo2);
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
		glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
			(vertices.size() * sizeof(float)) + (axis_color_data.size() * sizeof(float)), //this is the total size of the buffer
			0, //address of the content to be updated nothing because we are setting it in the subData buffers
			GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER,
			0, //offset where data replacement will begin
			vertices.size() * sizeof(float), //size
			&vertices[0]);//address of the content to be updated

		glBufferSubData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(float),
			axis_color_data.size() * sizeof(float),
			&axis_color_data[0]); //address of the content to be updated

		//Set up the vertex attributes pointer
		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // vertex buffer object
		glEnableVertexAttribArray(vertex_position);
		glVertexAttribPointer(vertex_color, 3, GL_FLOAT, GL_FALSE, 0, (const void *)(vertices.size() * sizeof(float)));
		glEnableVertexAttribArray(vertex_color);
		glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
		glBindVertexArray(0); // Disable our Vertex Buffer Object  
	}
	else if (loc == 'y')
	{
		gmtl::Point3f p0(.0f, .0f, .0f);
		gmtl::Point3f p1(.0f, .9f, .0f);
		gmtl::Point3f p2(.05f, .9f, .0f);
		gmtl::Point3f p3(.0f, .98f, .0f);
		UnwrapPointToArray(p0, vertices);
		UnwrapPointToArray(p1, vertices);
		UnwrapPointToArray(p2, vertices);
		UnwrapPointToArray(p3, vertices);
		RotatePoints(vertices, nm, np);
		GenerateColorAxis(vertices);
		SetIndices(nm, np, indices);
		axisIndexSize = indices.size();
		glGenVertexArrays(1, &vao3);
		glBindVertexArray(vao3);

		glGenBuffers(1, &ibo3); //create 1 buffer name and store the generated buffer object name their
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo3); //bind the buffer name to the target
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
			indices.size() * sizeof(unsigned int), //size of buffer object
			&indices[0], //pointer to the data
			GL_STATIC_DRAW);


		glGenBuffers(1, &vbo3);
		glBindBuffer(GL_ARRAY_BUFFER, vbo3);
		//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
		glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
			(vertices.size() * sizeof(float)) + (axis_color_data.size() * sizeof(float)), //this is the total size of the buffer
			0, //address of the content to be updated nothing because we are setting it in the subData buffers
			GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER,
			0, //offset where data replacement will begin
			vertices.size() * sizeof(float), //size
			&vertices[0]);//address of the content to be updated

		glBufferSubData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(float),
			axis_color_data.size() * sizeof(float),
			&axis_color_data[0]); //address of the content to be updated

		//Set up the vertex attributes pointer
		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // vertex buffer object
		glEnableVertexAttribArray(vertex_position);
		glVertexAttribPointer(vertex_color, 3, GL_FLOAT, GL_FALSE, 0, (const void *)(vertices.size() * sizeof(float)));
		glEnableVertexAttribArray(vertex_color);
		glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
		glBindVertexArray(0); // Disable our Vertex Buffer Object
		std::vector<float> axis_vertices_y;
	}
	else if (loc == 'z')
	{
		gmtl::Point3f p0(.0f, .0f, .0f);
		gmtl::Point3f p1(.0f, .0f, .0f);
		gmtl::Point3f p2(.0f, .00f, .9f);
		gmtl::Point3f p3(.0f, .0f, -.98f);
		UnwrapPointToArray(p0, vertices);
		UnwrapPointToArray(p1, vertices);
		UnwrapPointToArray(p2, vertices);
		UnwrapPointToArray(p3, vertices);
		RotatePoints(vertices, nm, np);
		GenerateColorAxis(vertices);
		SetIndices(nm, np, indices);
		axisIndexSize = indices.size();
		glGenVertexArrays(1, &vao4);
		glBindVertexArray(vao4);

		glGenBuffers(1, &ibo4); //create 1 buffer name and store the generated buffer object name their
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo4); //bind the buffer name to the target
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
			indices.size() * sizeof(unsigned int), //size of buffer object
			&indices[0], //pointer to the data
			GL_STATIC_DRAW);


		glGenBuffers(1, &vbo4);
		glBindBuffer(GL_ARRAY_BUFFER, vbo4);
		//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
		glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
			(vertices.size() * sizeof(float)) + (axis_color_data.size() * sizeof(float)), //this is the total size of the buffer
			0, //address of the content to be updated nothing because we are setting it in the subData buffers
			GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER,
			0, //offset where data replacement will begin
			vertices.size() * sizeof(float), //size
			&vertices[0]);//address of the content to be updated

		glBufferSubData(GL_ARRAY_BUFFER,
			vertices.size() * sizeof(float),
			axis_color_data.size() * sizeof(float),
			&axis_color_data[0]); //address of the content to be updated

		//Set up the vertex attributes pointer
		glVertexAttribPointer(vertex_position, 3, GL_FLOAT, GL_FALSE, 0, 0); // vertex buffer object
		glEnableVertexAttribArray(vertex_position);
		glVertexAttribPointer(vertex_color, 3, GL_FLOAT, GL_FALSE, 0, (const void *)(vertices.size() * sizeof(float)));
		glEnableVertexAttribArray(vertex_color);
		glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
		glBindVertexArray(0); // Disable our Vertex Buffer Object  
		std::vector<float> axis_vertices_z;
	}






}


void DrawSphere(unsigned int &v, unsigned int &b, unsigned int &i)
{
	glBindVertexArray(v);
	glBindBuffer(GL_ARRAY_BUFFER, b);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, &model_Matrix[0][0]);
	glEnable(GL_PRIMITIVE_RESTART); //Initialize primitive restarts
	glPrimitiveRestartIndex(0xFFFF); //set the primitive restart index	
	glDrawElements(GL_TRIANGLE_STRIP, sphereIndexSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0); // Unbind our Vertex Array Object 
}

void DrawAxis(unsigned int &v, unsigned int &b, unsigned int &i)
{
	glBindVertexArray(v);
	glBindBuffer(GL_ARRAY_BUFFER, b);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);

	glUniformMatrix4fv(render_model_matrix_loc, 1, GL_FALSE, &axis_Matrix[0][0]);
	glEnable(GL_PRIMITIVE_RESTART); //Initialize primitive restarts
	glPrimitiveRestartIndex(0xFFFF); //set the primitive restart index	
	glDrawElements(GL_TRIANGLE_STRIP, axisIndexSize, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0); // Unbind our Vertex Array Object 
}

void Init()
{
	glEnable(GL_DEPTH_TEST);
	// OpenGL background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Load/compile/link shaders and set to use for rendering
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "Cube_Vertex_Shader.vert" },
		{ GL_FRAGMENT_SHADER, "Cube_Fragment_Shader.frag" },
		{ GL_NONE, NULL }
	};

	unsigned int program = LoadShaders(shaders);
	vertex_position = glGetAttribLocation(program, "vertexPosition");
	vertex_color = glGetAttribLocation(program, "vertexColor");
	//Get the shader parameter locations to connect them to the
	//respective buffers later.
	glUseProgram(program);

	gmtl::identity(model_Matrix);

	// Find location needed to communicate value to vertex shader
	render_model_matrix_loc = glGetUniformLocation(program, "Matrix");
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	DrawSphere(vao1, vbo1, ibo1);
	DrawAxis(vao2, vbo2, ibo2);
	DrawAxis(vao3, vbo3, ibo3);
	DrawAxis(vao4, vbo4, ibo4);


	glutSwapBuffers();


	//Check for any errors and print the error string.
	//NOTE: The string returned by gluErrorString must not be altered.
	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		std::cout << "OpengGL Error: " << errString << std::endl;
	}
}

void DoRotate(RotationState state, char transform)
{

	switch (state)
	{
#pragma region PosXWorldLocalRotation
	case Rpos_x:
		if (transform == 'l')
		{

			rotation_Matrix.set(
				cos(rot_factor), 0.0, sin(rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(rot_factor), 0.0, cos(rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = rotation_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{

			rotation_Matrix.set(
				cos(rot_factor), 0.0, sin(rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(rot_factor), 0.0, cos(rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * rotation_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion

#pragma region NegXWorldLocalRotation
	case Rneg_x:
		if (transform == 'l')
		{
			rotation_Matrix.set(
				cos(-rot_factor), 0.0, sin(-rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(-rot_factor), 0.0, cos(-rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = rotation_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			rotation_Matrix.set(
				cos(-rot_factor), 0.0, sin(-rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(-rot_factor), 0.0, cos(-rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * rotation_Matrix;
			glutPostRedisplay();
			break;
#pragma endregion
	case Rpos_y:
		if (transform == 'l')
		{
			rotation_Matrix.set(
				cos(rot_factor), 0.0, sin(rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(rot_factor), 0.0, 1.0, cos(rot_factor),
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = rotation_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{

			rotation_Matrix.set(
				cos(rot_factor), 0.0, sin(rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(rot_factor), 0.0, 1.0, cos(rot_factor),
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * rotation_Matrix;
			glutPostRedisplay();
			break;
		}
	case Rneg_y:
		if (transform == 'l')
		{
			rotation_Matrix.set(
				cos(-rot_factor), 0.0, sin(-rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(-rot_factor), 0.0, 1.0, cos(-rot_factor),
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = rotation_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			rotation_Matrix.set(
				cos(-rot_factor), 0.0, sin(-rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(-rot_factor), 0.0, 1.0, cos(-rot_factor),
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * rotation_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma region requiredPOSZWorldLocalRotation
	case Rpos_z:
		if (transform == 'l')
		{
			rotation_Matrix.set(
				cos(rot_factor), sin(rot_factor), 0.0, 0.0,
				-sin(rot_factor), cos(rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = rotation_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{

			rotation_Matrix.set(
				cos(rot_factor), sin(rot_factor), 0.0, 0.0,
				-sin(rot_factor), cos(rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * rotation_Matrix;
			glutPostRedisplay();
			break;
		}
	case Rneg_z:
		if (transform == 'l')
		{
			std::cout << "local z rotation" << std::endl;
			rotation_Matrix.set(
				cos(-rot_factor), sin(-rot_factor), 0.0, 0.0,
				-sin(-rot_factor), cos(-rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * rotation_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			std::cout << "world z rotation" << std::endl;
			rotation_Matrix.set(
				cos(-rot_factor), sin(-rot_factor), 0.0, 0.0,
				-sin(-rot_factor), cos(-rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = rotation_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}

#pragma endregion

	default:
		break;
		}
	}
}

void DoTranslate(TranslationState state, char transform)
{
#pragma region notrequired
	switch (state)
	{
	case up_t:
		if (transform == 'l')
		{
			std::cout << "move up" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = translate_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{

			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix *= translate_Matrix;
			glutPostRedisplay();
			break;
		}

	case down_t:
		if (transform == 'l')
		{
			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, -mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = translate_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{

			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, -mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix *= translate_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion

#pragma region PosNegWorldLocalXTranslation
	case left_t:
		if (transform == 'l')
		{
			std::cout << "neg x local" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, -mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
				);
			model_Matrix = translate_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			std::cout << "neg x world" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, -mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * translate_Matrix;
			glutPostRedisplay();
			break;
		}
	case right_t:
		if (transform == 'l')
		{
			std::cout << "pos x local" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = translate_Matrix * model_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			std::cout << "pos x world" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			model_Matrix = model_Matrix * translate_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	//put the caps lock on and press the big letters to do World transform rotation/translation
	switch (key)
	{
		//local rotations use lower case key
		//world rotations user upper case key

		//rotations
		//r = x pos
		//t = x negative
		//q = z positive
		//e = z negative
#pragma region rotate
	case 'R':
		DoRotate(Rpos_x, 'w');
		break;
	case 'r':
		DoRotate(Rpos_x, 'l');
		break;
	case 'T':
		DoRotate(Rneg_x, 'w');
		break;
	case 't':
		DoRotate(Rneg_x, 'l');
		break;
	case 'Q':
		DoRotate(Rpos_z, 'w');
		break;
	case 'q':
		DoRotate(Rpos_z, 'l');
		break;
	case 'E'://req neg rot z world
		DoRotate(Rneg_z, 'w');
		break;
	case 'e'://req neg rot z local
		DoRotate(Rneg_z, 'l');
		break;
#pragma endregion rotate
#pragma region translate
		//local translations use lower case key
		//world translations user upper case key

		//a = x positive
	case 'W':
		DoTranslate(up_t, 'w');
		break;
	case 'w':
		DoTranslate(up_t, 'l');
		break;
	case 'A'://req neg tran world x
		DoTranslate(left_t, 'w');
		break;
	case 'a'://req neg tran local x
		DoTranslate(left_t, 'l');
		break;
	case 'S':
		DoTranslate(down_t, 'w');
		break;
	case 's':
		DoTranslate(down_t, 'l');
		break;
	case 'D'://req pos tran world x
		DoTranslate(right_t, 'w');
		break;
	case 'd'://req pos tran local x
		DoTranslate(right_t, 'l');
		break;
#pragma endregion translate
	case 033 /* Escape key */:
		exit(EXIT_SUCCESS);
		break;
	}

}

int main(int argc, char** argv)
{
	// For more details about the glut calls, 
	// refer to the OpenGL Introduction handout.


	std::vector<float> sphere_vertices;
	std::vector<unsigned int> sphere_indices;
	std::vector<float> axis_vertices_x;
	std::vector<unsigned int> axis_indices_x;
	std::vector<float> axis_vertices_y;
	std::vector<unsigned int> axis_indices_y;
	std::vector<unsigned int> axis_indices_z;
	std::vector<float> axis_vertices_z;

	int sphere_nm = 45;
	int sphere_np = 45;
	int axis_nm = 25;
	//Initialize the freeglut library
	glutInit(&argc, argv);

	//Specify the display mode
	glutInitDisplayMode(GLUT_RGBA);

	//Set the window size/dimensions
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(1000, 100);

	// Specify OpenGL version and core profile.
	// For newer cards, use (3, 3) or (4, 0).
	// For old cards or programming style, may have to remove calls below.
	// However, we only want to use recent versions, so avoid removing if possible!
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Assignment_02");

	glewExperimental = GL_TRUE;

	if (glewInit())
		exit(EXIT_FAILURE);

	Init();
	CreateSphere(sphere_nm, sphere_np, sphere_vertices, sphere_indices);
	CreateAxis(axis_nm, axis_vertices_x, axis_indices_x, 'x');//x
	CreateAxis(axis_nm, axis_vertices_y, axis_indices_y, 'y');//x
	CreateAxis(axis_nm, axis_vertices_z, axis_indices_z, 'z');


	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	//Transfer the control to glut processing loop.
	glutMainLoop();

	return 0;
}
