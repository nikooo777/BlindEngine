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
	for each (BEnode* n in BEnode::children_){
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

	for each (BEnode* n in BEnode::children_){
		n->CalcTransformation(tmpF);
	}
}