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
void LIB_API BElist::RenderAll()
{
	RenderLights();
	RenderMeshes();
	RenderCameras();
}

void LIB_API BElist::RenderMeshes()
{
	for (auto m : meshes_v_)
	{
		if (!m->mesh_->get_material()->IsTransparent())
		{
			m->mesh_->RenderSingle(m->world_coords_);
		}
	}
 	glDepthMask(GL_FALSE);
	this->DeepSort();
	for (auto m : meshes_v_)
	{
		if (m->mesh_->get_material()->IsTransparent())
		{
			glCullFace(GL_FRONT);
			m->mesh_->RenderSingle(m->world_coords_);
			glCullFace(GL_BACK);
			m->mesh_->RenderSingle(m->world_coords_);
		}
	}
	//glCullFace(GL_FRONT);
	glDepthMask(GL_TRUE);
}

void LIB_API BElist::RenderLights()
{
	for (const auto& pair : lights_)
	{
		pair.first->RenderSingle(pair.second);
	}
}
void LIB_API BElist::RenderCameras()
{
	for (const auto& pair : cameras_)
	{
		pair.first->RenderSingle(pair.second);
	}
}


/************************************************************************/
/* Mesh
/************************************************************************/
void LIB_API BElist::AddMesh(BEmesh*mesh)
{
	mesh_ordered_references_.push_back(mesh);
	AddMesh(mesh, glm::mat4(1));
}

void LIB_API BElist::AddMesh(BEmesh*mesh, glm::mat4 f)
{
	Mesh* mesh_to_add = new Mesh;
	mesh_to_add->mesh_ = mesh;
	mesh_to_add->world_coords_ = f;
	meshes_v_.push_back(mesh_to_add);
	//meshes_.insert(std::pair<BEmesh*, glm::mat4>(mesh, f));
}

void LIB_API BElist::AddMeshToMap(BEmesh* mesh)
{
	/*if (meshes_by_name_.find(mesh->get_name()) == meshes_by_name_.end())
	meshes_by_name_.insert(std::pair<std::string, BEmesh*>(mesh->get_name(), mesh));*/
	if (meshes_v_.size() == 0)
	{
		Mesh* mesh_to_add = new Mesh;
		mesh_to_add->mesh_ = mesh;
		mesh_to_add->world_coords_ = glm::mat4();
		meshes_v_.push_back(mesh_to_add);
		std::cout << "adding mesh to list" << std::endl;
		return;
	}
	for (auto m : meshes_v_)
	{
		if (m->mesh_ == mesh)
		{
			std::cout << "adding mesh to list" << std::endl;
			break;
		}
		else if (m == *(meshes_v_.end()-1))
		{
			Mesh* mesh_to_add = new Mesh;
			mesh_to_add->mesh_ = mesh;
			mesh_to_add->world_coords_ = glm::mat4();
			meshes_v_.push_back(mesh_to_add);
			std::cout << "adding mesh to list" << std::endl;
			break;
		}
	}
}

void LIB_API BElist::UpdateMesh(BEmesh* mesh, glm::mat4 f)
{
	for (auto m : meshes_v_)
	{
		if (m->mesh_ == mesh)
		{
			m->world_coords_ = f;
		}
	}

}

LIB_API BEmesh* BElist::GetMeshByName(std::string name)
{
	auto search = meshes_by_name_.find(name);
	if (search != meshes_by_name_.end())
		return search->second;
	else
		return nullptr;
}


LIB_API void BElist::DeepSort()
{
	std::sort(meshes_v_.begin(), meshes_v_.end(), [](Mesh* a, Mesh* b)
	{
		return a->world_coords_[3].z > b->world_coords_[3].z;
	});
}


/************************************************************************/
/* Light
/************************************************************************/
void LIB_API BElist::AddLight(BElight* light)
{
	lights_.insert(std::pair<BElight*, glm::mat4>(light, glm::mat4(1)));
}

void LIB_API BElist::AddLight(BElight* light, glm::mat4 f)
{
	lights_.insert(std::pair<BElight*, glm::mat4>(light, f));
}
void LIB_API BElist::UpdateLight(BElight* light, glm::mat4 f)
{
	lights_.find(light)->second = f;
}


/************************************************************************/
/* Camera
/************************************************************************/
void LIB_API BElist::AddCamera(BEcamera* camera)
{
	cameras_.insert(std::pair<BEcamera*, glm::mat4>(camera, glm::mat4(1)));
}

void LIB_API BElist::AddCamera(BEcamera* camera, glm::mat4 f)
{
	cameras_.insert(std::pair<BEcamera*, glm::mat4>(camera, f));
}

void LIB_API BElist::UpdateCamera(BEcamera* camera, glm::mat4 f)
{
	cameras_.find(camera)->second = f;
}


BEmesh* BElist::GetMesh(unsigned int index)
{
	return mesh_ordered_references_[index];
}

void BElist::AddMaterial(BEmaterial* material)
{
	material_ordered_list_.push_back(material);
}

BEmaterial* BElist::GetMaterial(unsigned int index)
{
	return material_ordered_list_[index];
}

unsigned int BElist::GetMaterialCount()
{
	return material_ordered_list_.size();
}

unsigned int BElist::GetMeshCount()
{
	return meshes_v_.size();
}
