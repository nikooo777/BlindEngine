#include "be_engine.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////
// DLL MAIN //
//////////////
#ifdef WIN32
#include <Windows.h>
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
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////
BEnode *root;

// Texture:
//unsigned char *bitmap = NULL;
//unsigned int texId = 0;
//float tiling = 1.0f;
//bool forceNewTexture = true;

// Auto/manual flag:
//bool automatic = false;

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
	angles_ = new Angles{ 15.f, 0.f };
	#ifdef WIN32
    distance_ = -15.f;
    delta_zoom_ = 1.f;
    #else
    distance_ = -500.f;
    delta_zoom_ = 50.f;
    #endif // WIN32
	lists_ = new BElist();
	fps_ = 0;
	frames_ = 0;
}

BEengine::~BEengine()
{
	delete instance_;
	delete lists_;
	delete angles_;
}

void LIB_API BEengine::CalcTransformation()
{
	// Set a matrix to move our triangle:
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, distance_));

	//the following 2 lines will be obsolete when the special callback and/or the camera will be removed
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(angles_->x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(angles_->y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 f = translation *rotation;

	BEnode::GetSuperRoot()->CalcTransformation(f);
	BEengine::lists_->DeepSort();
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

	BEengine::lists_->RenderAll();

	//////
	// 2D:
	// Set orthographic projection:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(BEengine::GetInstance()->get_ortho()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));

	BEengine::GetInstance()->PrintTextInfo();

	// Swap this context's buffer:
	glutSwapBuffers();

	// Inc. frames:
	(*BEengine::GetInstance()->get_frames())++;

	// Force rendering refresh:
	glutPostWindowRedisplay(BEengine::GetInstance()->get_window_id());
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
	BEengine::GetInstance()->SetPerspective(glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 3000.0f));
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


void BEengine::AddTimerCallBack(void(*timer_callback)(int value), int loop_time)
{
	glutTimerFunc(loop_time, timer_callback, 0);
}


void BEengine::SetKeyboardCallBack(void(*callback)(unsigned char key, int mouseX, int mouseY))
{
	keyboard_callback_ = callback;
	glutKeyboardFunc(keyboard_callback_);
}

void BEengine::SetSpecialCallBack(void(*callback)(int key, int mouseX, int mouseY))
{
	special_callback_ = callback;
	glutSpecialFunc(special_callback_);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* This callback is invoked each second.
* @param value passepartout value
*/
void timerCallback(int value)
{
	(*BEengine::GetInstance()->get_fps()) = *BEengine::GetInstance()->get_frames() / 1.0f;
	(*BEengine::GetInstance()->get_frames()) = 0;

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
	glutTimerFunc(1000, timerCallback, 0);

	// Global OpenGL settings:
	//glClearColor(1.0f, 0.6f, 0.1f, 1.0f);
	glClearColor(.2f, .2f, .2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
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
		BEengine::GetInstance()->CalcTransformation();

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
	CalcTransformation();

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

//rendering distance pointer (modifiable)
float * BEengine::get_distance()
{
	return &distance_;
}

//getter method for the angles used to render the scene
//angles have x and y fields
BEengine::Angles* BEengine::get_angles()
{
	return angles_;
}

//FPS count pointer (modifiable)
float* BEengine::get_fps()
{
	return &fps_;
}

//frames count pointer (modifiable)
int* BEengine::get_frames()
{
	return &frames_;
}

void BEengine::PrintTextInfo()
{
	// Disable lighting before rendering 2D text:
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	float go_up = 10.0f;
	static float delta = 20.0f;

	// Write some text:
	char text[64];

	snprintf(text, sizeof text, "FPS: %.1f", fps_);
	glRasterPos2f(1.0f, go_up);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);

	snprintf(text, sizeof text, "Selected: %s", node_selected_.c_str());
	glRasterPos2f(1.0f, go_up += delta);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);

	snprintf(text, sizeof text, "Speed Padding: %.2f", delta_padding_);
	glRasterPos2f(1.0f, go_up += delta);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);


	// Redo ligting:
	glEnable(GL_LIGHTING);
}

glm::mat4 BEengine::get_perspective() const
{
	return perspective_;
}
