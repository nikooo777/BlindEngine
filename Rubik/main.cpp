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
	static BEnode* node_selected = BEnode::GetSuperRoot()->Find("Rubik_No_Light");
	switch (key)
	{
	case ' ':
		//automatic = !automatic;
		break;
	case 'a':
	{
		//globalX -= BEengine::GetInstance()->GetDeltaPadding();
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(-BEengine::GetInstance()->GetDeltaPadding(), 0.0f, 0.0f));
		glm::mat4 new_translation = translation * node_selected->GetTransformation();
		node_selected->SetTransformation(new_translation);
	}
		break;
	case 'd':
	{
		//globalX += BEengine::GetInstance()->GetDeltaPadding();

		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(BEengine::GetInstance()->GetDeltaPadding(), 0.0f, 0.0f));
		glm::mat4 new_translation = translation * node_selected->GetTransformation();
		node_selected->SetTransformation(new_translation);
	}
		break;
	case 'w':
	{ // Necessary for scope
		//globalY += BEengine::GetInstance()->GetDeltaPadding();

		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, BEengine::GetInstance()->GetDeltaPadding(), 0.0f));
		glm::mat4 new_translation = translation * node_selected->GetTransformation();
		node_selected->SetTransformation(new_translation);
	}
		break;
	case 's':
	{ // Necessary for scope
		//globalY -= BEengine::GetInstance()->GetDeltaPadding();

		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, -BEengine::GetInstance()->GetDeltaPadding(), 0.0f));
		glm::mat4 new_translation = translation * node_selected->GetTransformation();
		node_selected->SetTransformation(new_translation);
	}
		break;
	case 'q':
	{
		//globalX -= BEengine::GetInstance()->GetDeltaPadding();
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -BEengine::GetInstance()->GetDeltaPadding()));
		glm::mat4 new_translation = translation * node_selected->GetTransformation();
		node_selected->SetTransformation(new_translation);
		break;
	}
	case 'y':
	{
		//globalX -= BEengine::GetInstance()->GetDeltaPadding();
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, BEengine::GetInstance()->GetDeltaPadding()));
		glm::mat4 new_translation = translation * node_selected->GetTransformation();
		node_selected->SetTransformation(new_translation);
		break;
	}

	case '+':
		BEengine::GetInstance()->SetDeltaPadding(BEengine::GetInstance()->GetDeltaPadding() + 0.5f);
		break;
	case '-':
		BEengine::GetInstance()->SetDeltaPadding(BEengine::GetInstance()->GetDeltaPadding() - 0.5f);
		break;

	case '1':
		node_selected = BEnode::GetSuperRoot()->Find("Rubik_No_Light");
		break;
	case '2':
		node_selected = BEnode::GetSuperRoot()->Find("Box001");
		break;
	case '3':
		node_selected = BEnode::GetSuperRoot()->Find("Cone001");
		break;
	case '4':
		node_selected = BEnode::GetSuperRoot()->Find("Sphere001");
		break;
	case '5':
		node_selected = BEnode::GetSuperRoot()->Find("Torus Knot001");
		break;
	case '6':
		node_selected = BEnode::GetSuperRoot()->Find("Teapot001");
		break;
	}
	BEengine::GetInstance()->set_node_selected(node_selected->get_name());
}
int main(int argc, char *argv[])
{
	
	BEengine *engine = BEengine::GetInstance();

	/************************************************************************/
	/* Init + Preferences
	/************************************************************************/
	engine->Init("BlindEngine - Rubik", 100, 100, 800, 600, nullptr, argc, argv);
	engine->SetDeltaPadding(.5f);
	engine->SetKeyBoardCallBack(keyboardCallback);


	/************************************************************************/
	/* Load scenes
	/************************************************************************/
	BEnode* cube_root = engine->LoadScene("Rubik_No_Light.DAE");

	engine->LoadScene("modellosore.DAE");
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