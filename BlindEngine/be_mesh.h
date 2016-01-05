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
	void Render() override;

	void SetVertices(glm::vec3 *vertices){ vertices_ = vertices; }
	void SetVerticesCount(unsigned int vertices_count){ vertices_count_ = vertices_count; }
	void SetNormals(glm::vec3 *normals){ normals_ = normals; }
	void SetFaces(glm::vec3 *faces){ faces_ = faces; }
	void SetTextureCoords(glm::vec2 *texture_coords){ texture_coords_ = texture_coords; }
	void SetMaterial(BEmaterial *material){ material_ = material; }

private:
	unsigned int vertices_count_ = 0;
	glm::vec3* vertices_;
	glm::vec3* normals_;
	glm::vec3* faces_;
	glm::vec2* texture_coords_;
	BEmaterial* material_;
};