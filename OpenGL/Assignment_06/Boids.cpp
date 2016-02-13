#include "Boids.h"

Bird::Bird(gmtl::Matrix44f trans, float m, float v, float h, int id)
{
	//initialize the birds unique identifier to help with collecting neighbors
	_id = id;
	//initialize the birds mass could be random
	_mass = m;
	//initialize the birds starting velocity
	_fwdVel = v;
	//initialize the birds starting height
	_height = h;
	//number of neighbors contributing to force calculations
	_aAffectors = 1;
	_dAffectors = 1;
	_cAffectors = 1;
	//make a translation for height
	gmtl::setTrans(_translation, gmtl::Vec3f(h, 0, 0));
	//initialize the starting {R} wrt {W} rotation
	_T_r_w =  trans;
	//translate the bird out by the height
	_T_o_r = _T_r_w * _translation;
	//initialize the birds world referenced velocity
	_worldRefVel = gmtl::Vec3f(trans[0][1], trans[1][1], trans[2][1]) * _fwdVel * _height;
	//initialize the birds world referenced acceleration
	_worldAccel = gmtl::Vec3f(0, 0, 0);
	_dispersion = gmtl::Vec3f(0, 0, 0);
	_centering = gmtl::Vec3f(0, 0, 0);
	_alignment = gmtl::Vec3f(0, 0, 0);

}

void Bird::Dispersion(float threshold, Bird nay)
{ 
	//passing this by ref b/c need to get out
	//two things. The direction and the distance
	gmtl::Vec3f direction;

	float distance = InterBirdDistance(nay, direction); 
	
	//what is the neighbors contribution to this birds
	//direction based on distance, mass , etc...
	float influence = dInfluence(nay, distance, threshold);
	
	_dispersion += (influence * direction);

	//This is to give them a jumpy effect when they get too close
	if(influence > .5f )
		_dispersion *= 2;
	if(influence > .75f)
		_dispersion *= 4;
}
 
void Bird::Centering(float threshold, Bird nay)
{
	//passing this by ref b/c need to get out
	//two things. The direction and the distance
	gmtl::Vec3f direction;

	float distance = InterBirdDistance(nay, direction);

	//what is the neighbors contribution to this birds
	//direction based on distance, mass , etc...
	float influence = cInfluence(nay, distance, threshold);

	_centering += (influence * direction * -1.0f);

}

void Bird::Alignment(float threshold, Bird nay)
{

	gmtl::Vec3f Xi = gmtl::Vec3f(_T_r_w[0][0], _T_r_w[1][0], _T_r_w[2][0]);
	gmtl::Vec3f Xj = gmtl::Vec3f(nay._T_r_w[0][0], nay._T_r_w[1][0], nay._T_r_w[2][0]);
	gmtl::Vec3f U = gmtl::makeCross(Xi, Xj);
	gmtl::normalize(U);

	float angle = gmtl::Math::aCos(gmtl::dot(Xi, Xj));

	float distance = angle * _height;
	//lol at parameters reversed
	float influence = aInfluence(nay, distance, threshold);


	gmtl::Matrix44f newMat = gmtl::make<gmtl::Matrix44f>(gmtl::AxisAnglef(-angle, U));
	gmtl::Vec3f direction = gmtl::Vec3f(newMat[0][1], newMat[1][1], newMat[2][1]);
	direction -= _worldRefVel;
	_alignment += (influence * direction);
}

float Bird::InterBirdDistance(Bird nay, gmtl::Vec3f &dir)
{
	gmtl::Vec3f Xi = gmtl::Vec3f(_T_r_w[0][0], _T_r_w[1][0], _T_r_w[2][0]);
	gmtl::Vec3f Xj = gmtl::Vec3f(nay._T_r_w[0][0], nay._T_r_w[1][0], nay._T_r_w[2][0]);
	gmtl::Vec3f U = makeCross(Xi, Xj);
	gmtl::normalize(U);

	float angle = gmtl::Math::aCos(gmtl::dot(Xi, Xj));

	dir = gmtl::makeCross(Xi, U);

	//return angle * ((_height * nay._height) / 2.0f);
	return angle * _height;

}

float Bird::dInfluence(Bird n, float distance, float threshold)
{
	float d_influence = 0.0f;
	 
		if (distance >= threshold)
		{
			d_influence = 0.0f;
		}
		else
		{
			d_influence = 1.0f - distance / threshold;
			_dAffectors++;
		}
 
	return d_influence;
}

float Bird::cInfluence(Bird n, float distance, float threshold)
{
	float c_influence = 0.0f;
	if (distance >= threshold)
	{
		c_influence = 0.0f;
	}
	else
	{
		
		c_influence = 1.0f - distance / threshold;
		_cAffectors++;
	}

	//if this is a really small bird looking at a big bird
	//increase the centering influence level the big bird
	//has on the little one
	//if (_mass < (n._mass * 3) || _mass > (n._mass * 1.5f))
	//{
		//c_influence *= .75f;
	//}

	return c_influence;
}

float Bird::aInfluence(Bird n, float distance, float threshold)
{
	float a_influence = 0.0f;

	if (distance >= threshold)
		a_influence = 0.0f;
	else
	{
		
		a_influence = 1.0f - distance / threshold;
		_aAffectors++;
	}

	return a_influence;
}

void Bird::FlightSpeedControl()
{
	if (_fwdVel > 50.0f)
	{
		//SPEED CONTROL::can be applied along vi's direction instead of along Y, or by handling
		//the vertical aspect separately by adding +-X to force depending on Vv.
		_flightSpeed = -2.0f * gmtl::makeNormal(_worldRefVel);
		if(_fwdVel > 100.0f)
			_flightSpeed *= 2.0f;
	}

	else if (_fwdVel < 25.0f)
	{
		_flightSpeed = gmtl::makeNormal(_worldRefVel) * 4.0f;
		if(_fwdVel < 15.0f)
			_flightSpeed *= 5;
	}
	 
}

void Bird::Print()
{
	std::cout << "velocity: " << _fwdVel << std::endl;
	std::cout << "T_r_w: " << std::endl << _T_r_w << std::endl;
	std::cout << "T_o_r: " << std::endl << _T_o_r << std::endl;
	std::cout << "world velocity: " << std::endl << _worldRefVel << std::endl;
	std::cout << "world acceleration: " << std::endl << _worldAccel << std::endl;
}
 
