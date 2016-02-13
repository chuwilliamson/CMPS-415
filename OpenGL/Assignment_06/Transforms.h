#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

 
enum CameraState
{
	Camera1,
	Camera2,
	Camera3,
	Camera4,
};

CameraState cameraState = Camera1;

gmtl::Matrix44f
rLong, rLat,
rAzi, rEle;

gmtl::Matrix44f
World, Projection,

//some sphere tranforms matrices
T_s1_w,
T_s2_w,
T_s3_w,
//camera transforms
T_c1_w,
T_c2_w,
T_c3_w,
T_c2_o,
T_b_o, 
T_j1_b, T_j2_b, T_j3_b;

float zInc = 2.5f;
float sphereRadius = 1.0f; 
float width = 3.0f;
float depth = 3.0f;


 
//=======================================================================================================
gmtl::Matrix44f Rotation(float s, char r);
gmtl::Matrix44f Translate(float step, char dir);
gmtl::Matrix44f Scale(float factor);
gmtl::Matrix44f GetCamera();
void SetPerspective(float l, float r, float b, float t, float n, float f, gmtl::Matrix44f &p);
void SetOrtho(float width, float depth, gmtl::Matrix44f &p);
void SetMatrices();
template <typename Type> void printT(Type a);
//=======================================================================================================

template <typename Type> void printT(Type a)
{
	std::cout<<a<<std::endl;

}

void SetMatrices()
{ 
	T_s2_w = Translate(2.50f, 'x');
	T_s3_w = Translate(-2.50f, 'x');
	
	//SetOrtho(3.0f, 3.0f, Projection);

}

gmtl::Matrix44f Rotation(float step, char rot)
{
	float rot_factor = gmtl::Math::deg2Rad(step);

	gmtl::Matrix44f rot_Mat;

	if (rot == 'z')
	{
		rot_Mat.set(// z rotation
			cos(rot_factor), -sin(rot_factor), 0.0, 0.0,
			sin(rot_factor), cos(rot_factor), 0.0, 0.0,
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
	rot_Mat.mState = gmtl::Matrix44f::ORTHOGONAL;


	return rot_Mat;


}

gmtl::Matrix44f Translate(float step, char dir)
{
	gmtl::Matrix44f translate_Matrix;

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

	translate_Matrix.mState = gmtl::Matrix44f::TRANS;
	return translate_Matrix;
}

gmtl::Matrix44f Scale(float factor)
{

	gmtl::Matrix44f scale_Matrix;

	scale_Matrix.set(
		1.0f * factor, 0.0, 0.0, 0.0,
		0.0, 1.0f * factor, 0.0, 0.0,
		0.0, 0.0, 1.0f * factor, 0.0,
		0.0, 0.0, 0.0, 1.0f);

	scale_Matrix.mState = gmtl::Matrix44f::ORTHOGONAL;
	return scale_Matrix;
}

gmtl::Matrix44f GetCamera()
{
	switch (cameraState)
	{
	case Camera1:
		return T_c1_w;
		break;
	case Camera2:
		return T_c2_o;
		break;
	case Camera3:
		return T_c2_w;
		break;
	case Camera4:
		return T_c3_w;
		break;
	default:
		return T_c1_w;
		break;
	}
}

void SetCamera(CameraState c)
{
	cameraState = c;
}


void SetOrtho(float width, float depth, gmtl::Matrix44f &p)
{
	p.set(2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / width, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f / depth, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void SetPerspective(float l, float r, float b, float t, float n, float f, gmtl::Matrix44f &p)
{	
	//n = near, f = far //t = top, b = bottom //r = right, l = left							
	p.set((2.0f * n) / (r - l), 0.0f, (r + l) / (r - l), 0.0f,
		0.0f, (2.0f * n) / (t - b), (t + b) / (t - b), 0.0f,
		0.0f, 0.0f, -(f + n) / (f - n), (-2.0f * f * n) / (f - n),
		0.0f, 0.0f, -1.0f, 0.0f);
}

gmtl::Matrix44f GetProjection()
{
	return Projection;
}

#endif