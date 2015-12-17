#include <iostream>
#include "BEengine.h"

int main(int argc, char *argv[])
{
	std::cout << "Welcome to Blind Engine 1.0" << std::endl;
	BEengine *engine = BEengine::getInstance();
	engine->init("First Test", 500, 500, 600, 400, nullptr, argc, argv);
	engine->start();
	
	return EXIT_SUCCESS;
}