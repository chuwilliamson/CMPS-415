#include "Particles.h"

//This will create a random flock of birds given state
Particles::Particles(int num, bool isPred)
{
	pred = isPred;

	if (!isPred)
	{
		for (int i = 0; i < num; i++)
		{

			float rmass = 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.25f - 1.0f)));
			float rvelocity = 5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f - 5.0f)));
			float rheight = 1.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.5f - 1.25f)));
			float height = 1.25f;
			float rX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360));
			float rY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360));
			float rZ = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360));
			int birdID = i;

			gmtl::Matrix44f rotMat = gmtl::makeRot<gmtl::Matrix44f>
				(gmtl::EulerAngleXYZf(gmtl::Math::deg2Rad(rX), gmtl::Math::deg2Rad(rY), gmtl::Math::deg2Rad(rZ)));

			Bird b(rotMat, rmass, rvelocity, height, birdID);

			_flock.push_back(b);
		}
		_size = _flock.size();
	}
	else
	{
		for (int i = 0; i < num; i++)
		{

			float rmass = 1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.25f - 1.0f)));
			float rvelocity = 5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10.0f - 5.0f)));
			float rheight = 1.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.5f - 1.25f)));
			float height = 1.25f;
			float rX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360));
			float rY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360));
			float rZ = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 360));
			int birdID = i;

			gmtl::Matrix44f rotMat = gmtl::makeRot<gmtl::Matrix44f>
				(gmtl::EulerAngleXYZf(gmtl::Math::deg2Rad(rX), gmtl::Math::deg2Rad(rY), gmtl::Math::deg2Rad(rZ)));

			Bird b(rotMat, rmass, rvelocity, height, birdID);

			_flock.push_back(b);
		}
		_size = _flock.size();
	}

}

//1) Convert bird's w to a world-referenced velocity vector. Do this for all birds before Step 2
void Particles::Step1()
{
	for (size_t i = 0; i < _flock.size(); i++)
	{
		Bird *b = &_flock.at(i);
		b->_worldRefVel = gmtl::Vec3f(b->_T_r_w[0][1], b->_T_r_w[1][1], b->_T_r_w[2][1]) * b->_fwdVel * b->_height;
		b->_dispersion = gmtl::Vec3f(0, 0, 0);
		b->_centering = gmtl::Vec3f(0, 0, 0);
		b->_alignment = gmtl::Vec3f(0, 0, 0);
	}

}

void Particles::CalculateForces(float dstr, float cstr, float astr)
{
	float dLimit = 1.0f;
	float cLimit = 3.0f;
	float aLimit = 2.0f;

	for (size_t i = 0; i < _flock.size(); i++)
	{
		Bird *b = &_flock.at(i);
		b->_dAffectors = 1;
		b->_cAffectors = 1;
		b->_aAffectors = 1;
		for (size_t j = 0; j < _flock.size(); j++)
		{
			if (i != j)
			{
				Bird nay = _flock.at(j);
				b->Dispersion(dLimit, nay);
				b->Centering(cLimit, nay);
				b->Alignment(aLimit, nay);
			}
		}

		b->_dispersion /= b->_dAffectors;
		b->_centering /= b->_cAffectors;
		b->_alignment /= b->_aAffectors;
		b->FlightSpeedControl();
		b->_worldAccel = ((dstr * b->_dispersion) + (cstr * b->_centering) + (astr * b->_alignment) + b->_flightSpeed) / b->_mass;
	}
}

//3) Update birds world-referenced velocity by adding the product of its acceleration and ^.
//4) Rotate the bird to point in the updated velocity direction, and convert velocity into updated w.
//5) Move the bird forward using local Z-rotation of {R} by an angle that is the product of w and ^.
void Particles::RotateAndMove(float deltaTime)
{
	for (size_t i = 0; i < _flock.size(); i++)
	{
		Bird* b = &_flock.at(i);
		b->_origin = gmtl::makeTrans<gmtl::Vec3f>(b->_T_o_r);
		b->_worldRefVel = b->_worldRefVel + (b->_worldAccel * deltaTime);

		gmtl::Vec3f Xaxis = gmtl::Vec3f(b->_T_r_w[0][0], b->_T_r_w[1][0], b->_T_r_w[2][0]);
		gmtl::Vec3f Yaxis = gmtl::Vec3f(b->_T_r_w[0][1], b->_T_r_w[1][1], b->_T_r_w[2][1]);
		gmtl::Vec3f Zaxis = gmtl::Vec3f(b->_T_r_w[0][2], b->_T_r_w[1][2], b->_T_r_w[2][2]);
		//The Z axis (third column) is the normalized cross product of the X axis and the updated newVelocity from Step 3. 
		gmtl::Vec3f newZaxis = gmtl::makeCross(Xaxis, b->_worldRefVel);
		gmtl::normalize(newZaxis);
		//The Y axis is the cross product of Z and X axes. 
		gmtl::Vec3f newYaxis = gmtl::makeCross(newZaxis, Xaxis);

		gmtl::Matrix44f newRotation;
		newRotation.set(
			//x			//y	           //z			//translation
			Xaxis[0], newYaxis[0], newZaxis[0], b->_T_r_w[0][3],
			Xaxis[1], newYaxis[1], newZaxis[1], b->_T_r_w[1][3],
			Xaxis[2], newYaxis[2], newZaxis[2], b->_T_r_w[2][3],
			b->_T_r_w[3][0], b->_T_r_w[3][1], b->_T_r_w[3][2], b->_T_r_w[3][3]
			);
		//calculate new rotation
		b->_T_r_w = newRotation;
		//rotate the bird local
		b->_T_o_r = b->_T_r_w * b->_translation;
		//set the bird velocity
		//the y axis dotted with the force calculation will give the speed
		b->_fwdVel = (gmtl::dot(newYaxis, b->_worldRefVel)) / b->_height;

		float test = gmtl::length(b->_worldRefVel);

		float angle = gmtl::Math::deg2Rad(b->_fwdVel * deltaTime);

		gmtl::Matrix44f forwardRotation;
		//move in the new z direction by the scalar velocity * deltaTime
		forwardRotation.set(// z rotation
			cos(angle), -sin(angle), 0.0, 0.0,
			sin(angle), cos(angle), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
		//update the world rotation to be the newrotation from direction composed with the forward z rotation matrix	
		b->_T_r_w *= forwardRotation;


	}
}

void Particles::Update(float dstr, float cstr, float astr, float deltaTime)
{

	Step1();
	//-------------------------------------
	CalculateForces(dstr, cstr, astr);
	RotateAndMove(deltaTime);

	//-------------------------------------

}

Bird Particles::BirdAt(int id)
{
	return _flock.at(id);
}


