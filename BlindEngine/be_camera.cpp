#include "be_camera.h"


BEcamera::BEcamera(std::string name) :BEnode(name,CAMERA)
{
}


BEcamera::~BEcamera()
{
}

void BEcamera::Render(glm::mat4 f)
{
	std::cout << "Rendering a camera" << std::endl;
	for each (BEnode* n in BEnode::children_){
		n->Render(f*transformation_);
	}
}