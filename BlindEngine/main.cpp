#include <iostream>
#include "be_engine.h"
#include "be_logging.h"
#include "be_scene_loader.h"
int main(int argc, char *argv[])
{
	be_logging::log("Welcome to Blind Engine 1.0");
	BEengine *engine = BEengine::GetInstance();
	be_logging::log("Instance created");
	engine->Init("BlindEngine - Rubik", 100, 100, 800, 600, nullptr, argc, argv);
	be_logging::log("Engine initialized");
	be_logging::log("Starting Engine...");

	//engine->LoadScene("Rubik_Downloaded.DAE");
	engine->LoadScene("modellosore.DAE");
	
	engine->Start();
	be_logging::log("Program exiting...");
	return EXIT_SUCCESS;
}