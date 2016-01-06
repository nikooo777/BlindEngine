#pragma once
#include "be_object.h"
class BEmaterial : public BEobject
{
public:
	BEmaterial();
	~BEmaterial();

	void Render(glm::mat4) override;
};