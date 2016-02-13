#include "Geometry.h"


//layout(location = 0) in vec4 VertexPosition;
//layout(location = 1) in vec4 VertexColor;
//layout(location = 2) in vec3 VertexNormal;
//layout(location = 3) in vec2 VertexUV;
//This is the layout in the shader for the attribute variables
//that will be binded to the different geometry buffers

void UnwrapPointToArray(gmtl::Point3f &point, std::vector<GLfloat> &array)
{
	for (int i = 0; i < 3; i++)
	{
		array.push_back(point[i]);
	}
}


void AddVert(std::vector<GLfloat> &v, GLfloat x, GLfloat y, GLfloat z)
{
	v.push_back(x);
	v.push_back(y);
	v.push_back(z);

}

void GenerateCircleVerts(std::vector<GLfloat> &start, int np, GLfloat radius)
{
	for (int i = 0; i < np; i++)
	{
		//get the current x_angle
		GLfloat theta = pi * (GLfloat)i / (GLfloat)(np - 1);
		GLfloat newX = radius * sin(theta);
		GLfloat newY = radius * cos(theta);

		start.push_back(newX); //x
		start.push_back(newY); //y
		start.push_back(0.0f); //z
	}
}

void RotatePoints(std::vector<GLfloat> &start, std::vector<GLfloat> &vertices, int nm, int np, char rot)
{
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
	switch (rot)
	{
	case 'y':
		for (int i = 0; i <= nm; i++) //nm must be at least 3 to get 4 rotations
		{
			GLdouble phi = 2 * pi * GLdouble(i) / GLdouble(nm);			 
			for (int j = 0; j < np * 3;) //np times
			{

				//need to grab a point and calculate it's new offset 
				x = start.at(j);
				y = start.at(j + 1);
				z = start.at(j + 2);


				GLdouble newX = x * cos(phi) + z * sin(phi);//New_X = z * sin(phi) + x * cos(phi)
				GLdouble newY = y;
				GLdouble newZ = z * cos(phi) - x * sin(phi);//New_Z = z * cos(phi) – x * sin(phi) 

				vertices.push_back(newX);
				vertices.push_back(newY);
				vertices.push_back(newZ);
				j += 3;
			}
		}
		break;

	case 'x':
		for (int i = 0; i <= nm; i++) //nm must be at least 3 to get 4 rotations
		{
			GLfloat phi = 2 * pi * GLfloat(i) / GLfloat(nm);
			std::cout << "phi is " << gmtl::Math::rad2Deg(phi) << std::endl;
			GLfloat c = cos(phi);
			GLfloat s = sin(phi);
			for (int j = 0; j < np * 3;) //np times
			{
				//need to grab a point and calculate it's new offset 
				GLfloat x = start.at(j);
				GLfloat y = start.at(j + 1);
				GLfloat z = start.at(j + 2);

				GLfloat newX = x;
				GLfloat newY = y * c + z * s;
				GLfloat newZ = -y * s + z * c;//New_Z = z * cos(phi) – x * sin(phi) 

				vertices.push_back(newX);
				vertices.push_back(newY);
				vertices.push_back(newZ);
				j += 3;
			}
		}
		break;

	case 'z':
		for (int i = 0; i <= nm; i++) //nm must be at least 3 to get 4 rotations
		{
			GLfloat phi = 2 * pi * GLfloat(i) / GLfloat(nm);
			//std::cout << "phi is " << gmtl::Math::rad2Deg(phi) << std::endl;
			for (int j = 0; j < (np * 3);) //np times
			{
				//need to grab a point and calculate it's new offset 
				GLfloat x = start.at(j + 0);
				GLfloat y = start.at(j + 1);
				GLfloat z = start.at(j + 2);

				GLfloat newX = x * cos(phi) - y * sin(phi);//New_X = z * sin(phi) + x * cos(phi)
				GLfloat newY = x * sin(phi) + y * cos(phi);//New_Z = z * cos(phi) – x * sin(phi) 
				GLfloat newZ = z;

				vertices.push_back(newX);
				vertices.push_back(newY);
				vertices.push_back(newZ);
				j += 3;
			}
		}
		break;
	} 
}

void GenerateIndices(int nm, int np, std::vector<unsigned int> &indices)
{
	std::vector<GLfloat> temp;

	//j=np-1
	//		
	//2		5	8	11	14	17
	//1     4	7	10	13	16
	//0		3	6	9	12	15		
	//	

	for (int i = 0; i < nm; i++) //nm = 4
	{
		int start = i * np;
		for (int j = 0; j < np; j++) //np = 3
		{
			int botR = (start + np + j);
			int botL = (start + j);
			indices.push_back(botL);
			indices.push_back(botR);
		}
		indices.push_back(0xFFFF);

	} //we copied the origin whenever we rotated around nm + 1 times so we dont need to get the end again


}

void GenerateColor(std::vector<GLfloat> &vertices, std::vector<GLfloat> &color_data)
{
	//std::cout << vertices.size() << std::endl;;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		//std::cout << vertices.at(i) << std::endl;
		//color_data.push_back(0.5f);
		if (vertices.at(i) <= 0.0f)
		{
			GLfloat colorDif = 1.0f - vertices.at(i);
			GLfloat newColor = vertices.at(i) + colorDif;
			color_data.push_back(newColor);
		}
		else
		{
			//color_data.push_back(.5f);
			color_data.push_back(vertices.at(i));
		}
	}

}

void GenerateNormals(char shape, int nm, int np, std::vector<GLfloat>&start, std::vector<GLfloat>&verts, std::vector<GLfloat> &normArray)
{
	switch (shape)
	{
	case 's':
		for (GLuint i = 0; i < verts.size();)
		{
			//magnitude given by a^2 + b^2 + c^2
			GLfloat x = verts.at(i);
			GLfloat y = verts.at(i + 1);
			GLfloat z = verts.at(i + 2);
			GLfloat magnitude = sqrt(x*x + y*y + z*z);
			//normalized value given by vert/magnitude
			GLfloat newX = x / magnitude;
			GLfloat newY = y / magnitude;
			GLfloat newZ = z / magnitude;

			normArray.push_back(newX);
			normArray.push_back(newY);
			normArray.push_back(newZ);
			i += 3;
		}
		break;
	case 'a':
		//get the starting verts
		//calculate their normals then rotate them

		for (GLuint i = 0; i < start.size() * 3;)
		{
			GLfloat x = verts.at(i);
			GLfloat y = verts.at(i + 1);
			GLfloat z = verts.at(i + 2);
			GLfloat magnitude = sqrt(x*x + y*y + z*z);

			GLfloat newX = x / magnitude;
			GLfloat newY = y / magnitude;
			GLfloat newZ = z / magnitude;

			normArray.push_back(newX);
			normArray.push_back(newY);
			normArray.push_back(newZ);
			i += 3;
		}
		RotatePoints(normArray, normArray, nm, np, 'y');
		break;
	case 'c':
		for (GLuint i = 0; i < start.size() * 3;)
		{
			GLfloat x = verts.at(i);
			GLfloat y = verts.at(i + 1);
			GLfloat z = verts.at(i + 2);
			GLfloat magnitude = sqrt(x*x + y*y + z*z);

			GLfloat newX = x / magnitude;
			GLfloat newY = y / magnitude;
			GLfloat newZ = z / magnitude;

			normArray.push_back(newX);
			normArray.push_back(newY);
			normArray.push_back(newZ);
			i += 3;
		}
		RotatePoints(normArray, normArray, nm, np, 'y');

		break;

	}
}

void GenerateUV(int nm, int np, std::vector<GLfloat> &uv)
{
	GLfloat u, v;

	//j=np-1
	//0,3	1,3		2,3		3,3		4,3		5,3
	//0,2	1,2		2,2		3,2		4,2		5,2
	//0,1	1,1		2,1		3,1		4,1		5,1
	//0,0	1,0		2,0		3,0		4,0		5,0
	//		                                i=nm

	//u,v u = j/nm
	//v = i/np-1

	//every point is a vertex with 3 values
	//for every point there is 1 u,v containing 2 values

	for (int i = 0; i <= nm; i++) //nm = 4
	{
		for (int j = 0; j < np;) //np = 4
		{
			u = (GLfloat)i / (GLfloat)nm;
			v = (GLfloat)j / (GLfloat)(np);
			//std::cout << "[" << i << "," << j << "]";
			//std::cout << "[u:" << u << "v:" << v << "]" << std::endl;
			uv.push_back(u); uv.push_back(v);
			j++;
		}

	}
}

int GetGeometrySize(char g)
{
	switch (g)
	{
	case 's':
		return sphereIndexSize;
		break;
	case 'a':
		return axisIndexSize;
		break;
	case 'c':
		return coneIndexSize;
		break;
	}
	return 0;
}

std::vector<gmtl::Point3f> rPoints(std::vector<gmtl::Point3f> start, int nm, int np, float radius)
{
	std::vector<gmtl::Point3f> verts;
	
	for (int i = 0; i <= nm; i++) //nm must be at least 3 to get 4 rotations
	{
		GLdouble phi = 2 * pi * GLdouble(i) / GLdouble(nm);
		for (int j = 0; j < np;) //np times
		{
			gmtl::Point3f oldP = start.at(j);
			float x = oldP[0];
			float y = oldP[1];
			float z = oldP[2];
			
			GLfloat newX = x * cos(phi) + z * sin(phi);//New_X = z * sin(phi) + x * cos(phi)
			GLfloat newY = y;
			GLfloat newZ = z * cos(phi) - x * sin(phi);//New_Z = z * cos(phi) – x * sin(phi) 

			gmtl::Point3f newP(newX, newY, newZ);
			verts.push_back(newP);
			j++;
		}
	}
		return verts;
}
	
std::vector<gmtl::Point3f> gPoints(int np, float radius)
{
	std::vector<gmtl::Point3f> start;
	for (int i = 0; i < np; i++)
	{
		GLfloat theta = pi * (GLfloat)i / (GLfloat)(np - 1);
		GLfloat newX = radius * sin(theta);
		GLfloat newY = radius * cos(theta);
		gmtl::Point3f newPoint(newX, newY, 0.0);
		start.push_back(newPoint); //x
	}
	return start;
}

std::vector<gmtl::Point3f> cPoints(std::vector<gmtl::Point3f> verts)
{
	std::vector<gmtl::Point3f> color;
	//std::cout << vertices.size() << std::endl;;
	for (unsigned int i = 0; i < verts.size(); i++)
	{
		gmtl::Point3f white(1,1,1);
		gmtl::Point3f grey(0.5f, 0.5f, 0.5f);
		color.push_back(grey);
	}
	return color;
}

std::vector<gmtl::Point3f> nPoints(std::vector<gmtl::Point3f> verts, char shape)
{
	std::vector<gmtl::Point3f> _normals;
	switch (shape)
	{

		case 's':
			for (GLuint i = 0; i < verts.size();)
			{
				gmtl::Point3f oldP = verts.at(i);
				//magnitude given by a^2 + b^2 + c^2
				GLfloat oldX = oldP[0];
				GLfloat oldY = oldP[1];
				GLfloat oldZ = oldP[2];
				GLfloat magnitude = sqrt(oldX * oldX + oldY * oldY + oldZ * oldZ);
				//normalized value given by vert/magnitude
				GLfloat newX = oldX / magnitude;
				GLfloat newY = oldY / magnitude;
				GLfloat newZ = oldZ / magnitude;

				gmtl::Point3f newP(newX, newY, newZ);
				_normals.push_back(newP);
				i++;
			}
			break;
		case 'c':
			for (GLuint i = 0; i < verts.size();)
			{
				gmtl::Point3f oldP = verts.at(i);
				//magnitude given by a^2 + b^2 + c^2
				GLfloat oldX = oldP[0];
				GLfloat oldY = oldP[1];
				GLfloat oldZ = oldP[2];
				GLfloat magnitude = sqrt(oldX * oldX + oldY * oldY + oldZ * oldZ);
				//normalized value given by vert/magnitude
				GLfloat newX = oldX / magnitude;
				GLfloat newY = oldY / magnitude;
				GLfloat newZ = oldZ / magnitude;

				gmtl::Point3f newP(newX, newY, newZ);
				_normals.push_back(newP);
				i++;
			}
			break;
		case 'a':
			for (GLuint i = 0; i < verts.size();)
			{
				gmtl::Point3f oldP = verts.at(i);
				//magnitude given by a^2 + b^2 + c^2
				GLfloat oldX = oldP[0];
				GLfloat oldY = oldP[1];
				GLfloat oldZ = oldP[2];
				GLfloat magnitude = sqrt(oldX * oldX + oldY * oldY + oldZ * oldZ);
				//normalized value given by vert/magnitude
				GLfloat newX = oldX / magnitude;
				GLfloat newY = oldY / magnitude;
				GLfloat newZ = oldZ / magnitude;

				gmtl::Point3f newP(newX, newY, newZ);
				_normals.push_back(newP);
				i++;
			}
			break;
		case 'b':
			for (GLuint i = 0; i < verts.size();)
			{
				gmtl::Point3f oldP = verts.at(i);
				//magnitude given by a^2 + b^2 + c^2
				GLfloat oldX = oldP[0];
				GLfloat oldY = oldP[1];
				GLfloat oldZ = oldP[2];
				GLfloat magnitude = sqrt(oldX * oldX + oldY * oldY + oldZ * oldZ);
				//normalized value given by vert/magnitude
				GLfloat newX = oldX / magnitude;
				GLfloat newY = oldY / magnitude;
				GLfloat newZ = oldZ / magnitude;

				gmtl::Point3f newP(newX, newY, newZ);
				_normals.push_back(newP);
				i++;
			}
			break;
	}
	
	
	return _normals;
}

std::vector<gmtl::Point2f> uvPoints(int nm, int np)
{
	GLfloat u, v;
	std::vector<gmtl::Point2f> _uvs;
	for (int i = 0; i <= nm; i++) //nm = 4
	{
		for (int j = 0; j < np;) //np = 4
		{
			
			u = (GLfloat)i / (GLfloat)nm;
			v = (GLfloat)j / (GLfloat)(np);
			//std::cout << "[" << i << "," << j << "]";
			//std::cout << "[u:" << u << "v:" << v << "]" << std::endl;
			gmtl::Point2f uv(u,v);
			_uvs.push_back(uv);
			j++;
		}
	}
	return _uvs;
}

void CreateSphere(int nm, int np, GLfloat radius, GLuint &vao, GLuint &vbo, GLuint &ibo)
{
	std::vector<gmtl::Point3f> startVerts;
	std::vector<gmtl::Point3f> verts;
	std::vector<GLuint> indices;
	std::vector<gmtl::Point3f> color;
	std::vector<gmtl::Point2f> uvs;
	std::vector<gmtl::Point3f> normals;

	startVerts = gPoints(np, radius);
	verts = rPoints(startVerts, nm, np, radius);
	color = cPoints(verts);
	normals = nPoints(verts, 's');
	uvs = uvPoints(nm, np);
	GenerateIndices(nm, np, indices);

	sphereIndexSize = indices.size();
	
	int indexOffset = indices.size() * sizeof(GLuint);
	int vertOffset = verts.size() * sizeof(gmtl::Point3f);	 	
	int colorOffset = color.size() * sizeof(gmtl::Point3f);
	int normalOffset = normals.size() * sizeof(gmtl::Point3f);
	int uvOffset = uvs.size() * sizeof(gmtl::Point2f);

	//Generate the vertex arrays for this object
	//the vertex array will beind the vertex array attributes
	//that will be declared later
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo); //create 1 buffer name and store the generated buffer object name their
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //bind the buffer name to the target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
		indexOffset, //size of buffer object
		&indices[0], //pointer to the data
		GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
		vertOffset + colorOffset + normalOffset + uvOffset, //this is the total size of the buffer
		NULL, //address of the content to be updated nothing because we are setting it in the subData buffers
		GL_STATIC_DRAW);//+ uvOffset + normalOffset
	glBufferSubData(GL_ARRAY_BUFFER,
		0, //offset where data replacement will begin
		vertOffset, //size
		&verts[0]);//address of the content to be updated
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset, //offset where data replacement will begin
		colorOffset, //size
		&color[0]);//data
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset,
		normalOffset,
		&normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset + normalOffset,
		uvOffset,
		&uvs[0]);
	//verts|uv|color|normals
	//layout(location = 0) in vec4 VertexPosition;
	//layout(location = 1) in vec4 VertexColor;
	//layout(location = 2) in vec3 VertexNormal;
	//layout(location = 3) in vec2 VertexUV;
	//This is the layout in the shader for the attribute variables
	//that will be binded to the different geometry buffers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset)); // vertex buffer object
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset + colorOffset)); // vertex buffer object
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset + colorOffset + normalOffset) );
		

	glBindVertexArray(0);
	//Bind the index buffer for the object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Bind the index buffer for the object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void CreateCone(int nm, GLuint &vao, GLuint &vbo, GLuint &ibo)
{
	int np = 3;
	std::vector<gmtl::Point3f>start;
	std::vector<gmtl::Point3f> verts;
	std::vector<unsigned int> indices;
	std::vector<gmtl::Point3f> color;
	std::vector<gmtl::Point2f> uvs;
	std::vector<gmtl::Point3f> normals;

	gmtl::Point3f p0(.0f, .0f, .0f);
	gmtl::Point3f p1(.0f, .25f, .0f);
	gmtl::Point3f p2(.125f, .0f, .0f);

	start.push_back(p0);
	start.push_back(p1);
	start.push_back(p2);
	

	verts = rPoints(start, nm, np, 'y');
	color = cPoints(verts);
	normals = nPoints(verts, 'c');
	uvs = uvPoints(nm, np);

	GenerateIndices(nm, np, indices);
	axisIndexSize = indices.size();

	int vertOffset = verts.size() * sizeof(gmtl::Point3f);
	int uvOffset = uvs.size() * sizeof(gmtl::Point2f);
	int colorOffset = color.size() * sizeof(gmtl::Point3f);
	int indexOffset = indices.size() * sizeof(GLuint);
	int normalOffset = normals.size() * sizeof(gmtl::Point3f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo); //create 1 buffer name and store the generated buffer object name their
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //bind the buffer name to the target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
		indexOffset, //size of buffer object
		&indices[0], //pointer to the data
		GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
		vertOffset + colorOffset + normalOffset + uvOffset, //this is the total size of the buffer
		NULL, //address of the content to be updated nothing because we are setting it in the subData buffers
		GL_STATIC_DRAW);//+ uvOffset + normalOffset
	glBufferSubData(GL_ARRAY_BUFFER,
		0, //offset where data replacement will begin
		vertOffset, //size
		&verts[0]);//address of the content to be updated
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset, //offset where data replacement will begin
		colorOffset, //size
		&color[0]);//data
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset,
		normalOffset,
		&normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset + normalOffset,
		uvOffset,
		&uvs[0]);
	//verts|uv|color|normals
	//layout(location = 0) in vec4 VertexPosition;
	//layout(location = 1) in vec4 VertexColor;
	//layout(location = 2) in vec3 VertexNormal;
	//layout(location = 3) in vec2 VertexUV;
	//This is the layout in the shader for the attribute variables
	//that will be binded to the different geometry buffers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset)); // vertex buffer object

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset + colorOffset)); // vertex buffer object

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset + colorOffset + normalOffset));


	glBindVertexArray(0);
	//Bind the index buffer for the object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Bind the index buffer for the object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void CreateAxis(int nm, GLuint &vao, GLuint &vbo, GLuint &ibo)
{
	std::vector<gmtl::Point3f>start;
	std::vector<gmtl::Point3f> verts;
	std::vector<unsigned int> indices;
	std::vector<gmtl::Point3f> color;
	std::vector<gmtl::Point2f> uvs;
	std::vector<gmtl::Point3f> normals;

	int np = 5;
	gmtl::Point3f p0(.0f, .0f, .0f);
	gmtl::Point3f p1(.025f, .0f, .0f);
	gmtl::Point3f p2(.025f, .85f, .0f);
	gmtl::Point3f p3(.07f, .85f, .0f);
	gmtl::Point3f p4(.0f, .95f, .0f);

	start.push_back(p0);
	start.push_back(p1);
	start.push_back(p2);
	start.push_back(p3);
	start.push_back(p4);

	verts = rPoints(start, nm, np, 'y');
	color = cPoints(verts);
	normals = nPoints(verts, 'a');
	uvs = uvPoints(nm, np);
	 
	GenerateIndices(nm, np, indices);
	coneIndexSize = indices.size();	

	int vertOffset = verts.size() * sizeof(gmtl::Point3f);
	int uvOffset = uvs.size() * sizeof(gmtl::Point2f);
	int colorOffset = color.size() * sizeof(gmtl::Point3f);
	int indexOffset = indices.size() * sizeof(GLuint);
	int normalOffset = normals.size() * sizeof(gmtl::Point3f);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo); //create 1 buffer name and store the generated buffer object name their
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //bind the buffer name to the target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
		indexOffset, //size of buffer object
		&indices[0], //pointer to the data
		GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
		vertOffset + colorOffset + normalOffset + uvOffset, //this is the total size of the buffer
		NULL, //address of the content to be updated nothing because we are setting it in the subData buffers
		GL_STATIC_DRAW);//+ uvOffset + normalOffset
	glBufferSubData(GL_ARRAY_BUFFER,
		0, //offset where data replacement will begin
		vertOffset, //size
		&verts[0]);//address of the content to be updated
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset, //offset where data replacement will begin
		colorOffset, //size
		&color[0]);//data
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset,
		normalOffset,
		&normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset + normalOffset,
		uvOffset,
		&uvs[0]);
	//verts|uv|color|normals
	//layout(location = 0) in vec4 VertexPosition;
	//layout(location = 1) in vec4 VertexColor;
	//layout(location = 2) in vec3 VertexNormal;
	//layout(location = 3) in vec2 VertexUV;
	//This is the layout in the shader for the attribute variables
	//that will be binded to the different geometry buffers
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(0));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset)); // vertex buffer object

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset + colorOffset)); // vertex buffer object

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(vertOffset + colorOffset + normalOffset));


	glBindVertexArray(0);
	//Bind the index buffer for the object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Bind the index buffer for the object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}
 
void CreateCube(GLuint &vao, GLuint &vbo)
{
	
 
#pragma region Verts
	GLfloat	vertices[] =
	{
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end

		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

#pragma endregion
	
#pragma region Colors
	GLfloat	colors[] =
	{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};
#pragma endregion

#pragma region Normals
	GLfloat normals[] =
	{
		 -0.577350, -0.577350, -0.577350,
		  0.816497, -0.408248, -0.408248,
		 -0.408248,  0.816497, -0.408248,
		  0.408248,  0.408248, -0.816497,
		 -0.408248, -0.408248,  0.816497,
		  0.408248, -0.816497,  0.408248,
		 -0.816497,  0.408248,  0.408248,
		  0.577350,  0.577350,  0.577350

	};
#pragma endregion
  
	int normalOffset = sizeof(normals);
	int vertOffset = sizeof(vertices);
	int colorOffset = sizeof(colors);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Set the size and data of the Vertex Buffer Object and set it to STATIC_DRAW
	glBufferData(GL_ARRAY_BUFFER, //this is the target we are buffering to
		vertOffset + normalOffset + colorOffset, //this is the total size of the buffer
		NULL, //address of the content to be updated nothing because we are setting it in the subData buffers
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER,
		0, //offset where data replacement will begin
		vertOffset, //size
		&vertices[0]);//data

	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset, //offset where data replacement will begin
		normalOffset, //size
		&normals[0]);//address of the content to be updated

	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + normalOffset,
		colorOffset,
		&colors[0]);
	 

	//layout(location = 0) in vec4 VertexPosition;
	//layout(location = 1) in vec4 VertexColor;
	//layout(location = 2) in vec3 VertexNormal;
	//layout(location = 3) in vec2 VertexUV;
	//This is the layout in the shader for the attribute variables
	//that will be binded to the different geometry buffers

	glEnableVertexAttribArray(0); //moved these around and the address ranges are correct color still not taking
	glVertexAttribPointer(0,
		3,
		GL_FLOAT, GL_FALSE,	0,
		(void*)(0));

	glEnableVertexAttribArray(2);
	//Set up the vertex attributes pointer
	glVertexAttribPointer(2,
		3, 
		GL_FLOAT, GL_FALSE, 0,
		(void*)(normalOffset)); // vertex buffer object

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT, GL_FALSE, 0,
		(void*)(vertOffset + normalOffset)); // vertex buffer object

	//Unbind the vertex array
	glBindVertexArray(0);
	//Unbind the index buffer for the object
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	


}








