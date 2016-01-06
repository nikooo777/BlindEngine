#include "be_node.h"

BEnode* BEnode::super_root_node = new BEnode("root_node",ROOT);

BEnode::BEnode(std::string name, Type type) : BEobject(name)
{
	type_ = type;
}


BEnode::~BEnode()
{

}

void BEnode::AddChild(BEnode* node)
{
	node->SetParent(this);
	children_.push_back(node);
}


void BEnode::RemoveChild(BEnode * node, bool should_delete)
{
	//for each children of the removed node, set the parent to the root node
	for (auto&child : node->GetChildren())
	{
		child->SetParent(BEnode::GetSuperRoot());
	}

	//remove the node from the vector containing all children
	for (std::vector<BEnode*>::iterator iter = children_.begin(); iter != children_.end(); iter++)
	{
		if (*iter == node)
		{
			iter = children_.erase(iter);
		}
	}
	if (should_delete)
	{
		delete node;
	}

}

BEnode* BEnode::find(std::string name)
{
	//the current node is the one sought
	if (this->get_name().compare(name) == 0)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for each (BEnode* n in children_)
	{
		if ((found_node = n->find(name)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}

BEnode* BEnode::find(long id)
{
	//the current node is the one sought
	if (this->get_id() == id)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for each (BEnode* n in children_)
	{
		if ((found_node = n->find(id)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}

void BEnode::Render(glm::mat4& f)
{
	glLoadMatrixf(glm::value_ptr(f));
	std::cout << "Wrong place" << std::endl;
	for each (BEnode* n in BEnode::children_){
		n->Render(f*transformation_);
	}
}
