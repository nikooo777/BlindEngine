#include <iostream>
#include "be_engine.h"
#include "rubik.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This callback is invoked each time a special keyboard key is pressed.
* @param key key pressed id
* @param mouseX mouse X coordinate
* @param mouseY mouse Y coordinate
*/
void specialCallback(int key, int mouseX, int mouseY)
{
	BEengine* engine = BEengine::GetInstance();
	// Change box rotation:
	const float speed = engine->GetDeltaPadding();
	switch (key)
	{
	case GLUT_KEY_UP:
		(*engine->get_angles()).x -= speed;
		break;

	case GLUT_KEY_DOWN:
		(*engine->get_angles()).x += speed;
		break;

	case GLUT_KEY_LEFT:
		(*engine->get_angles()).y -= speed;
		break;

	case GLUT_KEY_RIGHT:
		(*engine->get_angles()).y += speed;
		break;

	case GLUT_KEY_PAGE_DOWN:
		*engine->get_distance() += engine->GetDeltaZoom();
		break;

	case GLUT_KEY_PAGE_UP:
		*engine->get_distance() -= engine->GetDeltaZoom();
		break;
	}

	BEengine::GetInstance()->CalcTransformation();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This callback is invoked each time a standard keyboard key is pressed.
* @param key key pressed id
* @param mouseX mouse X coordinate
* @param mouseY mouse Y coordinate
*/
void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
	static BEnode* rubik_root_node = BEnode::GetSuperRoot()->Find("Rubik_Downloaded");
	static Rubik* cube = new Rubik(rubik_root_node);
	BEengine* engine = BEengine::GetInstance();
	switch (key)
	{
		//navigation
	case 'a':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(-engine->GetDeltaPadding(), 0.0f, 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 'd':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(engine->GetDeltaPadding(), 0.0f, 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 'w':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, engine->GetDeltaPadding(), 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 's':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, -engine->GetDeltaPadding(), 0.0f));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
	}
		break;
	case 'q':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -engine->GetDeltaPadding()));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
		break;
	}
	case 'y':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, engine->GetDeltaPadding()));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
		break;
	}
	case '+':
		engine->SetDeltaPadding(engine->GetDeltaPadding() + 0.5f);
		break;
	case '-':
		engine->SetDeltaPadding(engine->GetDeltaPadding() - 0.5f);
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
	case 'n':
		cube->RotateFace(Rubik::N_FACE, false);
		break;
	case '1':
	{
		BElight* light = (BElight*)BEnode::GetSuperRoot()->Find("Spot001");
		if (light)
			light->ToggleLight();
	}
		break;
	case '2':
	{
		BElight* light = (BElight*)BEnode::GetSuperRoot()->Find("Spot002");
		if (light)
			light->ToggleLight();
	}
		break;
	case '3':
	{
		BElight* light = (BElight*)BEnode::GetSuperRoot()->Find("Omni001");
		if (light)
			light->SetDiffuse(glm::vec3(0.2f,0.2f,0.2f));
	}
	break;
	case '4':
	{
		BElight* light = (BElight*)BEnode::GetSuperRoot()->Find("Omni001");
		if (light)
			light->SetDiffuse(glm::vec3(.9f, .9f, .9f));
	}
	break;
	}
	engine->set_node_selected(rubik_root_node->get_name());
	engine->CalcTransformation();
}

void addMeshToList(BEmesh*mesh)
{
	BEengine::GetInstance()->lists_->AddMirrored(mesh);
	mesh->SetIsMirrored(true);
}

int main(int argc, char *argv[])
{

	BEengine *engine = BEengine::GetInstance();

	/************************************************************************/
	/* Init + Preferences
	/************************************************************************/
	engine->Init("BlindEngine - Rubik", 100, 100, 1024, 768, nullptr, argc, argv);
	engine->SetDeltaPadding(5.f);
	engine->SetKeyboardCallBack(keyboardCallback);
	engine->SetSpecialCallBack(specialCallback);


	/************************************************************************/
	/* Load scenes
	/************************************************************************/
	//BEnode* cube_root = engine->LoadScene("Rubik_No_Light.DAE");
	//engine->LoadScene("no_grass.DAE");
	engine->LoadScene("scene_final.DAE");



	/************************************************************************/
	/* Init Rubik's cube
	/************************************************************************/
	BEmaterial* plane = ((BEmesh*)BEnode::GetSuperRoot()->Find("MarmSurface"))->get_material();
	plane->SetTransparency(0.6f);

	BEnode* tmp_rubik = BEnode::GetSuperRoot()->Find("Rubik_Downloaded");

	for (BEnode* child : tmp_rubik->GetChildren())
	{
		addMeshToList((BEmesh*)child);
	}


	// Lamp001
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("Cylinder001"));
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("C_clamp405"));
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("C_lamp_white"));
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("Sphere001"));
	// Lamp002
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("Cylinder002"));
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("C_clamp406"));
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("C_Clamp"));
	addMeshToList((BEmesh*)BEnode::GetSuperRoot()->Find("Sphere002"));
	

	/************************************************************************/
	/* Start BlindEngine
	/************************************************************************/
	engine->Start();

	return EXIT_SUCCESS;
}