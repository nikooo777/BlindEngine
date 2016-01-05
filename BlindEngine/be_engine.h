#pragma once
// FreeGLUT  
#include <GL/freeglut.h>

// OurClasses
#include "be_light.h"

/*
SINGLETON class
*/
class BEengine
{
public:
	static BEengine* GetInstance();
	void Init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[]);
	int Start();

	int AddLight(BElight *);
	bool RemoveLigh(int indexLight);
	int get_window_id();
	void LoadScene(char *);

private:
	static BEengine* instance_;
	int window_id_;
	bool initialized_ = false;

	// Matrices:
	glm::mat4 perspective_;
	glm::mat4 ortho_;

	BEengine();
	~BEengine();
};