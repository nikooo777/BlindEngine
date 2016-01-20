#pragma once
// DLL
#include "be_dll_interface.h"

#include "be_includes.h"

class LIB_API BEobject
{
public:
	//virtual void Render(glm::mat4 world_matrix) = 0;
	virtual void Render(glm::mat4 world_matrix) = 0;
	virtual ~BEobject();
	BEobject();
	BEobject(const std::string);
	std::string get_name(){ return name_; }
	void set_name(std::string name){ name_ = name; }
	long get_id(){ return id_; }
	void PrintName(){ std::cout << get_name() << std::endl; }
private:
	virtual void GenerateId();
	long id_;
	std::string name_;
};
