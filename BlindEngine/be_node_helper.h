#pragma once
#include "be_node.h"
class BEnodeHelper : public BEnode
{
public:
	BEnodeHelper();
	BEnodeHelper(std::string name){ BEnode::name_ = name; }
	~BEnodeHelper();

	virtual void Render() override;
};

