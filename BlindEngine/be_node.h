#pragma once
#include "be_object.h"
#include <glm\glm.hpp>
#include <vector>

class BEnode : public BEobject
{
public:
	BEnode();
	BEnode(const std::string);
	virtual ~BEnode();
	virtual void Render()=0;
private:
	glm::mat4 position_;

	BEnode* parent_;
	std::vector<BEnode*> children_;
};

