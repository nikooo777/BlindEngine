#include <iostream>
#include "be_engine.h"
#include "rubik.h"

int main(int argc, char *argv[])
{
	
	BEengine *engine = BEengine::GetInstance();

	/************************************************************************/
	/* Init + Preferences
	/************************************************************************/
	engine->Init("BlindEngine - Rubik", 100, 100, 800, 600, nullptr, argc, argv);
	engine->SetDeltaPadding(.5f);


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