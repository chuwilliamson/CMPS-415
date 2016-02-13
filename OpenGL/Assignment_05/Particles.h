#pragma once

#include <gmtl\gmtl.h>
#include <gmtl\Matrix.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>


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

	Bird(gmtl::Matrix44f rot, float m, float v, float h);

	float distance;

 
	gmtl::Vec3f Bird::GetWorldReferencedAcceleration();

	gmtl::Vec3f Bird::GetWorldReferencedVelocity();

	gmtl::Matrix44f Bird::Transform();

	gmtl::Matrix44f Bird::Rotation();

	float Bird::GetMass();

	float Bird::GetInfluence(float threshold, float distance);

	std::vector <Bird> Bird::GetNeighbors(int self, std::vector <Bird> birdArray);

	void Bird::RotateBird(int id, std::vector <Bird> flock , float deltaTime);

	void Bird::Update();
 
	void Bird::Print();

private:

	gmtl::Matrix44f T_o_r;

	gmtl::Matrix44f T_r_w;

	gmtl::Matrix44f translation;

	gmtl::Vec3f worldVelocity;

	gmtl::Vec3f worldAcceleration;

	float mass;

	float velocity;

	float height;

	std::vector<Bird> neighbors;

	gmtl::Vec3f Bird::GetDispersion(gmtl::Matrix44f T_r_w, std::vector<Bird> birdArray);

	gmtl::Vec3f Bird::GetCentering(gmtl::Matrix44f T_r_w, std::vector<Bird> birdArray);

	gmtl::Vec3f Bird::GetAlignment(gmtl::Matrix44f T_r_w, std::vector<Bird> birdArray);

};


class Particles
{
public:
	void Particles::Create(int num);
	int Size();
	std::vector <Bird> birds;

private:
	int size;

};