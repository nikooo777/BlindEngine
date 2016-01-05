#include "be_mesh.h"


BEmesh::BEmesh()
{
}

BEmesh::BEmesh(glm::vec3* vertices, long vertices_count, glm::vec3* normals, glm::vec2* texture_coords, BEmaterial* material, std::string name)
{
	vertices_ = vertices;
	normals_ = normals;
	texture_coords_ = texture_coords;
	material_ = material;
	vertices_count_ = vertices_count;
}

BEmesh::BEmesh(glm::vec3* vertices, long vertices_count, glm::vec3* normals, glm::vec2* texture_coords, BEmaterial* material) : BEnode()
{
	vertices_ = vertices;
	normals_ = normals;
	texture_coords_ = texture_coords;
	material_ = material;
	vertices_count_ = vertices_count;
}

BEmesh::~BEmesh()
{
	delete vertices_;
	delete normals_;
	delete texture_coords_;
	delete material_;
}

void BEmesh::Render()
{
	//TODO: eventually try other methods
	//glVertexPointer(3, GL_FLOAT, 0, vertices_);
	//glDrawArrays(GL_TRIANGLES, 0, vertices_count_);
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	std::cout << "Rendering a Mesh ";
	PrintName();
	std::cout << "Children count: " << BEnode::GetChildren().size() << std::endl;

	glBegin(GL_TRIANGLES);
	for (long i = 0; i < vertices_count_; i++)
	{
		std::cout << " vertex " << i << ": x: " << vertices_[i].x << " y: " << vertices_[i].y << " z: " << vertices_[i].z << std::endl;
		glVertex3fv(glm::value_ptr(vertices_[i]));
	}
	glEnd();

	for each (BEnode* n in BEnode::children_){
		n->Render();
	}
}

void BEmesh::AddVertex(glm::vec3 vertex)
{
	// vertices_.push_back(vertex);
}

void BEmesh::AddNormal(glm::vec3 normal)
{
	// normals_.push_back(normal);
}

void BEmesh::AddFace(glm::vec3 face)
{
	// faces.push_back(face);
}

void BEmesh::AddTextureCoord(glm::vec2 texture_coord)
{
	// texture_coords_.push_back(texture_coord);
}

void BEmesh::SetMaterial(BEmaterial* material)
{
	material_ = material;
}