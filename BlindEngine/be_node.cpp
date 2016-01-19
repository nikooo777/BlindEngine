#include "be_node.h"

BEnode* BEnode::super_root_node = new BEnode("root_node", ROOT);

BEnode::BEnode(std::string name, Type type) : BEobject(name)
{
	type_ = type;
	transformation_ = glm::mat4(1);
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
	std::vector<BEnode*>::iterator iter = children_.begin();
	while (iter != children_.end())
	{
		if (*iter == node)
			iter = children_.erase(iter);
		else
			iter++;
	}
	if (should_delete)
	{
		delete node;
	}

}

BEnode* BEnode::Find(std::string name)
{
	//the current node is the one sought
	if (this->get_name().compare(name) == 0)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for (BEnode* n : children_)
	{
		if ((found_node = n->Find(name)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}

BEnode* BEnode::Find(long id)
{
	//the current node is the one sought
	if (this->get_id() == id)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for (BEnode* n : children_)
	{
		if ((found_node = n->Find(id)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}



void BEnode::Render(glm::mat4 cumulated_transformation_matrix)
{
	/*glm::mat4 tmpF = cumulated_transformation_matrix*transformation_;
	glLoadMatrixf(glm::value_ptr(tmpF));

	std::cout << "Wrong place" << std::endl;
	for (BEnode* n : BEnode::children_){
	n->Render(tmpF);
	}*/
}

void BEnode::RenderSingle(glm::mat4 cumulated_transformation_matrix)
{
	std::cout << "RenderNode: Should not be here" << std::endl;
}

void BEnode::CalcTransformation(glm::mat4 cumulated_transformation_matrix)
{
	glm::mat4 tmpF = cumulated_transformation_matrix*transformation_;
	for (BEnode* n : BEnode::children_){
		n->CalcTransformation(tmpF);
	}
}

void BEnode::SetAsSceneRoot()
{
	BEnode::GetSuperRoot()->AddChild(this);
}

BEnode* BEnode::GetSceneRootByName(std::string node_name)
{
	BEnode* super_root = BEnode::GetSuperRoot();

	//seek the node in the children
	for (BEnode* n : super_root->children_)
	{
		std::cout << "|" << n->get_name() << "|" << std::endl;
		if (!(n->get_name().compare(node_name)))
			return n;
	}

	return nullptr;
}

LIB_API BEnode* BEnode::GetSuperRoot()
{
	return super_root_node;
}

void BEnode::SetParent(BEnode *parent)
{
	parent_ = parent;
}

void BEnode::UpdateTransformationRecursive(glm::mat4 transformation)
{
	transformation_ = transformation*transformation_;

	for (BEnode* n : children_)
		n->UpdateTransformationRecursive(transformation);
}
