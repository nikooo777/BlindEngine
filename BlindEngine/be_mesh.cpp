#include "be_mesh.h"
#include "be_engine.h"

BEmesh::BEmesh(std::string name, glm::vec3* vertices, long vertices_count, glm::vec3* normals, glm::vec2* texture_coords, BEmaterial* material, unsigned int sub_meshes_count, unsigned int* sub_meshes) : BEnode(name, MESH)
{
	vertices_ = vertices;
	normals_ = normals;
	texture_coords_ = texture_coords;
	material_ = material;
	vertices_count_ = vertices_count;
	sub_meshes_count_ = sub_meshes_count;

	if (sub_meshes_count_)
	{
		sub_meshes_ = new unsigned int[sub_meshes_count];
		for (unsigned int i = 0; i < sub_meshes_count; i++)
		{
			sub_meshes_[i] = sub_meshes[i];
		}
	}
	else
		sub_meshes_ = nullptr;

	if (material_ && material_->IsTransparent())
		to_shadow_ = false;
}

BEmesh::~BEmesh()
{
	delete[] vertices_;
	delete[] normals_;
	delete[] texture_coords_;
	delete[] material_;
}

void BEmesh::Render(glm::mat4 world_matrix)
{
	//std::cout << "Rendering Mesh: " << BEobject::get_name() << std::endl;
	glLoadMatrixf(glm::value_ptr(world_matrix));

	if (shadow_render_)
	{
		BEmaterial::mat_shadow->Render(world_matrix);
	}
	else if (material_)
	{
		material_->Render(world_matrix);
	}

	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < vertices_count_; i++)
	{
		glNormal3fv(glm::value_ptr(normals_[i]));
		glTexCoord2fv(glm::value_ptr(texture_coords_[i]));
		glVertex3fv(glm::value_ptr(vertices_[i]));
	}
	glEnd();

	for (unsigned int i = 0; i < sub_meshes_count_; i++)
	{
		BEmesh* tmp_mesh = BEengine::lists_->GetMesh(sub_meshes_[i]);
		if (tmp_mesh != this)
		{
			if (shadow_render_)
				tmp_mesh->SetShadowRender(true);
			
			tmp_mesh->Render(world_matrix);

			if (shadow_render_)
				tmp_mesh->SetShadowRender(false);
		}
	}
}

void BEmesh::CalcTransformation(glm::mat4 world_matrix)
{
	glm::mat4 tmpF = world_matrix*transformation_;
	BEengine::lists_->Pass(this, tmpF);

	if (to_mirror_)
	{
		glm::mat4 scaled = world_matrix * glm::scale(glm::mat4(1), glm::vec3(1, 1, -1)) * glm::translate(glm::mat4(), glm::vec3(.0f, .0f, -2 * glm::inverse(GetParent()->GetTransformation())[3].z))* transformation_;
		BEengine::lists_->PassMirrored(this, scaled);
	}

	if (to_shadow_)
	{
		glm::mat4 scaled = world_matrix * glm::translate(glm::mat4(), glm::vec3(.0f, .0f, .01f+glm::inverse(GetParent()->GetTransformation())[3].z))*  glm::scale(glm::mat4(1), glm::vec3(1.1, 1.1, 0)) * transformation_;
		BEengine::lists_->PassShadowed(this, scaled);
	}

	for (auto n : BEnode::children_){
		n->CalcTransformation(tmpF);
	}
}

void BEmesh::SetSubMeshes(unsigned int sub_meshes_count, unsigned int* sub_meshes, unsigned int delta)
{
	sub_meshes_count_ = sub_meshes_count;

	if (sub_meshes_count)
	{
		if (sub_meshes_)
			delete[] sub_meshes_;

		sub_meshes_ = new unsigned int[sub_meshes_count];

		for (unsigned int i = 0; i < sub_meshes_count; i++)
		{
			sub_meshes_[i] = sub_meshes[i] + delta;
		}
	}
}


BEnode* BEmesh::Find(std::string name)
{
	//the current node is the one sought
	if (this->get_name().compare(name) == 0)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for (BEnode* n : children_)
	{
		if ((found_node = n->Find(name)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}

BEnode* BEmesh::Find(long id)
{
	//the current node is the one sought
	if (this->get_id() == id)
	{
		return this;
	}

	//seek the node in the children
	BEnode *found_node = nullptr;
	for (BEnode* n : children_)
	{
		if ((found_node = n->Find(id)) != nullptr)
			return found_node;
	}
	//there are no more children to look into
	return nullptr;
}

BEmaterial* BEmesh::get_material()
{
	return material_;
}