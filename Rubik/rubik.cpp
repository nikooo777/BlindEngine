#include "rubik.h"


Rubik::Rubik(BEnode* cube_root)
{
	cube_root_ = cube_root;
	// z=0
	cube_faces_[0][0][0] = cube_root->Find("Block59");
	cube_faces_[1][0][0] = cube_root->Find("Block49");
	cube_faces_[2][0][0] = cube_root->Find("Block50");

	cube_faces_[0][1][0] = cube_root->Find("Block47");
	cube_faces_[1][1][0] = cube_root->Find("Block57");
	cube_faces_[2][1][0] = cube_root->Find("Block58");

	cube_faces_[0][2][0] = cube_root->Find("Block68");
	cube_faces_[1][2][0] = cube_root->Find("Block66");
	cube_faces_[2][2][0] = cube_root->Find("Block67");

	// z=1
	cube_faces_[0][0][1] = cube_root->Find("Block48");
	cube_faces_[1][0][1] = cube_root->Find("Block54");
	cube_faces_[2][0][1] = cube_root->Find("Block51");

	cube_faces_[0][1][1] = cube_root->Find("Block56");
	cube_faces_[1][1][1] = cube_root->Find("Block62"); // Central block -> Pivot
	cube_faces_[2][1][1] = cube_root->Find("Block55");

	cube_faces_[0][2][1] = cube_root->Find("Block65");
	cube_faces_[1][2][1] = cube_root->Find("Block71");
	cube_faces_[2][2][1] = cube_root->Find("Block64");

	// z=2
	cube_faces_[0][0][2] = cube_root->Find("Block46");
	cube_faces_[1][0][2] = cube_root->Find("Block53");
	cube_faces_[2][0][2] = cube_root->Find("Block52");

	cube_faces_[0][1][2] = cube_root->Find("Block63");
	cube_faces_[1][1][2] = cube_root->Find("Block61");
	cube_faces_[2][1][2] = cube_root->Find("Block60");

	cube_faces_[0][2][2] = cube_root->Find("Block72");
	cube_faces_[1][2][2] = cube_root->Find("Block70");
	cube_faces_[2][2][2] = cube_root->Find("Block69");


	/*for each (BEnode* n in cube_faces_)
	{
	if (n == nullptr)
	throw "A reference to a block is invalid";
	}*/
}


Rubik::~Rubik()
{
}


/************************************
* Method:	TranslateSingleCubeX
* Desc:	Translate a single cube in axis-x
*
* @param	unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x
* @return	void
************************************/
void Rubik::TranslateSingleCubeX(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x)
{
	glm::mat4 f = cube_faces_[element_x][element_y][element_z]->GetTransformation();

	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(value_x, 0.0f, 0.0f));
	glm::mat4 new_translation = translation * f;
	cube_faces_[element_x][element_y][element_z]->SetTransformation(new_translation);
}


/************************************
* Method:	TranslateSingleCubeY
* Desc:	Translate a single cube in axis-y
*
* @param	unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_y
* @return	void
************************************/
void Rubik::TranslateSingleCubeY(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_y)
{
	glm::mat4 f = cube_faces_[element_x][element_y][element_z]->GetTransformation();

	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, value_y, 0.0f));
	glm::mat4 new_translation = translation * f;
	cube_faces_[element_x][element_y][element_z]->SetTransformation(new_translation);
}

/************************************
* Method:	TranslateSingleCubeZ
* Desc:		Translate a single cube in axis-z
*
* @param	unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_z
* @return	void
************************************/
void Rubik::TranslateSingleCubeZ(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_z)
{
	glm::mat4 f = cube_faces_[element_x][element_y][element_z]->GetTransformation();

	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, value_z));
	glm::mat4 new_translation = translation * f;
	cube_faces_[element_x][element_y][element_z]->SetTransformation(new_translation);
}


/************************************
* Method:	TranslateSingleCube
* Desc:	Translate a single cube in all axis
*
* @param	unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x, float value_y, float value_z
* @return	void
************************************/
void Rubik::TranslateSingleCube(unsigned short element_x, unsigned short element_y, unsigned short element_z, float value_x, float value_y, float value_z)
{
	glm::mat4 f = cube_faces_[element_x][element_y][element_z]->GetTransformation();

	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(value_x, value_y, value_z));
	glm::mat4 new_translation = translation * f;
	cube_faces_[element_x][element_y][element_z]->SetTransformation(new_translation);
}

void Rubik::RotateFace(Face face, bool inverse)
{
	std::cout << "Rotating" << std::endl;
	switch (face)
	{
	case Rubik::U_FACE:
		//y doesn't change
		break;
	case Rubik::R_FACE:
		//y doesn't change
		break;
	case Rubik::D_FACE:
		//x doesn't change
		break;
	case Rubik::L_FACE:
		//x doesn't change
		break;
	case Rubik::F_FACE:
		//z doesn't change
	{
		BEnode* faces_to_swap[9];
		int index = 0;
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				//x123 -> y123 /^
				std::cout << "index: " << index << ": " << cube_faces_[i][j][0]->get_name() << std::endl;
				faces_to_swap[index++] = cube_faces_[i][j][0];
			}
		}
		std::cout << "grabbed nodes" << std::endl;

		BEnode* rotation_helper = new BEnode("rotation_helper", BEnode::ROOT);
		faces_to_swap[4]->AddChild(rotation_helper);
		rotation_helper->SetTransformation(glm::inverse(faces_to_swap[4]->GetTransformation()));
		for (int i = 0; i < 9; i++)
		{
			if (i != 4)
			{
				std::cout << "index: " << i << ": " << faces_to_swap[i]->get_name()<< std::endl;
				faces_to_swap[i]->GetParent()->RemoveChild(faces_to_swap[i]);
				std::cout << "child removed" << std::endl;
				rotation_helper->AddChild(faces_to_swap[i]);
				std::cout << "Parent set and child added" << std::endl;
			}
		}
		glm::mat4 f = faces_to_swap[4]->GetTransformation();
		glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::half_pi<float>(), glm::vec3(0, 1, 0))*f;
		faces_to_swap[4]->SetTransformation(rotation);

		for (int i = 0; i < 9; i++)
		{
			if (i != 4)
			{
				faces_to_swap[i]->SetTransformation(faces_to_swap[i]->GetParent()->CalcTransformation())
				std::cout << "child removed" << std::endl;
				rotation_helper->AddChild(faces_to_swap[i]);
				std::cout << "Parent set and child added" << std::endl;
			}
		}
		//glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(value_x, 0.0f, 0.0f));
		//glm::mat4 new_translation = translation * f;
		//cube_faces_[element_x][element_y][element_z]->SetTransformation(new_translation);


		//update the cube matrix
		index = 0;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cube_faces_[i][j][0] = faces_to_swap[index++];
			}
		}
	}

		break;
	case Rubik::B_FACE:
		//z doesn't change
		break;
	default:
		break;
	}
}