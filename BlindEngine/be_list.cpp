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

	//RenderMirrored(); //non renderizzarle qui. il render delle mirrored viene chiamato dal render delle mesh
	RenderMeshes();
	RenderCameras();
}

void LIB_API BElist::RenderMeshes()
{

	RenderOpaqueMeshes();

	//set buffer access to read only
	glDepthMask(GL_FALSE);

	SetupStencil();

	DeepSort();

	RenderTransparentMeshes();

	EnableStencilFiltering();

	RenderReflectedMeshes();

	glCullFace(GL_BACK);

	//toggle stencil off
	glDisable(GL_STENCIL_TEST);
	
	//restore buffer access to read/write
	glDepthMask(GL_TRUE);
}

void LIB_API BElist::RenderLights()
{
	for (const auto& pair : lights_)
	{
		pair.first->Render(pair.second);
	}
}
void LIB_API BElist::RenderCameras()
{
	for (const auto& pair : cameras_)
	{
		pair.first->Render(pair.second);
	}
}

//in teoria questa funzione dovresti eliminarla completamente e ciò che hai qui dentro deve andare in RenderReflectedMeshes()
LIB_API void BElist::RenderMirrored()
{
	glDepthMask(GL_TRUE);
	glCullFace(GL_FRONT);
	for (auto m : mirrored_v_)
	{
		m->mesh_->Render(m->world_coords_);
	}
	glCullFace(GL_BACK);
}

/************************************************************************/
// Mirrored
/************************************************************************/
void BElist::PassMirrored(BEmesh*mesh, glm::mat4 world_coords)
{
	for (auto m : mirrored_v_)
	{
		if (m->mesh_ == mesh)
		{
			m->world_coords_ = world_coords;
			return;
		}
	}
}


void LIB_API BElist::AddMirrored(BEmesh*mesh)
{
	Mesh* mesh_to_add = new Mesh;
	mesh_to_add->mesh_ = mesh;
	mesh_to_add->world_coords_ = glm::mat4(1);
	mirrored_v_.push_back(mesh_to_add);
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
}

void LIB_API BElist::AddMeshToMap(BEmesh* mesh)
{
	bool mesh_found = false;
	for (auto m : meshes_v_)
	{
		if (m->mesh_ == mesh)
		{
			mesh_found = true;
			break;
		}
	}
	if (!mesh_found)
	{
		PushMesh(mesh);
	}
}

void BElist::Pass(BEmesh*mesh, glm::mat4 world_coords)
{
	for (auto m : meshes_v_)
	{
		if (m->mesh_ == mesh)
		{
			m->world_coords_ = world_coords;
			return;
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

	std::sort(mirrored_v_.begin(), mirrored_v_.end(), [](Mesh* a, Mesh* b)
	{
		return a->world_coords_[3].z > b->world_coords_[3].z;
	});
}

void BElist::RenderOpaqueMeshes()
{
	for (auto m : meshes_v_)
	{
		if (m->mesh_->get_material() && !m->mesh_->get_material()->IsTransparent())
		{
			m->mesh_->Render(m->world_coords_);
		}
	}
}

void BElist::RenderTransparentMeshes()
{
	for (auto m : meshes_v_)
	{
		if (m->mesh_->get_material() && m->mesh_->get_material()->IsTransparent())
		{
			glCullFace(GL_FRONT);
			m->mesh_->Render(m->world_coords_);
			glCullFace(GL_BACK);
			m->mesh_->Render(m->world_coords_);
		}
	}
}


void BElist::RenderReflectedMeshes()
{
	//metti il tuo codice che chiama il render dalla lista che necessiti tu
}

void BElist::SetupStencil()
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
}


void BElist::EnableStencilFiltering()
{
	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glDepthMask(GL_TRUE);
	glCullFace(GL_FRONT);
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
void LIB_API BElist::Pass(BElight* light, glm::mat4 world_coords)
{
	lights_.find(light)->second = world_coords;
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

void LIB_API BElist::Pass(BEcamera* camera, glm::mat4 world_coords)
{
	cameras_.find(camera)->second = world_coords;
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

void BElist::PushMesh(BEmesh* mesh)
{
	Mesh* mesh_to_add = new Mesh;
	mesh_to_add->mesh_ = mesh;
	mesh_to_add->world_coords_ = glm::mat4();
	meshes_v_.push_back(mesh_to_add);
}
