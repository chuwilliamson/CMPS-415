#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__


#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>
 

std::vector<gmtl::Point3f> gPoints(std::vector<gmtl::Point3f> start, int nm, int np, float radius);
//Geoemetry header will handle Shape Generation
//Try to keep anything dealing with shapes or meshes in here

static const float pi = 3.1415926f;
static bool test = 0;
static int axisIndexSize, sphereIndexSize, coneIndexSize;

  
void SetupSphere(int nm, int np, float radius, unsigned int &v, unsigned int &b, unsigned int &i);
void SetupAxis(int nm, unsigned int &v, unsigned int &b, unsigned int &i);
void SetupCone(int nm, unsigned int &v, unsigned int &b, unsigned int &i);
void SetupCube(GLuint &vao, GLuint &vbo);
void GenerateIndices(int nm, int np, std::vector<unsigned int> &indices);
 
int GetGeometrySize(char); 

std::vector<gmtl::Point3f> rPoints(std::vector<gmtl::Point3f> start, int nm, int np, float radius);
std::vector<gmtl::Point3f> gPoints(int np, float radius);
std::vector<gmtl::Point3f> cPoints(std::vector<gmtl::Point3f> verts);
std::vector<gmtl::Point3f> nPoints(std::vector<gmtl::Point3f> verts, char shape);
#endif