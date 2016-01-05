#include "be_camera.h"


BEcamera::BEcamera(std::string name) :BEnode(name,CAMERA)
{
}


BEcamera::~BEcamera()
{
}

void BEcamera::Render()
{
	std::cout << "Rendering a camera" << std::endl;
	for each (BEnode* n in BEnode::children_){
		n->Render();
	}
}