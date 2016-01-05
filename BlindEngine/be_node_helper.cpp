#include "be_node_helper.h"


BEnodeHelper::BEnodeHelper()
{
}


BEnodeHelper::~BEnodeHelper()
{
}

void BEnodeHelper::Render()
{
	std::cout << std::endl << "Rendering a node ";
	BEnode::PrintName();
	for each (BEnode* n in BEnode::children_){
		n->Render();
	}
}

