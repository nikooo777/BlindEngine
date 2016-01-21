#pragma once
// DLL
#include "be_dll_interface.h"

#include "be_object.h"
#include "be_texture.h"


class LIB_API BEmaterial : public BEobject
{
public:
	BEmaterial(std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float transparency, float shininess, float shininess_strength, BEtexture* texture);
	~BEmaterial();

	//virtual void Render(glm::mat4) override;
	virtual void Render(glm::mat4 world_matrix) override;
	bool IsTransparent();
	void SetTransparency(float alpha);
	glm::vec4 ambient_;
	glm::vec4 diffuse_;
	glm::vec4 specular_;
	float shininess_;
	float shininess_strength_;
	BEtexture* texture_;

	static BEmaterial* mat_shadow;
private:
	float transparency_;
};

bool FloatCompare(float f1, float f2, float precision = 0.0000001f);