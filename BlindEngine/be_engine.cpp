#include "be_engine.h"

BEengine* BEengine::instance_ = nullptr;


BEengine* BEengine::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new BEengine();
	}
	return instance_;
}

BEengine::BEengine()
{
}


BEengine::~BEengine()
{
	delete instance_;
}

void BEengine::Init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[])
{
	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(x_position, y_position);

	// FreeGLUT can parse command-line params, in case:
	glutInit(&argc, argv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Create the window with a specific title:   
	window_id_ = glutCreateWindow(window_name);
	BEengine::initialized_ = true;
}

int BEengine::Start()
{
	if (BEengine::initialized_)
	{
		// Enter the main FreeGLUT processing loop:     
		glutMainLoop();

		//application exited
		return EXIT_SUCCESS;
	}

	//the engine wasn't initialized
	return EXIT_FAILURE;
}

// Return the index of the light
int AddLight(BElight *light){

	return -1; //TODO
}

// Return true on successful remove
bool RemoveLight(int indexLight){

	return false; //TODO
}