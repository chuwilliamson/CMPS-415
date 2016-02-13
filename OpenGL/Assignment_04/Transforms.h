#ifndef __TRANSFORMS_H__
#define __TRANSFORMS_H__

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

using namespace gmtl;
gmtl::Matrix44f
rLong, rLat,
rAzi, rEle, rZ,
World;
gmtl::Matrix44f
T_s_w, T_c1_w,
T_r_w, T_o_r,
T_b_o, T_c2_o;
gmtl::Matrix44f
model, view,
projection, perspective;
gmtl::Quatf qX, qY, qZ;

float zInc =  2;
float sphereRadius =  1.0f;
Quatf q, qYaw;
Matrix44f qm, yaw;



gmtl::Matrix44f Rotation(float s, char r);
gmtl::Matrix44f Translate(float step, char dir);
gmtl::Matrix44f Scale(float factor);
void SetPerspective(float l, float r, float b, float t, float n, float f, gmtl::Matrix44f &p);
void SetProjection(float width, float depth, gmtl::Matrix44f &p);
void SetMatrices();

gmtl::Matrix44f Rotation(float step, char rot)
{
	float rot_factor = gmtl::Math::deg2Rad(step);
	gmtl::Matrix44f rot_Mat;

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

void SetMatrices()
{
	gmtl::identity(World);
	T_s_w = World;
	T_c1_w = Translate(zInc, 'z');
	T_r_w = Rotation(45, 'z');
	T_o_r = Translate(sphereRadius + .75f, 'y');
	T_b_o = gmtl::identity(T_b_o);

	qX = gmtl::make<gmtl::Quatf>(Rotation(-5, 'x'));

	//SetProjection(width, depth, projection);
	SetPerspective(-.12f, .12f, -.08f, .08f, .1f, 100.0f, perspective);


}

void SetProjection(float width, float depth, gmtl::Matrix44f &p)
{
	p.set(2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / width, 0.0f, 0.0f,
		0.0f, 0.0f, 2.0f / depth, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void SetPerspective(float l, float r, float b, float t, float n, float f, gmtl::Matrix44f &p)
{//n = near, f = far
	//t = top, b = bottom
	//r = right, l = left
	p.set((2.0f * n) / (r - l), 0.0f, (r + l) / (r - l), 0.0f,
		0.0f, (2.0f * n) / (t - b), (t + b) / (t - b), 0.0f,
		0.0f, 0.0f, -(f + n) / (f - n), (-2.0f * f * n) / (f - n),
		0.0f, 0.0f, -1.0f, 0.0f);
}


#endif