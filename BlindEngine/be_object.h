#pragma once
#include <iostream>

class BEobject
{
public:
	virtual void Render()=0;

private:
	static long GenerateId();
	 long id_;
	 std::string name;
};

