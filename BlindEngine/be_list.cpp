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
	/*
	for (const auto& pair : meshes_)
	{
		pair.first->Render(glm::mat4(1));
	}
	}
	}*/
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


void BElist::AddMesh(BEmesh*mesh)
{
	std::cout << "Try to add -> " << mesh->get_name() << std::endl;

	mesh_ordered_list_.push_back(mesh);
	meshes_.insert(std::pair<BEmesh*, glm::mat4>(mesh, glm::mat4(1)));

	if (meshes_by_name_.find(mesh->get_name()) == meshes_by_name_.end())
		meshes_by_name_.insert(std::pair<std::string, BEmesh*>(mesh->get_name(), mesh));
	else
		std::cout << mesh->get_name() << " already exists!" << std::endl;
}

BEmesh* BElist::GetMeshByName(std::string name)
{
	auto search = meshes_by_name_.find(name);
	if (search != meshes_by_name_.end())
		return search->second;
	else
		return nullptr;
}


