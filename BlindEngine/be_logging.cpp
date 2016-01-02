#pragma once
#include "be_logging.h"


void be_logging::log(char *msg)
{
	std::cout << msg << std::endl;
}


void be_logging::log(std::string msg)
{
	std::cout << msg.c_str() << std::endl;
}