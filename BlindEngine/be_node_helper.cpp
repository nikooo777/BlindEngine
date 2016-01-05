#include "be_node_helper.h"


BEnodeHelper::BEnodeHelper()
{
}


BEnodeHelper::~BEnodeHelper()
{
}

void BEnodeHelper::Render()
{
	std::cout << std::endl << "Rendering a node helper ";
	BEnode::PrintName();
	std::cout << "Children count: " << BEnode::GetChildren().size() << std::endl;

	for each (BEnode* n in BEnode::children_){
		n->Render();
	}
}