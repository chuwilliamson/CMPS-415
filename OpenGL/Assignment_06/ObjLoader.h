#pragma once

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <gmtl\gmtl.h>
#include <GL/glew.h>


using namespace gmtl;

struct v3
{
	float x;
	float y;
	float z;
};

struct v2
{
	float x;
	float y;
};



void LoadObj(char *path,
	std::vector <v3> &vert,
	std::vector <v3> &normal,
	std::vector <v2> &uv,
	std::vector<unsigned int> &vi,
	std::vector<unsigned int> &uvi,
	std::vector<unsigned int> &ni);
