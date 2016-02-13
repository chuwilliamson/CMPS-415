#ifndef __ASSIGNMENT_02_H__
#define __ASSIGNMENT_02_H__

static enum RotationState
{
	Rpos_x,
	Rneg_x,
	Rpos_y,
	Rneg_y,
	Rpos_z,
	Rneg_z,
};

static enum TranslationState
{
	left_t,
	right_t,
	up_t,
	down_t,

};

unsigned int vao1, vao2, vao3, vao4;
unsigned int vbo1, vbo2, vbo3, vbo4;
unsigned int ibo1, ibo2, ibo3, ibo4;

gmtl::Matrix44f translate_Matrix;
gmtl::Matrix44f rotation_Matrix;
gmtl::Matrix44f xRot_Mat;
gmtl::Matrix44f yRot_Mat;
gmtl::Matrix44f zRot_Mat;

gmtl::Matrix44f scale_Matrix;
gmtl::Matrix44f world_Matrix;

float rot_factor = gmtl::Math::deg2Rad(5.0f);
const float mov_factor = 0.1f;
// Vertex array object


// Vertex buffer objects

int sphereIndexSize;
int axisIndexSize;

//vertex attribute locations
GLuint vertex_position;
GLuint vertex_color;

//uniform variable locations
unsigned int proj_matrix_loc;
unsigned int view_matrix_loc;
unsigned int model_matrix_loc;


static const double pi = 3.1415926;

#pragma region CreateGeometry
void GenerateCircleVerts(std::vector<float> &start_verts, int np);
void RotatePoints(std::vector<float> &vertices, int nm, int np, char rot);
void CreateSphere(int nm, int np, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<float> &color_data, float radius);
void CreateAxis(int nm, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<float> &color_data, char loc);
void SetIndices(int nm, int np, std::vector<unsigned int> &indices);
void GenerateColor(std::vector<float> &vertices, std::vector<float> &color_data);
void UnwrapPointToArray(gmtl::Point3f &point, std::vector<float> &array);
void GenerateBuffers(unsigned int &vao, unsigned int &vbo, unsigned int &ibo, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<float> &color_data);
#pragma endregion

#pragma region DrawGeometry
void DrawObject(unsigned int &v, unsigned int &b, unsigned int &i, gmtl::Matrix44f parent);
void DrawSphere(unsigned int &v, unsigned int &b, unsigned int &i);
void DrawAxis(unsigned int &v, unsigned int &b, unsigned int &i);
#pragma endregion

#pragma region Transformation
void DoRotate(RotationState state, char transform)
{
	bool debug = false;
	switch (state)
	{
#pragma region PosXLocalWorldRotation
	case Rpos_x:
		if (transform == 'l')
		{
			if (debug)
				std::cout << "positive local X rotation" << std::endl;
			rotation_Matrix.set(
				1.0f, 0.0, 0.0, 0.0,
				0.0, cos(rot_factor), -sin(rot_factor), 0.0,
				0.0, sin(rot_factor), cos(rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= rotation_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			if (debug)
				std::cout << "positive world X rotation" << std::endl;
			rotation_Matrix.set(
				1.0f, 0.0, 0.0, 0.0,
				0.0, cos(rot_factor), -sin(rot_factor), 0.0,
				0.0, sin(rot_factor), cos(rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = rotation_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion
#pragma region NegXLocalWorldRotation
	case Rneg_x:
		if (transform == 'l')
		{
			if (debug)
				std::cout << "negative local x rotation" << std::endl;
			rotation_Matrix.set(
				1.0f, 0.0, 0.0, 0.0,
				0.0, cos(-rot_factor), -sin(-rot_factor), 0.0,
				0.0, sin(-rot_factor), cos(-rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= rotation_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			if (debug)
				std::cout << "negative local x rotation" << std::endl;
			rotation_Matrix.set(
				1.0f, 0.0, 0.0, 0.0,
				0.0, cos(-rot_factor), -sin(-rot_factor), 0.0,
				0.0, sin(-rot_factor), cos(-rot_factor), 0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = rotation_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
#pragma endregion
#pragma region PosYLocalWorldRotation
	case Rpos_y:
		if (transform == 'l')
		{
			if (debug)
				std::cout << "positive local y rotation" << std::endl;
			rotation_Matrix.set(
				cos(rot_factor), 0.0, sin(rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(rot_factor), 0.0, cos(rot_factor), 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= rotation_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			if (debug)
				std::cout << "positive world y rotation" << std::endl;
			rotation_Matrix.set(
				cos(rot_factor), 0.0, sin(rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(rot_factor), 0.0, cos(rot_factor), 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = rotation_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion
#pragma region NegYLocalWorldYRotation
	case Rneg_y:
		if (transform == 'l')
		{
			if (debug)
				std::cout << "negative local Y rotation" << std::endl;
			rotation_Matrix.set(
				cos(-rot_factor), 0.0, sin(-rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(-rot_factor), 0.0, cos(-rot_factor), 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= rotation_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			if (debug)
				std::cout << "negative world Y rotation" << std::endl;
			rotation_Matrix.set(
				cos(-rot_factor), 0.0, sin(-rot_factor), 0.0,
				0.0, 1.0, 0.0, 0.0,
				-sin(-rot_factor), 0.0, cos(-rot_factor), 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = rotation_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion
#pragma region PosZLocalWorldRotation
	case Rpos_z:
		if (transform == 'l')
		{
			if (debug)
				std::cout << "positive local z rotation" << std::endl;
			rotation_Matrix.set(
				cos(rot_factor), sin(rot_factor), 0.0, 0.0,
				-sin(rot_factor), cos(rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= rotation_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			if (debug)
				std::cout << "positive world z rotation" << std::endl;
			rotation_Matrix.set(
				cos(rot_factor), sin(rot_factor), 0.0, 0.0,
				-sin(rot_factor), cos(rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = rotation_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion
#pragma region NegZLocalWorldRotation
	case Rneg_z:
		if (transform == 'l')
		{
			if (debug)
				std::cout << "local z rotation negative" << std::endl;
			rotation_Matrix.set(
				cos(-rot_factor), sin(-rot_factor), 0.0, 0.0,
				-sin(-rot_factor), cos(-rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= rotation_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			if (debug)
				std::cout << "world z rotation negative" << std::endl;
			rotation_Matrix.set(
				cos(-rot_factor), sin(-rot_factor), 0.0, 0.0,
				-sin(-rot_factor), cos(-rot_factor), 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = rotation_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}

#pragma endregion

	default:
		break;
		}
	}
}
void DoTranslate(TranslationState state, char transform)
{
	bool debug = true;
#pragma region PosNegWorldLocalYTranslation
	switch (state)
	{
	case up_t:
		if (transform == 'l')
		{
			if (debug)
				std::cout << "move up local" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= translate_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			if (debug)
				std::cout << "move up world" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = translate_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}

	case down_t:
		if (transform == 'l')
		{
			std::cout << "move down local" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, -mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= translate_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			std::cout << "move down world" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, -mov_factor,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);

			world_Matrix = translate_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion

#pragma region PosNegWorldLocalXTranslation
	case left_t:
		if (transform == 'l')
		{
			std::cout << "neg x local" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, -mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
				);
			world_Matrix *= translate_Matrix;

			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			std::cout << "neg x world" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, -mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = translate_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}
	case right_t:
		if (transform == 'l')
		{
			std::cout << "pos x local" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix *= translate_Matrix;
			glutPostRedisplay();
			break;
		}
		else if (transform == 'w')
		{
			std::cout << "pos x world" << std::endl;
			translate_Matrix.set(
				1.0, 0.0, 0.0, mov_factor,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
			world_Matrix = translate_Matrix * world_Matrix;
			glutPostRedisplay();
			break;
		}
#pragma endregion
	default:
		break;
	}
}
#pragma endregion
#endif