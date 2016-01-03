#pragma once
#include "be_node.h"
#include <vector>
#include "be_material.h"
#include <glm/glm.hpp>

class BEmesh : public BEnode
{
public:
	BEmesh();
	BEmesh(glm::vec3*, glm::vec3*, glm::vec2*, BEmaterial*, std::string);
	BEmesh(glm::vec3*, glm::vec3*, glm::vec2*, BEmaterial*);
	~BEmesh();

private:
	glm::vec3* vertices_;
	glm::vec3* normals_;
	glm::vec2* texture_coords;

	BEmaterial* material_;
};