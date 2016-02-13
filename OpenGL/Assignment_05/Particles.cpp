#include "Particles.h"



Bird::Bird(gmtl::Matrix44f trans, float m, float v, float h)
{

	gmtl::setTrans(translation, gmtl::Vec3f(h, 0, 0));
	//initialize the starting {R} wrt {W} rotation
	T_r_w = trans;
	//translate the bird out by the height
	T_o_r = T_r_w * translation;
	mass = m;
	velocity = v;
	height = h;
	worldVelocity = gmtl::Vec3f(trans[0][1], trans[1][1], trans[2][1]) * velocity * height;
	worldAcceleration = gmtl::Vec3f(1, 1, 1) / mass;
}

float Bird::GetMass()
{
	return this->mass;
}

void Bird::Print()
{
	std::cout << "velocity: " << velocity << std::endl;
	std::cout << "T_r_w: " << std::endl << T_r_w << std::endl;
	std::cout << "T_o_r: " << std::endl << T_o_r << std::endl;
	std::cout << "world velocity: " << std::endl << worldVelocity << std::endl;
	std::cout << "world acceleration: " << std::endl << worldAcceleration << std::endl;
}

float Bird::GetInfluence(float distance , float threshold)
{	
	float influence;
	if(distance > threshold)
		influence = 0;
	else
		influence =  1 - distance / threshold;
	//std::cout<<influence<<std::endl;
	
	return influence;
}

//Step 4 notes: To point a bird along the new velocity vector, a new rotation matrix is computed
//to replace the one in the state. 
//The X axis (first column) is unchanged. 
//The Z axis (third column) is the normalized cross product of the X axis and the updated newVelocity from Step 3. 
//The Y axis is then cross product of Z and X axes. 
//Then, w is the dot product of Y and newVelocity, dinewVelocityded by flight radius.
void Bird::RotateBird(int id, std::vector <Bird> flock, float deltaTime)
{
	std::vector<Bird> neighbors = GetNeighbors(id, flock);

	//the current birds transformation matrix
	//set the world velocity to the 2nd column
	worldVelocity = gmtl::Vec3f(T_r_w[0][1], T_r_w[1][1], T_r_w[2][1]) * velocity * height;

	float worldVelocityMagnitude = gmtl::length(worldVelocity);

	
	gmtl::Vec3f forceDirection;
	if (worldVelocityMagnitude > 10)
	{ 
		forceDirection = gmtl::Vec3f(0, -1.0f, 0);		 
	}
	else if (worldVelocityMagnitude < -10)
	{
		forceDirection = gmtl::Vec3f(0, 1.0f, 0);		
	}
	
	gmtl::Vec3f dispersion = GetDispersion(T_r_w, neighbors);	
	gmtl::Vec3f centering = GetCentering(T_r_w, neighbors);
	 
	//gmtl::Vec3f alignment = GetAlignment(T_r_w, neighbors);

	//worldAcceleration = (dispersion + centering + forceDirection)/ mass;
	//worldAcceleration = dispersion / mass;
	this->worldAcceleration = (dispersion) / mass;
	this->worldVelocity += (worldAcceleration * deltaTime);

	gmtl::Vec3f Xaxis = gmtl::Vec3f(T_r_w[0][0], T_r_w[1][0], T_r_w[2][0]);
	gmtl::Vec3f Yaxis = gmtl::Vec3f(T_r_w[0][1], T_r_w[1][1], T_r_w[2][1]);
	gmtl::Vec3f Zaxis = gmtl::Vec3f(T_r_w[0][2], T_r_w[1][2], T_r_w[2][2]);
 	//The Z axis (third column) is the normalized cross product of the X axis and the updated newVelocity from Step 3. 
	gmtl::Vec3f newZaxis = gmtl::makeCross(Xaxis, this->worldVelocity);
	gmtl::normalize(newZaxis);
	//The Y axis is the cross product of Z and X axes. 
	gmtl::Vec3f newYaxis = gmtl::makeCross(newZaxis, Xaxis);	

	gmtl::Matrix44f newRotation;
	newRotation.set(
		//x			//y	           //z			//translation
		Xaxis[0], newYaxis[0], newZaxis[0], T_r_w[0][3],
		Xaxis[1], newYaxis[1], newZaxis[1], T_r_w[1][3],
		Xaxis[2], newYaxis[2], newZaxis[2], T_r_w[2][3],
		T_r_w[3][0], T_r_w[3][1], T_r_w[3][2], T_r_w[3][3]
		);
	//calculate new rotation
	T_r_w = newRotation;
	//rotate bird to face in the new direction
	T_o_r = T_r_w * translation;

	velocity = gmtl::dot(newYaxis, worldVelocity) / height;

	float angle = gmtl::Math::deg2Rad(velocity * deltaTime);

	gmtl::Matrix44f forwardRotation;
	//move in the new z direction by the scalar velocity * deltaTime
	forwardRotation.set(// z rotation
		cos(angle), -sin(angle), 0.0, 0.0,
		sin(angle), cos(angle), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	//update the world rotation to be the newrotation from direction composed
	//with the forward z rotation matrix
	T_r_w = T_r_w * forwardRotation;
	//update the bird local transformation to be rotated by the new T_r_w
	T_o_r = T_r_w * translation;
}

gmtl::Vec3f Bird::GetDispersion(gmtl::Matrix44f T_r_w, std::vector<Bird> nay)
{
	gmtl::Vec3f dispersion;
	float threshold = 1.0f;
	
	for (int i = 0; i < nay.size(); i++)
	{
		gmtl::Matrix44f nay_T_r_w = nay.at(i).Rotation();
		gmtl::Vec3f Xi = gmtl::Vec3f(this->T_r_w[0][0], this->T_r_w[1][0], this->T_r_w[2][0]);
		gmtl::Vec3f Xj = gmtl::Vec3f(nay_T_r_w[0][0], nay_T_r_w[1][0], nay_T_r_w[2][0]);
		gmtl::Vec3f U = gmtl::makeCross(Xi, Xj);
		gmtl::normalize(U);

		float angle = gmtl::Math::aCos(gmtl::dot(Xi, Xj));
		distance = this->height * angle;	
		float influence = GetInfluence(distance, threshold);		
		
		dispersion += influence * gmtl::makeCross(Xi, U);
	
	}

	return  dispersion;
}

gmtl::Vec3f Bird::GetCentering(gmtl::Matrix44f T_r_w, std::vector<Bird> nay)
{
	gmtl::Vec3f centering, Xi, Xj, U, nay_T_r_w;
	float threshold = 15.0f;
	float influenceLvl = .50f;
	for (int i = 0; i < nay.size(); i++)
	{
		gmtl::Matrix44f nay_T_r_w = nay.at(i).Rotation();
		Xi = gmtl::Vec3f(this->T_r_w[0][0], this->T_r_w[1][0], this->T_r_w[2][0]);
		Xj = gmtl::Vec3f(nay_T_r_w[0][0], nay_T_r_w[1][0], nay_T_r_w[2][0]);
		U = gmtl::makeCross(Xi, Xj);
		gmtl::normalize(U);

		float angle = gmtl::Math::aCos(gmtl::dot(Xi, Xj));
		float distance = this->height * angle;
		float influence = GetInfluence(distance, threshold);

		
			centering += influence * (gmtl::makeCross(Xi, U));
	}

	return -centering;
}

gmtl::Vec3f Bird::GetAlignment(gmtl::Matrix44f T_r_w, std::vector<Bird> nay)
{
	gmtl::Vec3f alignment;
	
	float influenceLvl = .05f;
	float threshold = 1.0f;
	for (int i = 0; i < nay.size(); i++)
	{

		gmtl::Matrix44f nay_T_r_w = nay.at(i).Rotation();
		gmtl::Vec3f Xi = gmtl::Vec3f(this->T_r_w[0][0], this->T_r_w[1][0], this->T_r_w[2][0]);
		gmtl::Vec3f Xj = gmtl::Vec3f(nay_T_r_w[0][0], nay_T_r_w[1][0], nay_T_r_w[2][0]);
		gmtl::Vec3f U = gmtl::makeCross(Xi, Xj);
		gmtl::normalize(U);
		float angle = gmtl::Math::aCos(gmtl::dot(Xi, Xj));
		float distance = this->height * angle;

		//the influence factor acts like a radius around the bird
		//if the distance is within the influence factor
 
			//if the acting bird is closer to this bird increase his amount of influence
		float influence = GetInfluence(distance, threshold);

		if(influence > 0.02f)
		{			
			gmtl::Matrix44f newMat = gmtl::make<gmtl::Matrix44f>(gmtl::AxisAnglef(-angle, U));
			alignment += gmtl::Vec3f(newMat[0][2], newMat[1][2], 
				newMat[2][2]) - gmtl::Vec3f(T_r_w[0][2], T_r_w[1][2], T_r_w[2][2]);
		}

		 
	}

	return influenceLvl * alignment;
}

void Particles::Create(int num)
{
	float X = 0.0f;
	float Y = 0.0f;
	float Z = 0.0f;
	gmtl::Matrix44f rotMat;
	for (int i = 0; i < num; i++)
	{
		float rmass = rand() % 3 + 1;
		float mass = 3.0f;
		float rvelocity = rand() % 15 + 1;
		float velocity = .50f;
		float height = 1.0f;
		float rX = rand() % 360;
		float rY = rand() % 360;
		float rZ = rand() % 360;

		rotMat = gmtl::makeRot<gmtl::Matrix44f>
		(
			gmtl::EulerAngleXYZf
			(
			gmtl::Math::deg2Rad(rX),
			gmtl::Math::deg2Rad(rY),
			gmtl::Math::deg2Rad(rZ)
			)
		);
		Bird b(rotMat, mass, rvelocity, height);
		birds.push_back(b);
	}

	size = birds.size();
}

gmtl::Matrix44f Bird::Transform()
{
	//return the transform to draw
	return T_o_r;
}

gmtl::Matrix44f Bird::Rotation()
{
	return T_r_w;
}

#pragma region Private

std::vector <Bird> Bird::GetNeighbors(int self, std::vector <Bird> flock)
{
	std::vector<Bird> neighbors; 
	for (int i = 0; i < flock.size(); i++)	 
	{
		if (i != self)		 
			neighbors.push_back(flock.at(i)); 
	}
	 
 
	return neighbors;
	
}




int Particles::Size()
{
	return size;
}
#pragma endregion
