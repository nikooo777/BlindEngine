#pragma once

#include "be_node.h"
#include "glm\glm.hpp"

class BELight : public BEnode
{
public:
	BELight();
	~BELight();
private:

	int light_number_;
	// The light properties
	glm::vec4 ambient_, diffuse_, specular_, shininess_, light_position_, direction_;
};