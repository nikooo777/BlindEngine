#pragma once
#include "be_node.h"
class BEcamera : public BEnode
{
public:
	BEcamera(std::string);
	~BEcamera();
	virtual void Render(glm::mat4) override;
	virtual void RenderSingle(glm::mat4 cumulated_transformation_matrix) override;
	virtual void CalcTransformation(glm::mat4 cumulated_transformation_matrix) override;
};