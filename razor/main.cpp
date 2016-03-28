#include "be_engine.h"


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
	BEengine* engine = BEengine::GetInstance();
	switch (key)
	{

	case '+':
		if (engine->GetDeltaPadding() < 50)
			engine->SetDeltaPadding(engine->GetDeltaPadding() + 0.5f);
		break;
	case '-':
		if (engine->GetDeltaPadding())
			engine->SetDeltaPadding(engine->GetDeltaPadding() - 0.5f);
		break;

	case '1':
	{
		BElight* light = (BElight*)BEnode::GetSuperRoot()->Find("Omni001");
		if (light)
			light->ToggleLight();
	}
		break;
	case '2':
	{
		BElight* light = (BElight*)BEnode::GetSuperRoot()->Find("Omni002");
		if (light)
			light->ToggleLight();
	}
		break;
	
	}
	engine->CalcTransformation();
}

int main(int argc, char *argv[])
{

	BEengine *engine = BEengine::GetInstance();

	/************************************************************************/
	/* Init + Preferences
	/************************************************************************/
	engine->Init("BlindEngine - SimpleScene", 100, 100, 1024, 768, nullptr, argc, argv);
	engine->SetDeltaPadding(2.f);
	engine->SetKeyboardCallBack(keyboardCallback);
	engine->SetSpecialCallBack(specialCallback);


	/************************************************************************/
	/* Load scenes
	/************************************************************************/
	//BEnode* cube_root = engine->LoadScene("Rubik_No_Light.DAE");
	//engine->LoadScene("no_grass.DAE");
	engine->LoadScene("simple_scene.DAE");


	/************************************************************************/
	/* Start BlindEngine
	/************************************************************************/
	engine->Start();

	return EXIT_SUCCESS;
}