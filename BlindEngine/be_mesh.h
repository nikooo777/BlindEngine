#pragma once
#include "be_node.h"
#include "be_material.h"
#include "be_includes.h"

class BEmesh : public BEnode
{
public:
	BEmesh();
	BEmesh(glm::vec3*, long, glm::vec3*, glm::vec2*, BEmaterial*, std::string);
	BEmesh(glm::vec3*, long, glm::vec3*, glm::vec2*, BEmaterial*);
	~BEmesh();
	virtual void Render() override;

	void AddVertex(glm::vec3);
	void AddNormal(glm::vec3);
	void AddFace(glm::vec3);
	void AddTextureCoord(glm::vec2);
	void SetMaterial(BEmaterial*);

private:
	glm::vec3* vertices_;
	glm::vec3* normals_;
	glm::vec2* faces;
	glm::vec2* texture_coords_;
	long vertices_count_;
	BEmaterial* material_;
};