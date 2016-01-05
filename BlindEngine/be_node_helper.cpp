#include "be_node_helper.h"


BEnodeHelper::BEnodeHelper(std::string name) : BEnode(name)
{
}


BEnodeHelper::~BEnodeHelper()
{
}

void BEnodeHelper::Render()
{
	std::cout << std::endl << "Rendering node helper: " << get_name() << std::endl;
	std::cout << "Children count: " << BEnode::GetChildren().size() << std::endl;

	for each (BEnode* n in BEnode::children_){
		n->Render();
	}
}