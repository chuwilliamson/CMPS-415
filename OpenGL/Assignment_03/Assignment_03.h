#ifndef __ASSIGNMENT_03_H__
#define __ASSIGNMENT_03_H__


unsigned int vao1, vao2, vao3;
unsigned int vbo1, vbo2, vbo3;
unsigned int ibo1, ibo2, ibo3;

bool cam1 = true;
bool cam2 = false;
bool mouseMoving = false;
int mouse_long, mouse_lat, mouse_azi, mouse_ele;
int oldX, oldY, oldZ;
float width = 3.0f;
float depth = 3.0f;

bool camera1set = true;
bool camera2set = false;
float sphereRadius = .5f;

gmtl::Matrix44f rLong, rLat;
gmtl::Matrix44f rAzi, rEle, rZ;

GLenum errCode;
const GLubyte *errString;
//Frames
gmtl::Matrix44f M_world, M_sphere, M_axis, M_r, M_o, M_bird, camera_1, camera_2;
//M describes the sphere frame with respect to the world frame
//transformations
gmtl::Matrix44f T_s_w, T_c1_w, T_r_w, T_o_r, T_b_o, T_j1_o, T_j2_o, T_c2_o, T_other_j1, T_other_j2;
gmtl::Matrix44f model, view, projection;
gmtl::Quatf qX, qY, qZ;


bool showSphere = true;
bool showAxis = true;

int sphereIndexSize;
int axisIndexSize;
int coneIndexSize;

using namespace std;
//vertex attribute locations
GLuint vertex_position;
GLuint vertex_color;

//uniform variable locations
unsigned int MatrixID;
static const double pi = 3.1415926;
#pragma region HelperFunctions
void CheckError();
void UnwrapPointToArray(gmtl::Point3f &point, std::vector<float> &array);
gmtl::Matrix44f Rotation(float s, char r);
gmtl::Matrix44f Translate(float step, char dir);
gmtl::Matrix44f Scale(float factor);
void SetProjection(float width, float depth, gmtl::Matrix44f &p);
#pragma endregion

#pragma region CreateGeometry
void GenerateCircleVerts(std::vector<float> &start_verts, int np);
void RotatePoints(std::vector<float> &vertices, int nm, int np, char rot);
void CreateSphere(int nm, int np, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<float> &color_data, float radius, unsigned int &v, unsigned int &b, unsigned int &i);
void CreateAxis(int nm, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<float> &color_data, unsigned int &v, unsigned int &b, unsigned int &i);
void SetIndices(int nm, int np, std::vector<unsigned int> &indices);
void GenerateColor(std::vector<float> &vertices, std::vector<float> &color_data);
void CreateCone(int nm, vector<float> &vertices, vector<unsigned int> &indices, vector<float> &color_data, unsigned int &v, unsigned int &b, unsigned int &i);
void GenerateBuffers(unsigned int &vao, unsigned int &vbo, unsigned int &ibo, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<float> &color_data);
#pragma endregion

#pragma region DrawGeometry
void Display();
void DrawObject(unsigned int &v, unsigned int &b, unsigned int &i, int indexSize, gmtl::Matrix44f parent);
#pragma endregion

#pragma region Definitions
gmtl::Matrix44f Rotation(float step, char rot)
{
	float rot_factor = gmtl::Math::deg2Rad(step);
	gmtl::Matrix44f rot_Mat;
	rot_Mat.mState = gmtl::Matrix44f::ORTHOGONAL;
	if (rot == 'z')
	{

		rot_Mat.set(// z rotation
			cos(rot_factor), sin(rot_factor), 0.0, 0.0,
			-sin(rot_factor), cos(rot_factor), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	else if (rot == 'x')
	{

		rot_Mat.set(// x rotation
			1.0f, 0, 0, 0,
			0, cos(rot_factor), -sin(rot_factor), 0,
			0, sin(rot_factor), cos(rot_factor), 0,
			0, 0, 0, 1.0);
	}
	else if (rot == 'y')
	{

		rot_Mat.set(// y rotation
			cos(rot_factor), 0.0, sin(rot_factor), 0.0,
			0.0, 1.0, 0.0, 0.0,
			-sin(rot_factor), 0.0, cos(rot_factor), 0.0,
			0.0, 0.0, 0.0, 1.0);
	}



	return rot_Mat;


}

gmtl::Matrix44f Translate(float step, char dir)
{
	gmtl::Matrix44f translate_Matrix;
	translate_Matrix.mState = gmtl::Matrix44f::TRANS;

	if (dir == 'x')
	{
		translate_Matrix.set(
			1.0, 0.0, 0.0, step,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	else if (dir == 'y')
	{
		translate_Matrix.set(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, step,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	else if (dir == 'z')
	{
		translate_Matrix.set(
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, step,
			0.0, 0.0, 0.0, 1.0);
	}

	
	return translate_Matrix;
}

gmtl::Matrix44f Scale(float factor)
{
	gmtl::Matrix44f scale_Matrix;
	scale_Matrix.mState = gmtl::Matrix44f::TRANS;
	scale_Matrix.set(
		1.0 * factor, 0.0, 0.0, 0.0,
		0.0, 1.0 * factor, 0.0, 0.0,
		0.0, 0.0, 1.0 * factor, 0.0,
		0.0, 0.0, 0.0, 1.0);


	return scale_Matrix;
}

void CheckError()
{
	// Check for any errors and print the error string.
	// NOTE: The string returned by gluErrorString must not be altered.
	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		cout << "OpengGL Error: " << errString << endl;
	}
}

void UnwrapPointToArray(gmtl::Point3f &point, vector<float> &array)
{
	for (int i = 0; i < 3; i++)
	{
		array.push_back(point[i]);
	}
}

void GenerateCircleVerts(vector<float> &start_verts, int np, float radius)
{
	for (int i = 0; i <= np - 1; i++)
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

void RotatePoints(vector<float> &vertices, int nm, int np, char rot)
{
	//calculate new x y z values for the point set around the x axis
	if (rot == 'x')
	{
		for (int i = 0; i <= nm + 1; i++) //nm must be at least 3 to get 4 rotations
		{
			float phi = (2 * pi) * (float(i) / float(nm + 1));

			for (int j = 0; j <= (np * 3) - 1; j += 3) //np times
			{
				//need to grab a point and calculate it's new offset 
				float x = vertices.at(j + 0); float y = vertices.at(j + 1); float z = vertices.at(j + 2);
				float newX = x;// *cos(phi) + z * sin(phi);//New_X = z * sin(phi) + x * cos(phi)
				float newY = y * cos(phi) - z * sin(phi);
				float newZ = y * sin(phi) + z * sin(phi);//New_Z = z * cos(phi) – x * sin(phi) 

				vertices.push_back(newX); vertices.push_back(newY); vertices.push_back(newZ);
			}
		}
	}
	//calculate new x y z values for the point set around the y axis
	else if (rot == 'y')
	{

		for (int i = 0; i <= nm + 1; i++) //nm must be at least 3 to get 4 rotations
		{
			float phi = (2 * pi) * (float(i) / float(nm + 1));

			for (int j = 0; j <= (np * 3) - 1; j += 3) //np times
			{
				//need to grab a point and calculate it's new offset 
				float x = vertices.at(j + 0); float y = vertices.at(j + 1); float z = vertices.at(j + 2);

				float newX = x * cos(phi) + z * sin(phi);//New_X = z * sin(phi) + x * cos(phi)
				float newY = y;
				float newZ = (-x * sin(phi)) + z * cos(phi);//New_Z = z * cos(phi) – x * sin(phi) 

				vertices.push_back(newX); vertices.push_back(newY); vertices.push_back(newZ);
			}
		}
	}
	//calculate new x y z values for the point set around the z axis
	else if (rot == 'z')
	{

		for (int i = 0; i <= nm + 1; i++) //nm must be at least 3 to get 4 rotations
		{
			float phi = (2 * pi) * (float(i) / float(nm + 1));

			for (int j = 0; j <= (np * 3) - 1; j += 3) //np times
			{
				//need to grab a point and calculate it's new offset 
				float x = vertices.at(j + 0); float y = vertices.at(j + 1); float z = vertices.at(j + 2);

				float newX = x * cos(phi) - y * sin(phi);//New_X = z * sin(phi) + x * cos(phi)
				float newY = x * sin(phi) + y * cos(phi);//New_Z = z * cos(phi) – x * sin(phi) 
				float newZ = z;

				vertices.push_back(newX); vertices.push_back(newY); vertices.push_back(newZ);
			}
		}
	}



}

void SetIndices(int nm, int np, vector<unsigned int> &indices)
{

	for (int i = 0; i <= nm + 1; i++) //nm = 3
	{
		for (int j = 0; j <= np; j++) //np = 3
		{
			int start = i * np;
			int botL = (j + start);
			indices.push_back(botL);
			int botR = (j + (start + np));
			indices.push_back(botR);
		}
		indices.push_back(0xFFFF);
	} //we copied the origin whenever we rotated around nm + 1 times so we dont need to get the end again

}

void GenerateColor(vector<float> &vertices, vector<float> &color_data)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
		color_data.push_back(vertices.at(i));

}

void CreateSphere(int nm, int np,
	vector<float> &vertices, vector<unsigned int> &indices,
	vector<float> &color_data, float radius,
	unsigned int &v, unsigned int &b, unsigned int &i)
{
	GenerateCircleVerts(vertices, np, radius);
	RotatePoints(vertices, nm, np, 'y');
	GenerateColor(vertices, color_data);
	SetIndices(nm, np, indices);
	std::cout << vertices.size()/3;
	sphereIndexSize = indices.size();
	GenerateBuffers(v, b, i, vertices, indices, color_data);
}


void CreateCone(int nm, vector<float> &vertices,
	vector<unsigned int> &indices, vector<float> &color_data,
	unsigned int &v, unsigned int &b, unsigned int &i)
{
	int np = 3;
	gmtl::Point3f p0(.0f, .0f, .0f);
	gmtl::Point3f p1(.0f, -.15f, .0f);
	gmtl::Point3f p2(-.35f, .0f, .0f);
	
	UnwrapPointToArray(p0, vertices);
	UnwrapPointToArray(p1, vertices);
	UnwrapPointToArray(p2, vertices);


	RotatePoints(vertices, nm, np, 'x');
	GenerateColor(vertices, color_data);
	SetIndices(nm, np, indices);
	coneIndexSize = indices.size();
	GenerateBuffers(v, b, i, vertices, indices, color_data);
}
void CreateAxis(int nm, vector<float> &vertices,
	vector<unsigned int> &indices, vector<float> &color_data,
	unsigned int &v, unsigned int &b, unsigned int &i)
{
		int np = 5;
		gmtl::Point3f p0(.0f, .0f, .0f);
		gmtl::Point3f p1(.025f, .0f, .0f);
		gmtl::Point3f p2(.025f, .9f, .0f);
		gmtl::Point3f p3(.07f, .85f, .0f);
		gmtl::Point3f p4(.0f, .95f, .0f);
		UnwrapPointToArray(p0, vertices);
		UnwrapPointToArray(p1, vertices);
		UnwrapPointToArray(p2, vertices);
		UnwrapPointToArray(p3, vertices);
		UnwrapPointToArray(p4, vertices);

		RotatePoints(vertices, nm, np, 'y');
		GenerateColor(vertices, color_data);
		SetIndices(nm, np, indices);
		axisIndexSize = indices.size();
		GenerateBuffers(v, b, i, vertices, indices, color_data);
}

void GenerateBuffers(unsigned int &vao, unsigned int &vbo, unsigned int &ibo,
	vector<float> &vertices, vector<unsigned int> &indices, vector<float> &color_data)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo); //create 1 buffer name and store the generated buffer object name their
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //bind the buffer name to the target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
		indices.size() * sizeof(unsigned int), //size of buffer object
		&indices[0], //pointer to the data
		GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
		(vertices.size() * sizeof(float)) + (color_data.size() * sizeof(float)), //this is the total size of the buffer
		0, //address of the content to be updated nothing because we are setting it in the subData buffers
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER,
		0, //offset where data replacement will begin
		vertices.size() * sizeof(float), //size
		&vertices[0]);//address of the content to be updated

	glBufferSubData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		color_data.size() * sizeof(float),
		&color_data[0]); //address of the content to be updated

	//Set up the vertex attributes pointer
	glVertexAttribPointer(vertex_position,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		((void*)0)); // vertex buffer object

	// Enable the use of the vertex array
	glEnableVertexAttribArray(vertex_position);

	// Set up the use of the vertex array
	glVertexAttribPointer(vertex_color,
		3,
		GL_FLOAT, GL_FALSE,
		0,
		(const void *)(vertices.size() * sizeof(float)));

	// Enable the use of the color array
	glEnableVertexAttribArray(vertex_color);

}




#pragma endregion

#endif