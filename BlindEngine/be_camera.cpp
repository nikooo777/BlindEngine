#include "be_camera.h"


BEcamera::BEcamera()
{
}


BEcamera::~BEcamera()
{
}

void BEcamera::Render()
{
	for each (BEnode* n in BEnode::children_){
		n->Render();
	}
}