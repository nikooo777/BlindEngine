#include <iostream>
#include "be_engine.h"
#include "rubik.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This callback is invoked each time a standard keyboard key is pressed.
* @param key key pressed id
* @param mouseX mouse X coordinate
* @param mouseY mouse Y coordinate
*/
void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	//std::cout << "[std key pressed]" << std::endl;
	static BEnode* rubik_root_node = BEnode::GetSuperRoot()->Find("Rubik_Downloaded");
	static Rubik* cube = new Rubik(rubik_root_node);
	switch (key)
	{
		//navigation
	case 'a':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(-BEengine::GetInstance()->GetDeltaPadding(), 0.0f, 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 'd':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(BEengine::GetInstance()->GetDeltaPadding(), 0.0f, 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 'w':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, BEengine::GetInstance()->GetDeltaPadding(), 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 's':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, -BEengine::GetInstance()->GetDeltaPadding(), 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 'q':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -BEengine::GetInstance()->GetDeltaPadding()));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
		break;
	}
	case 'y':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, BEengine::GetInstance()->GetDeltaPadding()));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
		break;
	}
	case '+':
		BEengine::GetInstance()->SetDeltaPadding(BEengine::GetInstance()->GetDeltaPadding() + 0.5f);
		break;
	case '-':
		BEengine::GetInstance()->SetDeltaPadding(BEengine::GetInstance()->GetDeltaPadding() - 0.5f);
		break;

		/************************************************************************/
		/* Cube controls                                                        */
		/************************************************************************/
	case 'f':
		cube->RotateFace(Rubik::F_FACE, false);
		break;
	case 'b':
		cube->RotateFace(Rubik::B_FACE, false);
		break;
	case 'r':
		cube->RotateFace(Rubik::R_FACE, false);
		break;
	case 'l':
		cube->RotateFace(Rubik::L_FACE, false);
		break;
	case 'u':
		cube->RotateFace(Rubik::U_FACE, false);
		break;
		//find something better than "ground". d should be the one used but it's already used, so either solve that or IDK
	case 'g':
		cube->RotateFace(Rubik::D_FACE, false);
		break;
	}
	BEengine::GetInstance()->set_node_selected(rubik_root_node->get_name());
}

int main(int argc, char *argv[])
{

	BEengine *engine = BEengine::GetInstance();

	/************************************************************************/
	/* Init + Preferences
	/************************************************************************/
	engine->Init("BlindEngine - Rubik", 100, 100, 800, 600, nullptr, argc, argv);
	engine->SetDeltaPadding(5.f);
	engine->SetKeyBoardCallBack(keyboardCallback);


	/************************************************************************/
	/* Load scenes
	/************************************************************************/
	//BEnode* cube_root = engine->LoadScene("Rubik_No_Light.DAE");
	engine->LoadScene("Rubik_Downloaded.DAE");
	//engine->LoadScene("modellosore.DAE");
	//engine->LoadScene("scene_texture.DAE");


	/************************************************************************/
	/* Init Rubik's cube
	/************************************************************************/
	//Rubik* rubik_cube = new Rubik(cube_root);

	// Test: Translation of a single cube

	//rubik_cube->TranslateSingleCubeY(0, 0, 0, 20.0f);
	//rubik_cube->TranslateSingleCube(0, 0, 0, -20.0f, 0.0f, -10.0f);

	//engine->lists_->GetMeshByName("Block46");


	/************************************************************************/
	/* Start BlindEngine
	/************************************************************************/
	engine->Start();

	return EXIT_SUCCESS;
}