#include "be_camera.h"


BEcamera::BEcamera(std::string name) :BEnode(name,CAMERA)
{
}


BEcamera::~BEcamera()
{
}

void BEcamera::Render(glm::mat4 cumulated_transformation_matrix)
{
	std::cout << "Rendering a camera" << std::endl;
	for(BEnode* n : BEnode::children_){
		n->Render(cumulated_transformation_matrix*transformation_);
	}
}

void BEcamera::RenderSingle(glm::mat4 cumulated_transformation_matrix)
{
	std::cout << "Rendering a camera" << std::endl;
}

void BEcamera::CalcTransformation(glm::mat4 cumulated_transformation_matrix)
{
	glm::mat4 tmpF = cumulated_transformation_matrix*transformation_;

	for(BEnode* n : BEnode::children_){
		n->CalcTransformation(tmpF);
	}
}



BEnode* BEcamera::Find(std::string name)
{
	//the current node is the one sought
	if (this->get_name().compare(name) == 0)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for(BEnode* n : children_)
	{
		if ((found_node = n->Find(name)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}

BEnode* BEcamera::Find(long id)
{
	//the current node is the one sought
	if (this->get_id() == id)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for(BEnode* n : children_)
	{
		if ((found_node = n->Find(id)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}
