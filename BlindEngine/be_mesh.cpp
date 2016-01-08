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
}

//BEmesh::BEmesh(glm::vec3* vertices, long vertices_count, glm::vec3* normals, glm::vec2* texture_coords, BEmaterial* material) : BEnode()
//{
//	vertices_ = vertices;
//	normals_ = normals;
//	texture_coords_ = texture_coords;
//	material_ = material;
//	vertices_count_ = vertices_count;
//}

BEmesh::~BEmesh()
{
	delete[] vertices_;
	delete[] normals_;
	delete[] texture_coords_;
	delete[] material_;
}

void BEmesh::Render(glm::mat4 cumulated_transformation_matrix)
{
	//TODO: eventually try other methods
	//glVertexPointer(3, GL_FLOAT, 0, vertices_);
	//glDrawArrays(GL_TRIANGLES, 0, vertices_count_);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//std::cout << "Rendering Mesh: " << BEobject::get_name() << std::endl;
	/*
	std::cout << "Children count: " << BEnode::GetChildren().size() << std::endl;
	*/
	glm::mat4 tmpF = cumulated_transformation_matrix*transformation_;
	glLoadMatrixf(glm::value_ptr(tmpF));

	if (material_)
	{
		//std::cout << "The mesh contains a material"<< std::endl;
		material_->Render(tmpF);
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
			tmp_mesh->Render(tmpF);
	}

	for each (BEnode* n in BEnode::children_){
		n->Render(tmpF);
	}
}

void BEmesh::SetSubMeshes(unsigned int sub_meshes_count, unsigned int* sub_meshes)
{
	sub_meshes_count_ = sub_meshes_count;

	if (sub_meshes_count)
	{
		if (sub_meshes_)
			delete[] sub_meshes_;

		sub_meshes_ = new unsigned int[sub_meshes_count];

		for (unsigned int i = 0; i < sub_meshes_count; i++)
		{
			sub_meshes_[i] = sub_meshes[i];
		}
	}
}
