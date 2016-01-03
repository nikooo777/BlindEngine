#include "be_mesh.h"


BEmesh::BEmesh()
{
}

BEmesh::BEmesh(glm::vec3* verts, glm::vec3* norms, glm::vec2* tex_coords, BEmaterial* mat, std::string name): BEnode(name)
{
	this->vertices_ = verts;
	this->normals_ = norms;
	this->texture_coords = tex_coords;
	this->material_ = mat;
}

BEmesh::BEmesh(glm::vec3* verts, glm::vec3* norms, glm::vec2* tex_coords, BEmaterial* mat) : BEnode()
{
	this->vertices_ = verts;
	this->normals_ = norms;
	this->texture_coords = tex_coords;
	this->material_ = mat;
}



BEmesh::~BEmesh()
{
}
