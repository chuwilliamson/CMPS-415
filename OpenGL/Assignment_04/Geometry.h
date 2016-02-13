#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__


#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

class Sphere
{
public:
	Sphere();
	~Sphere();

private:

};


 
std::vector<gmtl::Point3f> gPoints(int np, float radius);
std::vector<gmtl::Point3f> gPoints(std::vector<gmtl::Point3f> start, int nm, int np, float radius);
//Geoemetry header will handle Shape Generation
//Try to keep anything dealing with shapes or meshes in here

static const float pi = 3.1415926f;
static bool test = 0;
static int axisIndexSize, sphereIndexSize, coneIndexSize;

void UnwrapPointToArray(gmtl::Point3f &point, std::vector<float> &array);
void GenerateNormals(char shape, int nm, int np, std::vector<float>&start, std::vector<float>&verts, std::vector<float> &normArray);
void GenerateCircleVerts(std::vector<float> &, std::vector<float>&, int np, float radius);
void RotatePoints(std::vector<float> &start,std::vector<float> &vertices, int nm, int np, char rot);
void CreateSphere(int nm, int np, float radius, unsigned int &v, unsigned int &b, unsigned int &i);
void CreateAxis(int nm, unsigned int &v, unsigned int &b, unsigned int &i);
void CreateCone(int nm, unsigned int &v, unsigned int &b, unsigned int &i);
void GenerateIndices(int nm, int np, std::vector<unsigned int> &indices);
void GenerateColor(std::vector<float> &v, std::vector<float> &c);
void GenerateUV(int nm, int np, std::vector<float> &uv);
int GetGeometrySize(char);
void AddVert(std::vector<float> &v, float x, float y, float z);
void CreateCube(GLuint &vao, GLuint &vbo);

#endif