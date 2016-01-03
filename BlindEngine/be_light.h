#pragma once

#include "be_node.h"
#include "glm\glm.hpp"

class BELight : public BEnode
{
public:
	//constructor for directional light -> omnidirectional = false
	//constructor for omnidirectional light -> omnidirectional = false
	BELight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 ,bool omnidirectional=false);

	//constructor for spotlight light
	BELight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 position, float cutoff);
	~BELight();

	//inherited render method
	virtual void Render();
private:
	//light index
	int light_number_;
	// The light properties
	glm::vec4 ambient_, diffuse_, specular_, position_, direction_;
};