#pragma once
#include "be_includes.h"

class BEobject
{
public:
	virtual void Render(glm::mat4) = 0;
	virtual ~BEobject();
	BEobject();
	BEobject(const std::string);
	std::string get_name();
	long get_id();
	void PrintName(){ std::cout << get_name() << std::endl; }
private:
	virtual void GenerateId();
	long id_;
	std::string name_;
};