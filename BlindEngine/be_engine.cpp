#include "be_engine.h"
#include "be_includes.h"
#include "be_scene_loader.h"

/////////////
// GLOBALS //
/////////////
///////////// 
BEnode *root;

// Rotation angles:
float angleX = 0.0f, angleY = 0.0f;
float distance = -45.0f;
float globalX = .0f;
float globalY = .0f;

// Matrices:

// Texture:
unsigned char *bitmap = NULL;
unsigned int texId = 0;
float tiling = 1.0f;
bool forceNewTexture = true;

// Auto/manual flag:
bool automatic = true;

// Show/hide normals:
bool showNormals = false;

// Mipmaps?
bool useMipmaps = false;

// Anisotrophic?
bool isAnisotropicSupported = false;
bool anisotropic = false;
int anisotropicLevel = 1;

// Frame counter:
int frames = 0;
float fps = 0.0f;

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
	glm::mat4 rotation = glm::rotate(glm::mat4(), glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 f = translation * rotation;

	// Set model matrix as current OpenGL matrix:
	glLoadMatrixf(glm::value_ptr(f));

	root->Render();

	//////
	// 2D:
	// Set orthographic projection:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(BEengine::GetInstance()->get_ortho()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));

	// Disable lighting before rendering 2D text:
	glDisable(GL_LIGHTING);

	// Write some text:
	char text[64];
	sprintf_s(text,sizeof text, "FPS: %.1f", fps);
	glRasterPos2f(1.0f, 10.0f);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);

	// Redo ligting:
	glEnable(GL_LIGHTING);

	// Swap this context's buffer:
	glutSwapBuffers();

	// Update coords:
	if (automatic)
	{
		angleX += 0.1f;
		angleY += 0.2f;

		// Force rendering refresh:
		glutPostWindowRedisplay(BEengine::GetInstance()->get_window_id());
	}

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
	std::cout << "[reshape func invoked]" << std::endl;

	// Update viewport size:
	glViewport(0, 0, width, height);

	// Update matrices:
	BEengine::GetInstance()->SetPerspective(glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f));
	BEengine::GetInstance()->SetOrtho(glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f));
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
	std::cout << "[std key pressed]" << std::endl;

	switch (key)
	{
	case ' ':
		automatic = !automatic;
		break;
	case 'a':
		globalX -= 2.5f;
		break;

	case 'd':
		globalX += 2.5f;
		break;

	case 'w':
		globalY += 2.5f;
		break;

	case 's':
		globalY -= 2.5f;
		break;
	}

	// Force rendering refresh:
	glutPostWindowRedisplay(BEengine::GetInstance()->get_window_id());
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
	std::cout << "[key pressed]" << std::endl;

	// Automatic-mode, not needed:
	if (automatic)
		return;

	// Change box rotation:
	const float speed = 2.5f;
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
		distance += 5.0f;
		break;

	case GLUT_KEY_PAGE_UP:
		distance -= 5.0f;
		break;
	}

	// Force rendering refresh:
	glutPostWindowRedisplay(BEengine::GetInstance()->get_window_id());
}

//////////////////////////////////////////////////////////////////////////
void BEengine::Init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[])
{
	// Init context:
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(x_position, y_position);
	glutInitWindowSize(width, heigth);

	// FreeGLUT can parse command-line params, in case:
	glutInit(&argc, argv);

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Create the window with a specific title:   
	window_id_ = glutCreateWindow(window_name);

	// Set callback functions:
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutKeyboardFunc(keyboardCallback);
	glutSpecialFunc(specialCallback);

	// Global OpenGL settings:
	glClearColor(1.0f, 0.6f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	BEengine::initialized_ = true;
}

//************************************
// Method:    Start
// FullName:  BEengine::Start
// Access:    public 
// Returns:   int -> EXIT_SUCCESS or EXIT_FAILURE
// this method starts the engine by loading the main loop
//************************************
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

void BEengine::LoadScene(char *)
{
	BEsceneLoader scene_loader;
	root = scene_loader.LoadScene("cornelbox.DAE");
}

//retrieves the window id of the current instance
int BEengine::get_window_id()
{
	return window_id_;
}

void BEengine::SetPerspective(glm::mat4 perspective)
{
	perspective_ = perspective;
}

void BEengine::SetOrtho(glm::mat4 ortho)
{
	ortho_ = ortho;
}

// Return the index of the light
int AddLight(BElight *light){

	return -1; //TODO
}

// Return true on successful remove
bool RemoveLight(int indexLight){

	return false; //TODO
}