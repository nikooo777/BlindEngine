#pragma once
#include "be_node.h"
class BEcamera : public BEnode
{
public:
	BEcamera(std::string);
	~BEcamera();
	virtual void Render(glm::mat4, bool is_sub_mesh = false) override;
};