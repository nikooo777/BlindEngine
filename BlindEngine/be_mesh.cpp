#include "be_mesh.h"

BEmesh::BEmesh(std::string name, glm::vec3* vertices, long vertices_count, glm::vec3* normals, glm::vec2* texture_coords, BEmaterial* material) : BEnode(name,MESH)
{
	vertices_ = vertices;
	normals_ = normals;
	texture_coords_ = texture_coords;
	material_ = material;
	vertices_count_ = vertices_count;
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

void BEmesh::Render(glm::mat4 f)
{
	//TODO: eventually try other methods
	//glVertexPointer(3, GL_FLOAT, 0, vertices_);
	//glDrawArrays(GL_TRIANGLES, 0, vertices_count_);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	std::cout << "Rendering Mesh: " << BEobject::get_name() << std::endl;
	/*
	std::cout << "Children count: " << BEnode::GetChildren().size() << std::endl;
	*/
	glm::mat4 tmpF = f*transformation_;
	glLoadMatrixf(glm::value_ptr(tmpF));
	

	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0 ; i < vertices_count_; i++)
	{
		//std::cout << " vertex " << i << ": x: " << vertices_[i].x << " y: " << vertices_[i].y << " z: " << vertices_[i].z << std::endl;
		glNormal3fv(glm::value_ptr(normals_[i]));
		glVertex3fv(glm::value_ptr(vertices_[i]));
	}
	glEnd();

	if (material_ != nullptr)
		material_->Render(f);

	for each (BEnode* n in BEnode::children_){
		n->Render(tmpF);
	}
}
