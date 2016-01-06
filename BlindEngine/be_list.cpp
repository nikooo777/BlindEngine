#include "be_list.h"


BElist::BElist()
{
}


BElist::~BElist()
{
}

void BElist::RenderAll()
{
	RenderMeshes();
	RenderLights();
	RenderCameras();
}
void BElist::RenderMeshes()
{
	for (const auto& pair : meshes_)
	{
		pair.first->Render(glm::mat4(1));
	}
}
void BElist::RenderLights()
{
	for (const auto& pair : lights_)
	{
		pair.first->Render(glm::mat4(1));
	}
}
void BElist::RenderCameras()
{
	for (const auto& pair : cameras_)
	{
		pair.first->Render(glm::mat4(1));
	}
}

void BElist::AddMaterial(BEmaterial* material)
{
	materials_.push_back(material);
}

BEmaterial* BElist::GetMaterial(unsigned int index)
{
	return materials_[index];
}
