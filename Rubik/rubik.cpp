#include "rubik.h"
#include <time.h>

//////////////////////////////////////////////////////////////////////////
// Animations
#define DURATION_ANIMATION 750
#define LOOP_TIMER_ANIMATION 50
#define DURATION_FAST_ANIMATION 100
#define LOOP_TIMER_FAST_ANIMATION 10
BEnode* animation_root = nullptr;
glm::mat4 animation_transformation = glm::mat4(1);
unsigned int animation_count_left = 0;

bool should_shuffle = false;
bool solving = false;
std::vector<Rubik::Step> shuffle_movements_;
std::vector<Rubik::Step> solve_movements_;

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

		if (should_shuffle)
		{
			shuffle_movements_.pop_back();
			if (shuffle_movements_.size() > 0)
			{
				Rubik::cube->RotateFace(shuffle_movements_.back().face, shuffle_movements_.back().inverted, true);
			}
			else
			{
				should_shuffle = false;
			}
		}

		if (solving)
		{
			solve_movements_.pop_back();
			if (solve_movements_.size()>0)
			{
				Rubik::cube->RotateFace(solve_movements_.back().face, solve_movements_.back().inverted, true);
			}
			else
			{
				solving = false;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
Rubik* Rubik::cube = nullptr;

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


void Rubik::RotateFace(Face face, bool inverted, bool fast_animation)
{
	if (animation_count_left > 0 || ((should_shuffle || solving) && !fast_animation)){
		std::cout << "Animation in progress... input not allowed" << std::endl;
		return;
	}

	BEnode* faces_to_swap[9];
	int index = 0;
	animation_root = new BEnode("animation_root", BEnode::ROOT);

	switch (face)
	{
	case Rubik::U_FACE:
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][2][j];

		// Setup the animation
		glm::vec3 axis = glm::vec3(0, 0, (!inverted ? -1 : 1));
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[j][2][2 - i] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2 - j][2][i] = faces_to_swap[index++];
		}

	}
	break;
	case Rubik::R_FACE:
		//y doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[2][i][j];

		// Setup the animation
		glm::vec3 axis = glm::vec3((!inverted ? -1 : 1), 0, 0);
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2][2 - j][i] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2][j][2 - i] = faces_to_swap[index++];
		}
	}
	break;
	case Rubik::D_FACE:
		//y doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][0][j];

		// Setup the animation
		glm::vec3 axis = glm::vec3(0, 0, (!inverted ? 1 : -1));
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2 - j][0][i] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[j][0][2 - i] = faces_to_swap[index++];
		}
	}
	break;
	case Rubik::L_FACE:
		//x doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[0][i][j];

		// Setup the animation
		glm::vec3 axis = glm::vec3((!inverted ? 1 : -1), 0, 0);
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[0][j][2 - i] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[0][2 - j][i] = faces_to_swap[index++];
		}
	}
	break;
	case Rubik::F_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][j][0];

		// Setup the animation
		glm::vec3 axis = glm::vec3(0, (!inverted ? 1 : -1), 0);
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[j][2 - i][0] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2 - j][i][0] = faces_to_swap[index++];
		}
	}
	break;
	case Rubik::B_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][j][2];

		// Setup the animation
		glm::vec3 axis = glm::vec3(0, (!inverted ? -1 : 1), 0);
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2 - j][i][2] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[j][2 - i][2] = faces_to_swap[index++];
		}
	}
	break;
	case Rubik::M_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[1][i][j];

		// Setup the animation
		glm::vec3 axis = glm::vec3((!inverted ? -1 : 1), 0, 0);
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[1][2 - j][i] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[1][j][2 - i] = faces_to_swap[index++];
		}
	}
	break;
	case Rubik::MF_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][1][j];

		// Setup the animation
		glm::vec3 axis = glm::vec3(0, 0, (!inverted ? 1 : -1));
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2 - j][1][i] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[j][1][2 - i] = faces_to_swap[index++];
		}
	}
	break;
	case Rubik::ML_FACE:
		//z doesn't change
	{
		//gather the cubes to rotate
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				faces_to_swap[index++] = cube_faces_[i][j][1];

		// Setup the animation
		glm::vec3 axis = glm::vec3(0, (!inverted ? 1 : -1), 0);
		SetupAnimation(faces_to_swap, axis, fast_animation);

		// Updating the position of each cube in our 3x3 matrix
		index = 0;
		if (!inverted)
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[j][2 - i][1] = faces_to_swap[index++];
		}
		else
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					cube_faces_[2 - j][i][1] = faces_to_swap[index++];
		}
	}
	break;
	default:
		break;
	}

	if (!solving)
	{
		Step tmp;
		tmp.face = face;
		tmp.inverted = !inverted;
		solve_movements_.push_back(tmp);
	}
}


/************************************
 * Method:	SetupAnimation
 * Desc:	Setup the animation for the current rotation
 *
 * @param	BEnode** faces_to_swap, glm::vec3 axis
 ************************************/
void Rubik::SetupAnimation(BEnode** faces_to_swap, glm::vec3 axis, bool fast_animation)
{
	// Temporary save the parent of the cubes (every cube has same parent: the center of cube)
	BEnode* cube_root = faces_to_swap[4]->GetParent();

	// Adding faces to swap to the "virtual animation root"
	BuildSceneGraph(animation_root, faces_to_swap);

	if (fast_animation)
	{
		// Setup the rotate-transformation
		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_FAST_ANIMATION) / ((float)DURATION_FAST_ANIMATION), axis);
		animation_count_left = DURATION_FAST_ANIMATION / LOOP_TIMER_FAST_ANIMATION;
		// Call the function for the animation
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_FAST_ANIMATION);
	}
	else
	{
		// Setup the rotate-transformation
		animation_transformation = glm::rotate(glm::mat4(1), glm::half_pi<float>() * ((float)LOOP_TIMER_ANIMATION) / ((float)DURATION_ANIMATION), axis);
		animation_count_left = DURATION_ANIMATION / LOOP_TIMER_ANIMATION;
		// Call the function for the animation
		BEengine::GetInstance()->AddTimerCallBack(Animation, LOOP_TIMER_ANIMATION);
	}

	// Restoring previous parent of faces_to_swap
	RestoreSceneGraph(cube_root, faces_to_swap);
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

void Rubik::RestoreSceneGraph(BEnode* parent, BEnode** faces_to_swap)
{
	for (int i = 0; i < 9; i++)
	{
		faces_to_swap[i]->SetParent(parent);
	}
}

void Rubik::ShuffleCube()
{
	if (should_shuffle || solving)
	{
		std::cout << "Busy... " << std::endl;
		return;
	}

	should_shuffle = true;

	Face all_faces[FACE_MAX] = {
		U_FACE,
		R_FACE,
		D_FACE,
		L_FACE,
		F_FACE,
		B_FACE,
		M_FACE,
		MF_FACE,
		ML_FACE };

	srand(time(0));

	for (int i = 0; i < 10; i++)
	{
		Step tmp;
		tmp.face = all_faces[(rand() % FACE_MAX + 1) - 1];
		tmp.inverted = rand() % 2 == 1;

		shuffle_movements_.push_back(tmp);
	}

	Rubik::cube->RotateFace(shuffle_movements_.back().face, shuffle_movements_.back().inverted, true);
}

void Rubik::SolveCube()
{
	if (solving || should_shuffle)
	{
		std::cout << "Busy... " << std::endl;
		return;
	}

	if (solve_movements_.size() == 0)
	{
		std::cout << "Nothing to solve..." << std::endl;
		return;
	}
	solving = true;

	Rubik::cube->RotateFace(solve_movements_.back().face, solve_movements_.back().inverted, true);
}

