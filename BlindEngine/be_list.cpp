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
	//RenderCameras();
}

void LIB_API BElist::RenderMeshes()
{
	SetupStencil();

	// we need to render them first in order to prepare the stencil buffer
	RenderTransparentMeshes();

	// after this call, anything drawn outside the stencil buffer area is discarded
	EnableStencilFiltering();

	// Clear the transparent meshes from the buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Rendering only reflected meshes
	RenderReflectedMeshes();

	// toggle stencil off
	glDisable(GL_STENCIL_TEST);

	// Rendering "normal" meshes
	RenderOpaqueMeshes();

	// Rendering transparents objects
	RenderTransparentMeshes();

	RenderShadows();
}

void LIB_API BElist::RenderLights()
{
	for (const auto& pair : lights_)
	{
		pair.first->Render(pair.second);
	}
}

//void LIB_API BElist::RenderCameras()
//{
//	for (const auto& pair : cameras_)
//	{
//		pair.first->Render(pair.second);
//	}
//}

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
// Shadows
/************************************************************************/

void BElist::PassShadowed(BEmesh*mesh, glm::mat4 world_coords)
{
	for (auto m : shadows_v_)
	{
		if (m->mesh_ == mesh)
		{
			m->world_coords_ = world_coords;
			return;
		}
	}
}


void LIB_API BElist::RemoveShadows(BEmesh*mesh)
{
	unsigned int i = 0;
	bool found = false;

	for (auto m : shadows_v_)
	{
		if (m->mesh_ == mesh)
		{
			found = true;
			break;
		}
		i++;
	}

	if (found)
		shadows_v_.erase(shadows_v_.begin() + i);
}


/************************************************************************/
/* Mesh
/************************************************************************/
void LIB_API BElist::AddMesh(BEmesh*mesh)
{
	mesh_ordered_references_.push_back(mesh);

	Mesh* mesh_to_add = new Mesh;
	mesh_to_add->mesh_ = mesh;
	mesh_to_add->world_coords_ = glm::mat4();
	meshes_v_.push_back(mesh_to_add);

	Mesh* mesh_shad = new Mesh;
	memcpy(mesh_shad, mesh_to_add, sizeof(Mesh));

	if(mesh->get_material() && !mesh->get_material()->IsTransparent())
		shadows_v_.push_back(mesh_shad);
		//AddShadowed(mesh);
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


LIB_API void BElist::DeepSort()
{
	std::sort(meshes_v_.begin(), meshes_v_.end(), [](Mesh* a, Mesh* b)
	{
		return a->world_coords_[3].z > b->world_coords_[3].z;
	});

	/*std::sort(mirrored_v_.begin(), mirrored_v_.end(), [](Mesh* a, Mesh* b)
	{
		return a->world_coords_[3].z > b->world_coords_[3].z;
	});*/
}

void BElist::RenderOpaqueMeshes()
{
	// restore buffer access to read/write
	glDepthMask(GL_TRUE);
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
	//set buffer access to read only
	glDepthMask(GL_FALSE);
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
	//restore buffer access to read/write
	glDepthMask(GL_TRUE);
}


void BElist::RenderReflectedMeshes()
{
	glDepthMask(GL_TRUE); //make sure the buffer is accessible
	glCullFace(GL_FRONT);
	for (auto m : mirrored_v_)
	{
		m->mesh_->Render(m->world_coords_);
	}
	glCullFace(GL_BACK);
}


void BElist::RenderShadows()
{
	// restore buffer access to read/write
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	//glDisable(GL_LIGHTING);

	for (auto m : shadows_v_)
	{
		m->mesh_->SetShadowRender(true);
		m->mesh_->Render(m->world_coords_); //doesn't fucking work shit shit shit
		m->mesh_->SetShadowRender(false);
	}

	//glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);
	//glDepthMask(GL_TRUE);
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
}

/************************************************************************/
/* Light
/************************************************************************/
void LIB_API BElist::AddLight(BElight* light)
{
	lights_.insert(std::pair<BElight*, glm::mat4>(light, glm::mat4(1)));
}

void LIB_API BElist::Pass(BElight* light, glm::mat4 world_coords)
{
	lights_.find(light)->second = world_coords;
}

/************************************************************************/
// Camera
/************************************************************************/
//void LIB_API BElist::AddCamera(BEcamera* camera)
//{
//	cameras_.insert(std::pair<BEcamera*, glm::mat4>(camera, glm::mat4(1)));
//}
//
//void LIB_API BElist::Pass(BEcamera* camera, glm::mat4 world_coords)
//{
//	cameras_.find(camera)->second = world_coords;
//}

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


