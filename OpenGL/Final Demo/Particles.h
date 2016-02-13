#pragma once


#include <time.h>
#include "Boids.h"
 
class Particles
{
public:
	Particles();
	Particles(int num, bool isPred);
	int _size;
	Bird Particles::BirdAt(int index);	
	void Particles::Update(float dstr, float cstr, float astr, float deltaTime);
private:
	std::vector <Bird> _flock;
	bool pred;
	//1) Convert bird's w to a world-referenced velocity vector. Do this for all birds before Step 2
	void Particles::Step1();
	//2) Compute bird's world-referenced acceleration vector based on the influence of all other birds.
	//Compute acceleration on all birds before going to Step 3(to use consistent state for all birds).
	
	void Particles::CalculateForces(float dstr, float cstr, float astr);
	//3) Update birds world-referenced velocity by adding the product of its acceleration and ^.
	//4) Rotate the bird to point in the updated velocity direction, and convert velocity into updated w.
	//5) Move the bird forward using local Z-rotation of {R} by an angle that is the product of w and ^.
	void Particles::RotateAndMove(float deltaTime);
};