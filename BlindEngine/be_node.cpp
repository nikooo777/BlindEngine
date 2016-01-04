#include "be_node.h"

BEnode * BEnode::root_node = nullptr;

BEnode::BEnode()
{
	Init();
}

BEnode::BEnode(const std::string name) : BEobject(name)
{
	Init();
}

void BEnode::Init()
{
	if (root_node == nullptr)
		root_node = new BEnode("root_node");
	parent_ = root_node;
}

BEnode::~BEnode()
{

}

void BEnode::AddChild(BEnode* node)
{
	node->ChangeParent(this);
	children_.push_back(node);
}

/** changes the parent of the current node
* __________if the current node has a parent already, it calls removechild to de-register__________
* This method is NOT responsible for calling addchild, therefore it's the user's
* responsibility to call such function on the parent.
* Forgetting such action will leave a semi-orphaned child hanging around
*/
void BEnode::ChangeParent(BEnode* parent)
{
	//if (parent_ != nullptr)
	//{
	//	parent_->RemoveChild(this);
	//}
	parent_ = parent;
}


void BEnode::RemoveChild(BEnode * node, bool should_delete)
{
	//for each children of the removed node, set the parent to the root node
	for (auto&child : node->GetChildren())
	{
		child->ChangeParent(BEnode::GetRoot());
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

std::vector<BEnode*> BEnode::GetChildren()
{
	return children_;
}

//static getter for the root node. There is only one root node
//the root node is instantiated automatically
BEnode* BEnode::GetRoot()
{
	return root_node;
}

void BEnode::Render()
{
	throw new std::runtime_error("Rendering is unimplemented for element Node");
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