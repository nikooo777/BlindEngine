#pragma once

// DLL
#include "be_dll_interface.h"


// OurClasses
#include "be_includes.h"
#include "be_light.h"
#include "be_list.h"
#include "be_scene_loader.h"
#include "be_node.h"

/************************************************************************/
// Engine class header
/************************************************************************/
class LIB_API BEengine
{
public:
	/// angles structure holding x and y components
	typedef struct Angles{
		float x;
		float y;
	}Angles;

	/// method used to retrieve the singleton instance of the class engine
	static BEengine* GetInstance();

	/// method used to initialize the context of the engine
	void Init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[]);

	/// method used to start the egine
	int Start();

	/// retrieves the window id of the current instance
	int get_window_id();

	/// allows the loading of a scene given a certain path
	BEnode* LoadScene(char *);

	/// allows retrieving the prespective 
	glm::mat4 get_perspective() const;
	glm::mat4 get_ortho() const { return ortho_; }
	void SetPerspective(glm::mat4 perspective);
	void SetOrtho(glm::mat4 ortho);
	void CalcTransformation();

	inline void SetDeltaPadding(float delta_padding){ delta_padding_ = delta_padding; }
	inline float GetDeltaPadding(){ return delta_padding_; }
	inline void SetDeltaZoom(float delta_zoom){ delta_zoom_ = delta_zoom; }
	void AddTimerCallBack(void(*timer_callback)(int value), int loop_time);
	void SetKeyboardCallBack(void(*callback)(unsigned char key, int mouseX, int mouseY));
	void SetSpecialCallBack(void(*callback)(int key, int mouseX, int mouseY));
	inline float GetDeltaZoom(){ return delta_zoom_; }
	static BElist* lists_;
	float* get_fps();
	float * get_distance();
	int * get_frames();
	Angles* get_angles();
	void PrintTextInfo();

	void AddText(std::string text){ text_.push_back(text); }

private:
	static BEengine* instance_;
	int window_id_;
	bool initialized_ = false;
	float delta_padding_ = 1.f;
	float delta_zoom_;
	// Init VAO:
	GLuint vao_;
	void(*keyboard_callback_)(unsigned char key, int mouseX, int mouseY);
	void(*special_callback_)(int key, int mouseX, int mouseY);
	// Matrices:
	glm::mat4 perspective_;
	glm::mat4 ortho_;
	float distance_;
	Angles* angles_;
	int frames_;
	float fps_;
	BEengine();
	~BEengine();

	std::vector<std::string> text_;
};
