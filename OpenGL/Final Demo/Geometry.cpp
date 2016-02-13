#include "Geometry.h"


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
 
int GetSize(char g)
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
	case 't':
		return teapotSize;
		break;
	case 'b':
		return boxSize;
		break;
	case 'm':
		return mSphere;
		break;
	}
	return 0;
}

void SetSize(char g, int val)
{
	switch (g)
	{
	case 's':
		sphereIndexSize = val;
		break;
	case 'a':
		axisIndexSize = val;
		break;
	case 'c':
		coneIndexSize = val;
		break;
	case 't':
		teapotSize = val;
		break;
	case 'b':
		boxSize = val;
		break;
	case 'm':
		mSphere = val;
		break;
	}
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

std::vector<gmtl::Point3f> rPoints(std::vector<gmtl::Point3f> start, int nm, int np, char rot)
{
	std::vector<gmtl::Point3f> verts;

	for (int i = 0; i <= nm; i++) //nm must be at least 3 to get 4 rotations
	{
		GLfloat phi = 2 * pi * GLfloat(i) / GLfloat(nm);

		for (int j = 0; j < np; j++) //np times
		{
			gmtl::Point3f oldP = start.at(j);
			GLfloat x = oldP[0];
			GLfloat y = oldP[1];
			GLfloat z = oldP[2];
			GLfloat newX;
			GLfloat newY;
			GLfloat newZ;
			GLfloat c = cos(phi);
			GLfloat s = sin(phi);
			switch (rot)
			{
			case 'y':
				newX = x * c + z * s;
				newY = y;
				newZ = z * c - x * s;
				break;
			case 'x':
				newX = x * c - y * s;
				newY = x * s + y * c;
				newZ = z;
				break;
			case 'z':
				newX = x;
				newY = y * c + z * s;
				newZ = -y * s + z * c;
				break;
			}

			gmtl::Point3f newP(newX, newY, newZ);

			verts.push_back(newP);

		}
	}

	return verts;
}

std::vector<gmtl::Point3f> cPoints(std::vector<gmtl::Point3f> verts, char col)
{
	std::vector<gmtl::Point3f> color;
	//std::cout << vertices.size() << std::endl;;
	for (unsigned int i = 0; i < verts.size(); i++)
	{
		gmtl::Point3f red(1, 0, 0);
		gmtl::Point3f blue(0, 0, 1);
		gmtl::Point3f green(0, 1, 0);
		gmtl::Point3f white(1, 1, 1);
		gmtl::Point3f grey(0.5f, 0.5f, 0.5f);
		gmtl::Point3f old(verts.at(i));
		switch (col)
		{
		case 'w':
			color.push_back(white);
			break;
		case 'o':
			color.push_back(old);
			break;
		case 'g':
			color.push_back(grey);
			break;
		case 'h':
			if (i < verts.size() / 2)
				color.push_back(blue);
			else
				color.push_back(green);
			break;
		}


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
			gmtl::Point2f uv(u, v);
			_uvs.push_back(uv);
			j++;
		}
	}
	return _uvs;
}

std::vector<gmtl::Vec3f> tangents(std::vector<gmtl::Point3f> start, std::vector<gmtl::Point3f> startNormals)
{
	//t b n
	//t = -z direction
	//b = n X t
	//rotate it
	std::vector <Vec3f> tangents;
	std::vector <Vec3f> bitangents;
	
	for (int i = 0; i < start.size(); i++)
	{
		Point3f p1 = start.at(i);
		float t = p1[2];
		//float b = 
	}
	return tangents;
}
 
void SetupCone(int nm, GLuint &vao, GLuint &vbo, GLuint &ibo)
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
	gmtl::Point3f p2(.15f, .0f, .0f);

	start.push_back(p0);
	start.push_back(p1);
	start.push_back(p2);


	verts = rPoints(start, nm, np, 'y');
	color = cPoints(verts, 'g');
	normals = nPoints(verts, 'c');
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

	glGenBuffers(1, &ibo); //Setup 1 buffer name and store the generated buffer object name their
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));	 //verts
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset));//color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset)); //normals
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset + normalOffset));//uvs

	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i);
	}

}

void SetupAxis(int nm, GLuint &vao, GLuint &vbo, GLuint &ibo)
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
	gmtl::Point3f p3(.055f, .85f, .0f);
	gmtl::Point3f p4(.0f, 1.0f, .0f);

	start.push_back(p0);
	start.push_back(p1);
	start.push_back(p2);
	start.push_back(p3);
	start.push_back(p4);

	verts = rPoints(start, nm, np, 'y');
	color = cPoints(verts, 'o');
	normals = nPoints(verts, 'a');
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

	glGenBuffers(1, &ibo); //Setup 1 buffer name and store the generated buffer object name their
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));	 //verts
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset));//color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset)); //normals
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset + normalOffset));//uvs
 
	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i);
	}

}

void SetupSphere(int nm, int np, GLfloat radius, GLuint &vao, GLuint &vbo, GLuint &ibo)
{
	std::vector<gmtl::Point3f> startVerts = gPoints(np, radius);
	std::vector<gmtl::Point3f> verts = rPoints(startVerts, nm, np, 'y');
	std::vector<gmtl::Point3f> color = cPoints(verts, 'w');
	std::vector<gmtl::Point3f> normals = nPoints(verts,'s');
	std::vector<gmtl::Point2f> uvs = uvPoints(nm, np);
	std::vector<GLuint> indices;
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

	glGenBuffers(1, &ibo); //Setup 1 buffer name and store the generated buffer object name their
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
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));	 //verts
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset));//color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset)); //normals
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset + normalOffset));//uvs

	for (int i = 0; i < 4; i++)	
		glEnableVertexAttribArray(i);
	
 
 
}

int SetupOBJ(char* obj, GLuint &vao, GLuint &vbo, GLuint &ibo)
{
	std::vector<unsigned int> tmp_vertex_indices, tmp_normal_indices, temp_uv_indices;
	std::vector<v3> tmp_verts, tmp_normals;
	std::vector<v2> tmp_uvs;

	std::vector<Point3f> verts, normals;
	std::vector<Point2f> uvs;

	std::vector<unsigned int> vertex_indices, ni, tmp_uv_indices;

	LoadObj(obj,
		tmp_verts, tmp_normals, tmp_uvs,
		tmp_vertex_indices, tmp_normal_indices, tmp_uv_indices);

	std::cout << obj << " @ " << tmp_verts.size() << "verts";
	//go over the vertex values and make them points
	for (unsigned int i = 0; i < tmp_verts.size(); i++)
	{
		Point3f p(Point3f(tmp_verts.at(i).x, tmp_verts.at(i).y, tmp_verts.at(i).z));
		verts.push_back(p);
	}

	//same for the normals
	for (unsigned int i = 0; i < tmp_normals.size(); i++)
	{
		Point3f p(Point3f(tmp_normals.at(i).x, tmp_normals.at(i).y, tmp_normals.at(i).z));
		normals.push_back(p);
	}

	//same for the uvs except this is a 2d point
	for (unsigned int i = 0; i < tmp_uvs.size(); i++)
	{
		Point2f p(tmp_uvs.at(i).x, tmp_uvs.at(i).y);
		uvs.push_back(p);
	}

	//we got our verts so make an index list based on the
	//f values from the obj v/vt/vn v/vt/vn v/vt/vn
	//obj indexing starts at 1 so subtract 1 from all the elements
	for (unsigned int i = 0; i < tmp_vertex_indices.size(); i++)
	{
		unsigned int index = tmp_vertex_indices.at(i) - 1;

		if (i % 3 == 0 && i > 0)
			vertex_indices.push_back(9000);
		vertex_indices.push_back(index);
	}

	std::vector<gmtl::Point3f> color = cPoints(verts, 'w');

	int indexOffset = sizeof(unsigned int) * vertex_indices.size();
	int vertOffset = sizeof(Point3f) * verts.size();
	int normalOffset = sizeof(Point3f) * normals.size();
	int colorOffset = sizeof(Point3f) * color.size();
	int uvOffset = sizeof(Point2f) * uvs.size();

	unsigned int indexSize = vertex_indices.size();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo); //Setup 1 buffer name and store the generated buffer object name their
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //bind the buffer name to the target
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, //target buffer object
		indexOffset, //size of buffer object
		&vertex_indices[0], //pointer to the data
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
		vertOffset,
		colorOffset,
		&color[0]);

	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset,
		normalOffset,
		&normals[0]);

	glBufferSubData(GL_ARRAY_BUFFER,
		vertOffset + colorOffset + normalOffset,
		uvOffset,
		&uvs[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));	 //verts
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset));//color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset)); //normals
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)(vertOffset + colorOffset + normalOffset));//uvs

	for (int i = 0; i < 4; i++)
		glEnableVertexAttribArray(i);
	 
	return indexSize;

}