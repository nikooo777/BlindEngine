#pragma once
// DLL
#include "be_dll_interface.h"

#include "be_node.h"
#include "be_material.h"
#include "be_includes.h"

class LIB_API BEmesh : public BEnode
{
public:
	//BEmesh();
	BEmesh(std::string, glm::vec3*, long, glm::vec3*, glm::vec2*, BEmaterial*, unsigned int, unsigned int*);
	//BEmesh(glm::vec3*, long, glm::vec3*, glm::vec2*, BEmaterial*);
	~BEmesh();

	//virtual void Render(glm::mat4 world_matrix) override;
	virtual void Render(glm::mat4 world_matrix) override;
	virtual void CalcTransformation(glm::mat4 world_matrix) override;

	void SetVertices(glm::vec3 *vertices){ vertices_ = vertices; }
	void SetVerticesCount(unsigned int vertices_count){ vertices_count_ = vertices_count; }
	void SetNormals(glm::vec3 *normals){ normals_ = normals; }
	void SetFaces(glm::vec3 *faces){ faces_ = faces; }
	void SetTextureCoords(glm::vec2 *texture_coords){ texture_coords_ = texture_coords; }
	void SetMaterial(BEmaterial *material){ material_ = material; }

	void SetSubMeshes(unsigned int sub_meshes_count, unsigned int* sub_meshes, unsigned int delta);
	void SetIsMirrored(bool should_be_mirrored){ to_mirror_ = should_be_mirrored; }
	bool GetIsMirrored(){ return to_mirror_; }
	void SetIsShadowed(bool should_be_shadowed){ to_shadow_ = should_be_shadowed; }
	bool GetIsShadowed(){ return to_shadow_; }

	void SetShadowRender(bool shadow_render){ shadow_render_ = shadow_render; }

	// Utility
	virtual BEnode* Find(std::string) override;
	virtual BEnode* Find(long) override;
	BEmaterial* get_material();
private:
	unsigned int vertices_count_ = 0;
	glm::vec3* vertices_;
	glm::vec3* normals_;
	glm::vec3* faces_;
	glm::vec2* texture_coords_;
	BEmaterial* material_;

	unsigned int* sub_meshes_;
	unsigned int sub_meshes_count_;
	bool to_mirror_ = false;
	bool to_shadow_ = true;
	bool shadow_render_ = false;
};