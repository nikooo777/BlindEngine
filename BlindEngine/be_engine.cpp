#include "be_engine.h"


//////////////
// DLL MAIN //
//////////////

#ifdef WIN32
#include <Windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* DLL entry point. Avoid to rely on it for easier code portability (Linux doesn't use this method).
* @param instDLL handle
* @param reason reason
* @param _reserved reserved
* @return true on success, false on failure
*/
int APIENTRY DllMain(HANDLE instDLL, DWORD reason, LPVOID _reserved)
{
	// Check use:
	switch (reason)
	{
		///////////////////////////		
	case DLL_PROCESS_ATTACH: //		                     

		break;


		///////////////////////////
	case DLL_PROCESS_DETACH: //                 

		break;
	}

	// Done:
	return true;
}
#endif
#include "be_node.h"

/////////////
// GLOBALS //
/////////////

BEnode *root;

// Rotation angles:
float angleX = 0.0f, angleY = 0.0f;
float distance = -10.0f;
float globalX = .0f;
float globalY = .0f;

bool alreadyRendered = false;

// Matrices:

// Texture:
unsigned char *bitmap = NULL;
unsigned int texId = 0;
float tiling = 1.0f;
bool forceNewTexture = true;

// Auto/manual flag:
bool automatic = false;

// Frame counter:
int frames = 0;
float fps = 0.0f;

//these declarations are needed for the linker
BEengine* BEengine::instance_ = nullptr;
BElist* BEengine::lists_ = nullptr;
void PrintTextInfo();


LIB_API BEengine* BEengine::GetInstance()
{
	if (instance_ == nullptr)
	{
		std::cout << "*****************************" << std::endl;
		std::cout << "\tBlindEngine v" << LIB_VERSION / 10 << std::endl;
		std::cout << "*****************************" << std::endl;
		instance_ = new BEengine();
	}
	return instance_;
}

BEengine::BEengine()
{
	lists_ = new BElist();
}

BEengine::~BEengine()
{
	delete instance_;
	delete lists_;
}


///////////////
// CALLBACKS //
///////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This is the main rendering routine automatically invoked by FreeGLUT.
*/
void displayCallback()
{
	// Clear the screen:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//////
	// 3D:
	// Set perpsective matrix:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(BEengine::GetInstance()->get_perspective()));
	glMatrixMode(GL_MODELVIEW);

	// Set a matrix to move our triangle:
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(globalX, globalY, distance));

	//the following 2 lines will be obsolete when the special callback and/or the camera will be removed
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 f = translation *rotation;

	// Set model matrix as current OpenGL matrix:
	glLoadMatrixf(glm::value_ptr(f));
	BEnode::GetSuperRoot()->CalcTransformation(f);

	BEengine::lists_->RenderAll();

	//////
	// 2D:
	// Set orthographic projection:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(BEengine::GetInstance()->get_ortho()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));

	PrintTextInfo();

	// Swap this context's buffer:
	glutSwapBuffers();

	// Force rendering refresh:
	glutPostWindowRedisplay(BEengine::GetInstance()->get_window_id());

	// Inc. frames:
	frames++;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This callback is invoked each time the window gets resized (and once also when created).
* @param width new window width
* @param height new window height
*/
void reshapeCallback(int width, int height)
{
	// Update viewport size:
	glViewport(0, 0, width, height);

	// Update matrices:
	BEengine::GetInstance()->SetPerspective(glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f));
	BEengine::GetInstance()->SetOrtho(glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f));
}

void printGlmMat4(glm::mat4& gMat){
	std::cout << std::endl;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			std::cout << gMat[j][i] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void BEengine::SetKeyBoardCallBack(void(*callback)(unsigned char key, int mouseX, int mouseY))
{
	keyboard_callback_ = callback;
	glutKeyboardFunc(keyboard_callback_);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This callback is invoked each time a special keyboard key is pressed.
* @param key key pressed id
* @param mouseX mouse X coordinate
* @param mouseY mouse Y coordinate
*/
void specialCallback(int key, int mouseX, int mouseY)
{
	// Change box rotation:
	const float speed = BEengine::GetInstance()->GetDeltaPadding();
	switch (key)
	{
	case GLUT_KEY_UP:
		angleX -= speed;
		break;

	case GLUT_KEY_DOWN:
		angleX += speed;
		break;

	case GLUT_KEY_LEFT:
		angleY -= speed;
		break;

	case GLUT_KEY_RIGHT:
		angleY += speed;
		break;

	case GLUT_KEY_PAGE_DOWN:
		distance += BEengine::GetInstance()->GetDeltaZoom();
		break;

	case GLUT_KEY_PAGE_UP:
		distance -= BEengine::GetInstance()->GetDeltaZoom();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This callback is invoked each second.
* @param value passepartout value
*/
void timerCallback(int value)
{
	fps = frames / 1.0f;
	frames = 0;

	// Register the next update:
	glutTimerFunc(1000, timerCallback, 0);
}

//////////////////////////////////////////////////////////////////////////
void LIB_API BEengine::Init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[])
{
	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(x_position, y_position);
	glutInitWindowSize(width, heigth);

	// FreeGLUT can parse command-line params, in case:
	glutInit(&argc, argv);

	FreeImage_Initialise();


	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Create the window with a specific title:   
	window_id_ = glutCreateWindow(window_name);

	// Set callback functions:
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);

	//THIS MUST BE MOVED AWAY FROM HERE 
	//==============================================
	//the contained line is under arrest!
	//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	glutSpecialFunc(specialCallback);
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	glutTimerFunc(1000, timerCallback, 0);

	// Global OpenGL settings:
	glClearColor(1.0f, 0.6f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	BEengine::initialized_ = true;
}

//************************************
// Method:    Start
// FullName:  BEengine::Start
// Access:    public 
// Returns:   int -> EXIT_SUCCESS or EXIT_FAILURE
// this method starts the engine by loading the main loop
//************************************
int LIB_API BEengine::Start()
{
	if (!root){
		std::cout << std::endl << "[Start Error] There is no scene loaded!" << std::endl;
		return EXIT_FAILURE;
	}

	if (BEengine::initialized_)
	{
		node_selected_ = std::string("none or default");

		// Enter the main FreeGLUT processing loop:     

		glutMainLoop();

		//application exited
		return EXIT_SUCCESS;
	}
	else
	{
		std::cout << std::endl << "[Start Error] The engine hasn't been initialized! *Did you forget to call Init()?*" << std::endl;
	}

	//the engine wasn't initialized
	return EXIT_FAILURE;
}

/**
* Check if the file exists
*/
inline bool file_exists(const char* name)
{
	struct stat buffer;
	return (stat(name, &buffer) == 0);
}


LIB_API BEnode* BEengine::LoadScene(char *fileName)
{
	if (!file_exists(fileName))
	{
		std::cout << std::endl << "[Error 404] File \"" << fileName << "\" not found!" << std::endl;
		return nullptr;
	}

	BEsceneLoader scene_loader;
	root = scene_loader.LoadScene(fileName);

	return root;
}

//retrieves the window id of the current instance
int BEengine::get_window_id()
{
	return window_id_;
}

void LIB_API BEengine::SetPerspective(glm::mat4 perspective)
{
	perspective_ = perspective;
}

void LIB_API BEengine::SetOrtho(glm::mat4 ortho)
{
	ortho_ = ortho;
}

std::string BEengine::get_node_selected()
{
	return node_selected_;
}

void BEengine::set_node_selected(std::string name)
{
	node_selected_ = name;
}

// Return the index of the light
int AddLight(BElight *light){

	return -1; //TODO
}

// Return true on successful remove
bool RemoveLight(int indexLight){

	return false; //TODO
}

void PrintTextInfo()
{
	// Disable lighting before rendering 2D text:
	glDisable(GL_LIGHTING);

	float go_up = 10.0f;
	float delta = 20.0f;

	// Write some text:
	char text[64];

	snprintf(text, sizeof text, "FPS: %.1f", fps);
	glRasterPos2f(1.0f, go_up);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);

	snprintf(text, sizeof text, "Selected: %s", BEengine::GetInstance()->get_node_selected().c_str());
	glRasterPos2f(1.0f, go_up += delta);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);

	snprintf(text, sizeof text, "Speed Padding: %.2f", BEengine::GetInstance()->GetDeltaPadding());
	glRasterPos2f(1.0f, go_up += delta);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);


	// Redo ligting:
	glEnable(GL_LIGHTING);
}
