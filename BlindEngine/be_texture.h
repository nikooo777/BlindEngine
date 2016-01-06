#pragma once
#include "be_object.h"
class BEtexture : public BEobject
{
public:
	BEtexture(std::string);
	~BEtexture();
	void Render(glm::mat4, bool is_sub_mesh = false) override;
private:
	std::string name_;
};