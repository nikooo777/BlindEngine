#include "BEengine.h"

BEengine* BEengine::__instance = nullptr;


BEengine* BEengine::getInstance()
{
	if (__instance == nullptr)
	{
		__instance = new BEengine();
	}
	return __instance;
}

BEengine::BEengine()
{
}


BEengine::~BEengine()
{
}

void BEengine::init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[])
{
	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(x_position, y_position);

	// FreeGLUT can parse command-line params, in case:
	glutInit(&argc, argv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Create the window with a specific title:   
	window_id = glutCreateWindow(window_name);
	BEengine::initialized = true;
}

int BEengine::start()
{
	if (BEengine::initialized)
	{
		// Enter the main FreeGLUT processing loop:     
		glutMainLoop();

		//application exited
		return EXIT_SUCCESS;
	}

	//the engine wasn't initialized
	return EXIT_FAILURE;
}