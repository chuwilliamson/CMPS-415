#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__


#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>
 
#include "ObjLoader.h"

std::vector<gmtl::Point3f> gPoints(std::vector<gmtl::Point3f> start, int nm, int np, float radius);
//Geoemetry header will handle Shape Generation
//Try to keep anything dealing with shapes or meshes in here

static const float pi = gmtl::Math::PI;
static bool test = 0;
static int axisIndexSize, sphereIndexSize, coneIndexSize, teapotSize, boxSize, mSphere;
static int * ptr;
  
void SetupSphere(int nm, int np, float radius, GLuint &v, GLuint &b, GLuint &i);
void SetupAxis(int nm, GLuint &v, GLuint &b, GLuint &i);
void SetupCone(int nm, GLuint &v, GLuint &b, GLuint &i);
int SetupOBJ(char* obj, GLuint &vao, GLuint &vbo, GLuint &ibo);
void SetupSkybox(int nm, int np, float radius, GLuint &v, GLuint &b, GLuint &i);
void GenerateIndices(int nm, int np, std::vector<GLuint> &indices); 
int GetSize(char); 
void SetSize(char g, int val);
int * getPtr();
std::vector<gmtl::Point3f> rPoints(std::vector<gmtl::Point3f> start, int nm, int np, float radius);
std::vector<gmtl::Point3f> gPoints(int np, float radius);
std::vector<gmtl::Point3f> cPoints(std::vector<gmtl::Point3f> verts);
std::vector<gmtl::Point3f> nPoints(std::vector<gmtl::Point3f> verts, char shape);
#endif