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
// SHADERS //
/////////////

//////////////////////////////////////////
char *vertShader = R"(
   #version 440 core

   // Uniforms:
   uniform mat4 projection;
   uniform mat4 modelview;
   uniform mat3 normalMatrix;

   // Attributes:
   layout(location = 0) in vec3 in_Position;
   layout(location = 1) in vec3 in_Normal;

   // Varying:
   out vec4 fragPosition;
   out vec3 normal;   

   void main(void)
   {
      fragPosition = modelview * vec4(in_Position, 1.0);
      gl_Position = projection * fragPosition;      
      normal = normalMatrix * in_Normal;
   }
)";

//////////////////////////////////////////
char *fragShader = R"(
   #version 440 core

   in vec4 fragPosition;
   in vec3 normal;   
   
   out vec4 fragOutput;

   // Material properties:
   uniform vec3 matAmbient;
   uniform vec3 matDiffuse;
   uniform vec3 matSpecular;
   uniform float matShininess;

   // Light properties:
   uniform vec3 lightPosition; 
   uniform vec3 lightAmbient; 
   uniform vec3 lightDiffuse; 
   uniform vec3 lightSpecular;

   void main(void)
   {      
      // Ambient term:
      vec3 fragColor = matAmbient * lightAmbient;

      // Diffuse term:
      vec3 _normal = normalize(normal);
      vec3 lightDirection = normalize(lightPosition - fragPosition.xyz);      
      float nDotL = dot(lightDirection, _normal);   
      if (nDotL > 0.0)
      {
         fragColor += matDiffuse * nDotL * lightDiffuse;
      
         // Specular term:
         vec3 halfVector = normalize(lightDirection + normalize(-fragPosition.xyz));                     
         float nDotHV = dot(_normal, halfVector);         
         fragColor += matSpecular * pow(nDotHV, matShininess) * lightSpecular;
      } 
      
      // Final color:
      fragOutput = vec4(fragColor, 1.0);
   }
)";

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
	//OLD_gl //glMatrixMode(GL_PROJECTION);
	//OLD_gl //glLoadMatrixf(glm::value_ptr(BEengine::GetInstance()->get_perspective()));
	//OLD_gl //glMatrixMode(GL_MODELVIEW);
	BEshader* shader = BEengine::GetInstance()->get_shader();
	shader->setMatrix(shader->projLoc, BEengine::GetInstance()->get_perspective());

	BEengine::lists_->RenderAll();

	//////
	// 2D:
	// Set orthographic projection:
	//OLD_gl //glMatrixMode(GL_PROJECTION);
	//OLD_gl //glLoadMatrixf(glm::value_ptr(BEengine::GetInstance()->get_ortho()));
	//OLD_gl //glMatrixMode(GL_MODELVIEW);
	//OLD_gl //glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));
	shader->setMatrix(shader->projLoc, BEengine::GetInstance()->get_ortho());
	shader->setMatrix(shader->mvLoc, glm::mat4());

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

/**
* Debug message callback for OpenGL. See https://www.opengl.org/wiki/Debug_Output
*/
void __stdcall DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
{
	std::cout << "OpenGL says: \"" << std::string(message) << "\"" << std::endl;
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
	glutInitContextVersion(4, 4);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG); // <-- Debug flag required by the OpenGL debug callback


	FreeImage_Initialise();

	// Set some optional flags:
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Create the window with a specific title:
	window_id_ = glutCreateWindow(window_name); //OpenGL context is initialized at this point

	//glew initialization
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();

	if (GLEW_OK != error)
	{
		std::cout << "Error: " << glewGetErrorString(error) << std::endl;
		return;
	}
	else if (GLEW_VERSION_4_4)
	{
		std::cout << "Driver supports OpenGL 4.4\n" << std::endl;
	}
	else
	{
		std::cout << "Error: OpenGL 4.4 not supported\n" << std::endl;
		return;
	}

	// Register OpenGL debug callback:
	glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	int oglVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &oglVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &oglVersion[1]);
	std::cout << "   Version  . . :  " << glGetString(GL_VERSION) << " [" << oglVersion[0] << "." << oglVersion[1] << "]" << std::endl;
	std::cout << "   GLSL . . . . :  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << std::endl;


	// Set callback functions:
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutTimerFunc(1000, timerCallback, 0);

	// Init VAO:
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	// Global OpenGL settings:
	//glClearColor(1.0f, 0.6f, 0.1f, 1.0f);
	glClearColor(.2f, .2f, .2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	//OLD_gl //glEnable(GL_LIGHTING);
	//OLD_gl //glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	

	// Compile shaders:
	BEshader vertex_shader;
	vertex_shader.loadFromMemory(BEshader::TYPE_VERTEX, vertShader);

	BEshader pixel_shader;
	pixel_shader.loadFromMemory(BEshader::TYPE_FRAGMENT, fragShader);

	shader_.build(&vertex_shader, &pixel_shader);
	shader_.Render(glm::mat4());
	shader_.initParam();

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
	//OLD_gl //glDisable(GL_LIGHTING);
	//OLD_gl //glDisable(GL_TEXTURE_2D);


	static float delta = 20.0f;
	float go_up = 10.0f - delta / 2;

	// Write some text:
	char text[255];

	for (auto tex : text_)
	{
		snprintf(text, sizeof text, "%s", tex.c_str());
		//OLD_gl//glRasterPos2f(1.0f, go_up += delta);
		//OLD_gl//glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);
	}

	snprintf(text, sizeof text, "Speed Padding: %.2f", delta_padding_);
	//OLD_gl//glRasterPos2f(1.0f, go_up += delta);
	//OLD_gl//glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);

	snprintf(text, sizeof text, "FPS: %.1f", fps_);
	//OLD_gl//glRasterPos2f(1.0f, go_up += delta);
	//OLD_gl//glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *)text);



	// Redo ligting:
	//OLD_gl //glEnable(GL_LIGHTING);
}

glm::mat4 BEengine::get_perspective() const
{
	return perspective_;
}
