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
		if ((*engine->get_angles()).x < 0)
			(*engine->get_angles()).x = 0;
		break;

	case GLUT_KEY_DOWN:
		(*engine->get_angles()).x += speed;
		if ((*engine->get_angles()).x > 180)
			(*engine->get_angles()).x = 180;
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
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, engine->GetDeltaPadding()));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
		break;
	}
	case 'y':
	{
		glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -engine->GetDeltaPadding()));
		glm::mat4 new_translation = translation * rubik_root_node->GetTransformation();
		rubik_root_node->SetTransformation(new_translation);
		break;
	}
	case '+':
		if(engine->GetDeltaPadding() < 50)
			engine->SetDeltaPadding(engine->GetDeltaPadding() + 0.5f);
		break;
	case '-':
		if (engine->GetDeltaPadding())
			engine->SetDeltaPadding(engine->GetDeltaPadding() - 0.5f);
		break;

		/************************************************************************/
		/* Cube controls                                                        */
		/************************************************************************/
	case 'f':
		Rubik::cube->RotateFace(Rubik::F_FACE, false);
		break;
	case 'F':
		Rubik::cube->RotateFace(Rubik::F_FACE, true);
		break;
	case 'b':
		Rubik::cube->RotateFace(Rubik::B_FACE, false);
		break;
	case 'B':
		Rubik::cube->RotateFace(Rubik::B_FACE, true);
		break;
	case 'r':
		Rubik::cube->RotateFace(Rubik::R_FACE, false);
		break;
	case 'R':
		Rubik::cube->RotateFace(Rubik::R_FACE, true);
		break;
	case 'l':
		Rubik::cube->RotateFace(Rubik::L_FACE, false);
		break;
	case 'L':
		Rubik::cube->RotateFace(Rubik::L_FACE, true);
		break;
	case 'u':
		Rubik::cube->RotateFace(Rubik::U_FACE, false);
		break;
	case 'U':
		Rubik::cube->RotateFace(Rubik::U_FACE, true);
		break;
	case 'g':
		Rubik::cube->RotateFace(Rubik::D_FACE, false);
		break;
	case 'G':
		Rubik::cube->RotateFace(Rubik::D_FACE, true);
		break;
	case 'm':
		Rubik::cube->RotateFace(Rubik::M_FACE, false);
		break;
	case 'M':
		Rubik::cube->RotateFace(Rubik::M_FACE, true);
		break;
	case 'k':
		Rubik::cube->RotateFace(Rubik::MF_FACE, false);
		break;
	case 'K':
		Rubik::cube->RotateFace(Rubik::MF_FACE, true);
		break;
	case 'j':
		Rubik::cube->RotateFace(Rubik::ML_FACE, false);
		break;
	case 'J':
		Rubik::cube->RotateFace(Rubik::ML_FACE, true);
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
	case ' ':
		Rubik::cube->ShuffleCube();
		break;
	case 'p':
		Rubik::cube->SolveCube();
		break;
	}
	engine->CalcTransformation();
}

void AddReflection(BEmesh*mesh)
{
	BEengine::GetInstance()->lists_->AddMirrored(mesh);
	mesh->SetIsMirrored(true);
}

void RemoveShadow(BEmesh*mesh)
{
	BEengine::GetInstance()->lists_->RemoveShadowed(mesh);
	mesh->SetIsShadowed(false);
}

int main(int argc, char *argv[])
{

	BEengine *engine = BEengine::GetInstance();

	/************************************************************************/
	/* Init + Preferences
	/************************************************************************/
	engine->Init("BlindEngine - Rubik", 100, 100, 1024, 768, nullptr, argc, argv);
	engine->SetDeltaPadding(2.f);
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
	Rubik::cube = new Rubik(BEnode::GetSuperRoot()->Find("Rubik_Downloaded"));

	BEmaterial* plane = ((BEmesh*)BEnode::GetSuperRoot()->Find("MarmSurface"))->get_material();
	plane->SetTransparency(0.6f);
	RemoveShadow((BEmesh*)BEnode::GetSuperRoot()->Find("MarmSurface"));

	BEnode* tmp_rubik = BEnode::GetSuperRoot()->Find("Rubik_Downloaded");

	for (BEnode* child : tmp_rubik->GetChildren())
	{
		AddReflection((BEmesh*)child);
	}


	// Lamp001
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("Cylinder001"));
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("C_clamp405"));
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("C_lamp_white"));
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("Sphere001"));
	// Lamp002
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("Cylinder002"));
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("C_clamp406"));
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("C_Clamp"));
	AddReflection((BEmesh*)BEnode::GetSuperRoot()->Find("Sphere002"));

	// SkyBox
	RemoveShadow((BEmesh*)BEnode::GetSuperRoot()->Find("SkyBox"));
	// Plane
	RemoveShadow((BEmesh*)BEnode::GetSuperRoot()->Find("Plane001"));

	RemoveShadow((BEmesh*)BEnode::GetSuperRoot()->Find("Cylinder001"));
	RemoveShadow((BEmesh*)BEnode::GetSuperRoot()->Find("Cylinder002"));

	/************************************************************************/
	/* Start BlindEngine
	/************************************************************************/
	engine->Start();

	return EXIT_SUCCESS;
}