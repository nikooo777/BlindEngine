#include "rubik.h"

//////////////////////////////////////////////////////////////////////////
// Animations
#define DURATION_ANIMATION 750
#define LOOP_TIMER_ANIMATION 50
BEnode* animation_root = nullptr;
glm::mat4 animation_transformation = glm::mat4(1);
unsigned int animation_count_left = 0;

void Animation(int value)
{
	animation_root->UpdateTransformationRecursive(animation_transformation);
	BEengine::GetInstance()->CalcTransformation();

	animation_count_left--;
	if (animation_count_left)
	{
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);
	}
	else
	{
		delete animation_root;
	}
}
//////////////////////////////////////////////////////////////////////////


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
}


Rubik::~Rubik()
{
}


void Rubik::RotateFace(Face face, bool inverse)
{
	if (animation_count_left > 0){
		std::cout << "Animation in progress... input not allowed" << std::endl;
		return;
	}

	BEnode* faces_to_swap[9];
	int index = 0;
	animation_root = new BEnode("animation_root", BEnode::ROOT);
	BEnode* cube_root;

	switch (face)
	{
	case Rubik::U_FACE:
		//y doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][2][j];

		cube_root = faces_to_swap[4]->GetParent();

		BuildSceneGraph(animation_root, faces_to_swap);

		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), glm::vec3(0, 0, 1));
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);
		
		BuildSceneGraph(cube_root, faces_to_swap);

		index = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cube_faces_[2 - j][2][i] = faces_to_swap[index++]; // fixed

	}
		break;
	case Rubik::R_FACE:
		//y doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[2][i][j];


		cube_root = faces_to_swap[4]->GetParent();

		BuildSceneGraph(animation_root, faces_to_swap);

		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), glm::vec3(-1, 0, 0));
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);

		BuildSceneGraph(cube_root, faces_to_swap);

		index = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cube_faces_[2][2 - j][i] = faces_to_swap[index++]; // fixed
	}
		break;
	case Rubik::D_FACE:
		//y doesn't change
	{

		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][0][j];

		cube_root = faces_to_swap[4]->GetParent();

		BuildSceneGraph(animation_root, faces_to_swap);

		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), glm::vec3(0, 0, 1));
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);

		BuildSceneGraph(cube_root, faces_to_swap);

		index = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cube_faces_[2 - j][0][i] = faces_to_swap[index++]; // fixed
	}
		break;
	case Rubik::L_FACE:
		//x doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[0][i][j];

		cube_root = faces_to_swap[4]->GetParent();

		BuildSceneGraph(animation_root, faces_to_swap);

		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), glm::vec3(1, 0, 0));
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);

		BuildSceneGraph(cube_root, faces_to_swap);

		index = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cube_faces_[0][j][2 - i] = faces_to_swap[index++]; // Fixed
	}
		break;
	case Rubik::F_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][j][0];

		cube_root = faces_to_swap[4]->GetParent();

		BuildSceneGraph(animation_root, faces_to_swap);

		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), glm::vec3(0, 1, 0));
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);

		BuildSceneGraph(cube_root, faces_to_swap);

		index = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cube_faces_[j][2 - i][0] = faces_to_swap[index++]; // Fixed
	}
		break;
	case Rubik::B_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][j][2];

		cube_root = faces_to_swap[4]->GetParent();

		BuildSceneGraph(animation_root, faces_to_swap);

		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), glm::vec3(0, -1, 0));
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);

		BuildSceneGraph(cube_root, faces_to_swap);

		index = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cube_faces_[2 - j][i][2] = faces_to_swap[index++]; // Fixed
	}
		break;
	case Rubik::N_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[1][i][j];

		cube_root = faces_to_swap[4]->GetParent();

		BuildSceneGraph(animation_root, faces_to_swap);

		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), glm::vec3(-1, 0, 0));
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);

		BuildSceneGraph(cube_root, faces_to_swap);

		index = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				cube_faces_[1][2 - j][i] = faces_to_swap[index++]; // Fixed
	}
		break;
	default:
		break;
	}
}

void Rubik::BuildSceneGraph(BEnode* parent, BEnode** faces_to_swap)
{
	for (int i = 0; i < 9; i++)
	{

		//addchild adds a child to the vector of the parent and then calls setparent on the new child.
		//This call does not call RemoveChild from the previous parent if one was present
		//Forgetting about previous parents can leave loops in the scene graph
		parent->AddChild(faces_to_swap[i]);
	}
}

