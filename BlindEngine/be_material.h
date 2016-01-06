#pragma once
#include "be_object.h"
#include "be_texture.h"


class BEmaterial : public BEobject
{
public:
	BEmaterial(std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess, BEtexture* texture);
	~BEmaterial();

	void Render(glm::mat4) override;
	glm::vec4 ambient_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	float shininess_;
	BEtexture* texture_;
};