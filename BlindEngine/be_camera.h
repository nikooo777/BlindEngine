#pragma once
#include "be_node.h"
class BEcamera : public BEnode
{
public:
	BEcamera();
	~BEcamera();
	virtual void Render() override;
};
