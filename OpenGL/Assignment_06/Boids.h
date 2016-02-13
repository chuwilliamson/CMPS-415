#pragma once
#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>

/*Create an array of birds(states), each with a mass, rotation ({R} wrt {W}), scalar forward angular
velocity w, and other variables as needed below. Store the rotation as a matrix (no quaternions).
Initialize states to randomized or interesting values. For A5, all birds will fly
at a constant height (flight radius r).

For each time step, ^, of a simulation, update states as follows
1) Convert bird's w to a world-referenced velocity vector. Do this for all birds before Step 2
2) Compute bird's world-referenced acceleration vector based on the influence of all other birds.
Compute acceleration on all birds before going to Step 3(to use consistent state for all birds).
3) Update birds world-referenced velocity by adding the product of its acceleration and ^.
4) Rotate the bird to point in the updated velocity direction, and convert velocity into updated w.
5) Move the bird forward using local Z-rotation of {R} by an angle that is the product of w and ^.
*/


class Bird
{
	public:
		Bird();
		Bird(gmtl::Matrix44f rot, float m, float v, float h, int id);			
		int _id;
		float _mass;
		float _fwdVel;
		float _vertVel;
		float _height;		
		float _cAffectors;
		float _dAffectors;
		float _aAffectors;
		gmtl::Matrix44f _T_o_r;
		gmtl::Matrix44f _T_r_w;
		gmtl::Matrix44f _translation;
		gmtl::Vec3f _origin;		
		gmtl::Vec3f _worldRefVel;
		gmtl::Vec3f _worldAccel; 
		gmtl::Vec3f _dispersion;	
		gmtl::Vec3f _centering;
		gmtl::Vec3f _alignment;	
		gmtl::Vec3f _flightSpeed;		
		float Bird::dInfluence(Bird nay, float threshold, float distance);		
		float Bird::cInfluence(Bird nay, float threshold, float distance);
		float Bird::aInfluence(Bird nay, float threshold, float distance);
		void Bird::Dispersion(float threshold, Bird nay);
		void Bird::Centering(float threshold, Bird nay);
		void Bird::Alignment(float threshold, Bird nay);
		void Bird::FlightSpeedControl();
		void Bird::Print();
		float Bird::InterBirdDistance(Bird nay, gmtl::Vec3f &dir);
};