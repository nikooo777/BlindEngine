#pragma once
// DLL
#include "be_dll_interface.h"
#include "be_node.h"

class LIB_API BEcamera : public BEnode
{
public:
	BEcamera(std::string);
	~BEcamera();
	virtual void Render(glm::mat4 world_matrix) override;
	virtual void CalcTransformation(glm::mat4 world_matrix) override;

	//Utility
	virtual BEnode* Find(std::string) override;
	virtual BEnode* Find(long) override;
};