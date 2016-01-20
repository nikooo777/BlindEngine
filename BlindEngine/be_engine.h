#pragma once

// DLL
#include "be_dll_interface.h"


// OurClasses
#include "be_includes.h"
#include "be_light.h"
#include "be_list.h"
#include "be_scene_loader.h"

/*
SINGLETON class
*/
class LIB_API BEengine
{
public:
	static BEengine* GetInstance();
	void Init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[]);
	int Start();

	int AddLight(BElight *);
	bool RemoveLigh(int indexLight);
	int get_window_id();
	BEnode* LoadScene(char *);
	glm::mat4 get_perspective() const { return perspective_; }
	glm::mat4 get_ortho() const { return ortho_; }
	void SetPerspective(glm::mat4 perspective);
	void SetOrtho(glm::mat4 ortho);

	inline void SetDeltaPadding(float delta_padding){ delta_padding_ = delta_padding; }
	inline float GetDeltaPadding(){ return delta_padding_; }
	inline void SetDeltaZoom(float delta_zoom){ delta_zoom_ = delta_zoom; }
	void AddTimerCallBack(void(*timer_callback)(int value), int loop_time);
	void SetKeyBoardCallBack(void(*callback)(unsigned char key, int mouseX, int mouseY));
	inline float GetDeltaZoom(){ return delta_zoom_; }
	std::string get_node_selected();
	void set_node_selected(std::string name);
	static BElist* lists_;
private:
	std::string node_selected_;
	static BEengine* instance_;
	int window_id_;
	bool initialized_ = false;
	float delta_padding_ = 1.f, delta_zoom_ = 1.f;
	void(*keyboard_callback_)(unsigned char key, int mouseX, int mouseY);
	// Matrices:
	glm::mat4 perspective_;
	glm::mat4 ortho_;
	BEengine();
	~BEengine();
};