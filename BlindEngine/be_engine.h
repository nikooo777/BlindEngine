#pragma once
// FreeGLUT  
#include <GL/freeglut.h>

// OurClasses
#include "be_Light.h"

/*
	SINGLETON class
*/
class BEengine
{
public:
	static BEengine* getInstance();
	void init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[]);
	int start();

	int add_light(BELight *);
	bool remove_light(int indexLight);

private:
	static BEengine* instance_;
	int window_id_;
	bool initialized_ = false;
	BEengine();
	~BEengine();
};

