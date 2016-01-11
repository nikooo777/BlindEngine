#include "be_list.h"


BElist::BElist()
{
}


BElist::~BElist()
{
}

/************************************************************************/
/* Render
/************************************************************************/
void BElist::RenderAll()
{
	RenderLights();
	RenderMeshes();
	RenderCameras();
}

void BElist::RenderMeshes()
{
	for (const auto& pair : meshes_)
	{
		pair.first->RenderSingle(pair.second);
	}
}
void BElist::RenderLights()
{
	for (const auto& pair : lights_)
	{
		pair.first->RenderSingle(pair.second);
	}
}
void BElist::RenderCameras()
{
	for (const auto& pair : cameras_)
	{
		pair.first->RenderSingle(pair.second);
	}
}


/************************************************************************/
/* Mesh
/************************************************************************/
void BElist::AddMesh(BEmesh*mesh)
{
	std::cout << "Try to add -> " << mesh->get_name() << std::endl;

	mesh_ordered_references_.push_back(mesh);
	meshes_.insert(std::pair<BEmesh*, glm::mat4>(mesh, glm::mat4(1)));

	if (meshes_by_name_.find(mesh->get_name()) == meshes_by_name_.end())
		meshes_by_name_.insert(std::pair<std::string, BEmesh*>(mesh->get_name(), mesh));
	else
		std::cout << mesh->get_name() << " already exists!" << std::endl;
}

void BElist::AddMesh(BEmesh*mesh, glm::mat4 f)
{
	std::cout << "Try to add -> " << mesh->get_name() << std::endl;

	mesh_ordered_references_.push_back(mesh);
	meshes_.insert(std::pair<BEmesh*, glm::mat4>(mesh, f));

	if (meshes_by_name_.find(mesh->get_name()) == meshes_by_name_.end())
		meshes_by_name_.insert(std::pair<std::string, BEmesh*>(mesh->get_name(), mesh));
	else
		std::cout << mesh->get_name() << " already exists!" << std::endl;
}

void BElist::UpdateMesh(BEmesh*mesh, glm::mat4 f)
{
	meshes_.find(mesh)->second = f;
}

BEmesh* BElist::GetMeshByName(std::string name)
{
	auto search = meshes_by_name_.find(name);
	if (search != meshes_by_name_.end())
		return search->second;
	else
		return nullptr;
}

/************************************************************************/
/* Light
/************************************************************************/
void BElist::AddLight(BElight* light)
{
	lights_.insert(std::pair<BElight*, glm::mat4>(light, glm::mat4(1)));
}

void BElist::AddLight(BElight* light, glm::mat4 f)
{
	lights_.insert(std::pair<BElight*, glm::mat4>(light, f));
}
void BElist::UpdateLight(BElight* light, glm::mat4 f)
{
	lights_.find(light)->second = f;
}


/************************************************************************/
/* Camera
/************************************************************************/
void BElist::AddCamera(BEcamera* camera)
{
	cameras_.insert(std::pair<BEcamera*, glm::mat4>(camera, glm::mat4(1)));
}

void BElist::AddCamera(BEcamera* camera, glm::mat4 f)
{
	cameras_.insert(std::pair<BEcamera*, glm::mat4>(camera, f));
}

void BElist::UpdateCamera(BEcamera* camera, glm::mat4 f)
{
	cameras_.find(camera)->second = f;
}
