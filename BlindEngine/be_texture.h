#pragma once
#include "be_object.h"
class BEtexture : public BEobject
{
public:
	BEtexture();
	~BEtexture();
	void Render(glm::mat4) override;
private:
};