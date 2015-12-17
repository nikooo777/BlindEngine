// FreeGLUT  
#include <GL/freeglut.h>
#pragma once
/*
	SINGLETON class
*/
class BEengine
{
public:
	static BEengine* getInstance();
	void init(char* window_name, int x_position, int y_position, int width, int heigth, void(*keyCallback)(int, int, int), int argc, char *argv[]);
	int start();

private:
	static BEengine* __instance;
	int window_id;
	bool initialized = false;
	BEengine();
	~BEengine();
};

