#pragma once
// DLL
#include "be_dll_interface.h"

#include "be_object.h"
#include "be_texture.h"


class LIB_API BEmaterial : public BEobject
{
public:
	BEmaterial(std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess, float shininess_strength,  BEtexture* texture);
	~BEmaterial();

	virtual void Render(glm::mat4) override;
	virtual void RenderSingle(glm::mat4 cumulated_transformation_matrix) override;
	glm::vec4 ambient_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	float shininess_;
	float shininess_strength_;
	BEtexture* texture_;
};