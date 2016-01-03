#pragma once
#include "be_includes.h"

class BEobject
{
public:
	virtual void Render()=0;
	virtual ~BEobject();
	BEobject();
	BEobject(const std::string);


private:
	virtual void GenerateId();
	 long id_;
	 std::string name_;
};

