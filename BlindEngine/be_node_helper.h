#pragma once
#include "be_node.h"
class BEnodeHelper : public BEnode
{
public:
	BEnodeHelper();
	BEnodeHelper(std::string name);
	~BEnodeHelper();

	void Render() override;
};